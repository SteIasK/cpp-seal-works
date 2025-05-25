#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>
#include <sstream>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    try {
        initializeSEAL();
    }
    catch (const std::exception& e) {
        QMessageBox::critical(this, "初始化错误", QString("SEAL初始化失败: %1").arg(e.what()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initializeSEAL()
{
    // Initialize BFV parameters
    bfv_params = std::make_unique<seal::EncryptionParameters>(seal::scheme_type::bfv);
    size_t poly_modulus_degree = 8192; // 增加多项式模数度
    bfv_params->set_poly_modulus_degree(poly_modulus_degree);
    bfv_params->set_coeff_modulus(seal::CoeffModulus::BFVDefault(poly_modulus_degree));
    bfv_params->set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, 40)); // 增加明文模数位数

    bfv_context = std::make_unique<seal::SEALContext>(*bfv_params);

    // 验证参数是否有效
    if (!bfv_context->parameters_set()) {
        throw std::runtime_error("BFV参数设置无效");
    }

    bfv_keygen = std::make_unique<seal::KeyGenerator>(*bfv_context);

    // Create keys
    bfv_public_key = std::make_unique<seal::PublicKey>();
    bfv_secret_key = std::make_unique<seal::SecretKey>();

    // Generate keys
    bfv_keygen->create_public_key(*bfv_public_key);
    *bfv_secret_key = bfv_keygen->secret_key();

    bfv_encryptor = std::make_unique<seal::Encryptor>(*bfv_context, *bfv_public_key);
    bfv_decryptor = std::make_unique<seal::Decryptor>(*bfv_context, *bfv_secret_key);
    bfv_evaluator = std::make_unique<seal::Evaluator>(*bfv_context);

    // 重新配置CKKS参数
    try {
        ckks_params = std::make_unique<seal::EncryptionParameters>(seal::scheme_type::ckks);
        size_t ckks_poly_modulus_degree = 16384; // 增加CKKS的多项式模数度
        ckks_params->set_poly_modulus_degree(ckks_poly_modulus_degree);

        // 使用更保守的模数链
        std::vector<int> modulus_bits;
        if (ckks_poly_modulus_degree == 16384) {
            modulus_bits = {60, 40, 40, 40, 40, 40, 60}; // 适合16384的模数链
        } else {
            throw std::runtime_error("不支持的CKKS多项式模数度");
        }

        auto coeff_modulus = seal::CoeffModulus::Create(ckks_poly_modulus_degree, modulus_bits);
        ckks_params->set_coeff_modulus(coeff_modulus);

        ckks_context = std::make_unique<seal::SEALContext>(*ckks_params);

        if (!ckks_context->parameters_set()) {
            throw std::runtime_error("CKKS参数设置无效");
        }

        qDebug() << "CKKS context created successfully";

        ckks_keygen = std::make_unique<seal::KeyGenerator>(*ckks_context);
        ckks_public_key = std::make_unique<seal::PublicKey>();
        ckks_secret_key = std::make_unique<seal::SecretKey>();

        ckks_keygen->create_public_key(*ckks_public_key);
        *ckks_secret_key = ckks_keygen->secret_key();

        ckks_encryptor = std::make_unique<seal::Encryptor>(*ckks_context, *ckks_public_key);
        ckks_decryptor = std::make_unique<seal::Decryptor>(*ckks_context, *ckks_secret_key);
        ckks_evaluator = std::make_unique<seal::Evaluator>(*ckks_context);

        qDebug() << "CKKS initialization completed";
    }
    catch (const std::exception& e) {
        qDebug() << "CKKS initialization failed:" << e.what();
        throw;
    }
}

bool MainWindow::compareBFV(int64_t value1, int64_t value2)
{
    try {
        seal::Plaintext plain1, plain2;
        seal::Ciphertext cipher1, cipher2, result;

        // Debug输出原始值
        qDebug() << "BFV比较原始值:" << value1 << "vs" << value2;

        // 使用编码器进行编码
        seal::BatchEncoder encoder(*bfv_context);
        std::vector<int64_t> pod_matrix1(encoder.slot_count(), value1);
        std::vector<int64_t> pod_matrix2(encoder.slot_count(), value2);
        encoder.encode(pod_matrix1, plain1);
        encoder.encode(pod_matrix2, plain2);

        // Encrypt
        bfv_encryptor->encrypt(plain1, cipher1);
        bfv_encryptor->encrypt(plain2, cipher2);

        // 显示加密结果（Base64格式）
        {
            std::ostringstream cipher1_stream;
            cipher1.save(cipher1_stream);
            std::string cipher1_str = cipher1_stream.str();

            std::ostringstream cipher2_stream;
            cipher2.save(cipher2_stream);
            std::string cipher2_str = cipher2_stream.str();

            // 转换为Base64
            QByteArray cipher1_data(cipher1_str.c_str(), cipher1_str.length());
            QByteArray cipher2_data(cipher2_str.c_str(), cipher2_str.length());

            QString cipher1_base64 = cipher1_data.toBase64();
            QString cipher2_base64 = cipher2_data.toBase64();

            QString displayText = QString("加密后的值1 (Base64前100字符): %1...\n\n加密后的值2 (Base64前100字符): %2...")
                                      .arg(cipher1_base64.left(100))
                                      .arg(cipher2_base64.left(100));
            ui->encryptedLabel->setText(displayText);
        }

        // 计算差值
        bfv_evaluator->sub(cipher1, cipher2, result);

        // Decrypt result
        seal::Plaintext decrypted_result;
        bfv_decryptor->decrypt(result, decrypted_result);

        // Decode
        std::vector<int64_t> decoded_result;
        encoder.decode(decrypted_result, decoded_result);

        // Debug输出解密结果
        qDebug() << "BFV解密结果:" << decoded_result[0];

        return decoded_result[0] >= 0;  // 大于等于返回true，小于返回false
    }
    catch (const std::exception& e) {
        QMessageBox::warning(nullptr, "比较错误", QString("BFV比较操作失败: %1").arg(e.what()));
        return false;
    }
}

bool MainWindow::compareCKKS(double value1, double value2)
{
    try {
        if (!ckks_context || !ckks_context->parameters_set()) {
            throw std::runtime_error("CKKS上下文未正确初始化");
        }

        // Debug输出原始值
        qDebug() << "CKKS比较原始值:" << value1 << "vs" << value2;

        seal::CKKSEncoder encoder(*ckks_context);
        double scale = pow(2.0, 30);

        if (std::abs(value1) > 1e6 || std::abs(value2) > 1e6) {
            throw std::runtime_error("输入值过大，请使用小于1e6的数值");
        }

        std::vector<double> input1 = {value1};
        std::vector<double> input2 = {value2};

        seal::Plaintext plain1, plain2;
        encoder.encode(input1, scale, plain1);
        encoder.encode(input2, scale, plain2);

        seal::Ciphertext cipher1, cipher2;
        ckks_encryptor->encrypt(plain1, cipher1);
        ckks_encryptor->encrypt(plain2, cipher2);

        // 显示加密结果（Base64格式）
        {
            std::ostringstream cipher1_stream;
            cipher1.save(cipher1_stream);
            std::string cipher1_str = cipher1_stream.str();

            std::ostringstream cipher2_stream;
            cipher2.save(cipher2_stream);
            std::string cipher2_str = cipher2_stream.str();

            // 转换为Base64
            QByteArray cipher1_data(cipher1_str.c_str(), cipher1_str.length());
            QByteArray cipher2_data(cipher2_str.c_str(), cipher2_str.length());

            QString cipher1_base64 = cipher1_data.toBase64();
            QString cipher2_base64 = cipher2_data.toBase64();

            QString displayText = QString("加密后的值1 (Base64前100字符): %1...\n\n加密后的值2 (Base64前100字符): %2...")
                                      .arg(cipher1_base64.left(100))
                                      .arg(cipher2_base64.left(100));
            ui->encryptedLabel->setText(displayText);
        }

        // 计算差值
        seal::Ciphertext result;
        ckks_evaluator->sub(cipher1, cipher2, result);

        seal::Plaintext decrypted_result;
        ckks_decryptor->decrypt(result, decrypted_result);

        std::vector<double> decoded_result;
        encoder.decode(decrypted_result, decoded_result);

        // Debug输出解密结果
        qDebug() << "CKKS解密结果:" << decoded_result[0];

        const double epsilon = 1e-6;
        return decoded_result[0] >= -epsilon;  // 大于等于返回true，小于返回false
    }
    catch (const std::exception& e) {
        QString errorMsg = QString("CKKS比较操作失败: %1").arg(e.what());
        QMessageBox::warning(nullptr, "比较错误", errorMsg);
        qDebug() << errorMsg;
        return false;
    }
}

void MainWindow::on_compareButton_clicked()
{
    bool ok1, ok2;
    int64_t value1 = ui->input1->text().toLongLong(&ok1);
    int64_t value2 = ui->input2->text().toLongLong(&ok2);

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "输入错误", "请输入有效的整数值");
        return;
    }

    bool result = compareBFV(value1, value2);
    QString compareResult;
    if (result) compareResult = "大于或等于";
    else compareResult = "小于";

    QString message = QString("加密比较结果：\n%1 %2 %3")
                          .arg(value1)
                          .arg(compareResult)
                          .arg(value2);

    ui->resultLabel->setText(message);
}

void MainWindow::on_compareFloatButton_clicked()
{
    bool ok1, ok2;
    double value1 = ui->input1->text().toDouble(&ok1);
    double value2 = ui->input2->text().toDouble(&ok2);

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "输入错误", "请输入有效的浮点数值");
        return;
    }

    bool result = compareCKKS(value1, value2);
    QString compareResult;
    if (result) compareResult = "大于或等于";
    else compareResult = "小于";

    QString message = QString("加密比较结果（浮点数）：\n%1 %2 %3")
                          .arg(value1)
                          .arg(compareResult)
                          .arg(value2);

    ui->resultLabel->setText(message);
}

void MainWindow::on_loadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "选择文件", "",
                                                    "文本文件 (*.txt);;CSV文件 (*.csv);;所有文件 (*)");

    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件");
        return;
    }

    QTextStream in(&file);
    QStringList values;

    // 读取第一行，检查文件格式
    QString firstLine = in.readLine();
    if (firstLine.isEmpty()) {
        QMessageBox::warning(this, "错误", "文件为空");
        file.close();
        return;
    }

    // 检查文件格式
    bool isCSV = fileName.endsWith(".csv", Qt::CaseInsensitive);
    if (isCSV) {
        // CSV格式：处理第一行
        values = firstLine.split(',', Qt::SkipEmptyParts);
        // 如果第一行是标题，则读取下一行
        if (values[0].contains("薪资", Qt::CaseInsensitive) ||
            values[0].contains("工资", Qt::CaseInsensitive) ||
            values[0].contains("数值", Qt::CaseInsensitive)) {
            QString dataLine = in.readLine();
            if (!dataLine.isEmpty()) {
                values = dataLine.split(',', Qt::SkipEmptyParts);
            }
        }
    } else {
        // 普通文本格式：每行一个数值
        values.append(firstLine.trimmed());
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                values.append(line);
            }
        }
    }

    file.close();

    // 验证数值
    if (values.size() < 2) {
        QMessageBox::warning(this, "错误", "文件格式不正确，需要至少两个值");
        return;
    }

    // 验证数值格式
    bool ok1, ok2;
    if (ui->compareButton->isEnabled()) {
        // 整数模式
        values[0].toLongLong(&ok1);
        values[1].toLongLong(&ok2);
    } else {
        // 浮点数模式
        values[0].toDouble(&ok1);
        values[1].toDouble(&ok2);
    }

    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "错误", "文件中的数值格式不正确");
        return;
    }

    // 设置值到输入框
    ui->input1->setText(values[0].trimmed());
    ui->input2->setText(values[1].trimmed());

    QMessageBox::information(this, "成功", "文件加载成功！");
}
