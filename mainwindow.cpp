#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <seal/seal.h>
#include <QDebug>
#include <sstream>
#include <vector>
#include <iomanip>
#include <QString>

using namespace seal;

// Base64 编码函数
static QString toBase64(const seal::Ciphertext &ciphertext) {
    std::stringstream ss;
    ciphertext.save(ss);
    std::string binary = ss.str();
    static const char* b64_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string result;
    result.reserve((binary.size() + 2) / 3 * 4);
    for (size_t i = 0; i < binary.size(); i += 3) {
        int val = (binary[i] & 0xFF) << 16;
        if (i + 1 < binary.size()) val |= (binary[i + 1] & 0xFF) << 8;
        if (i + 2 < binary.size()) val |= (binary[i + 2] & 0xFF);
        result.push_back(b64_table[(val >> 18) & 0x3F]);
        result.push_back(b64_table[(val >> 12) & 0x3F]);
        if (i + 1 < binary.size()) result.push_back(b64_table[(val >> 6) & 0x3F]);
        else result.push_back('=');
        if (i + 2 < binary.size()) result.push_back(b64_table[val & 0x3F]);
        else result.push_back('=');
    }
    return QString::fromStdString(result);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->encryptButton, &QPushButton::clicked, this, &MainWindow::encryptData);
    connect(ui->compareButton, &QPushButton::clicked, this, &MainWindow::compareData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化SEAL上下文和密钥
void MainWindow::initializeSEAL()
{
    // 设置BFV方案参数
    EncryptionParameters parms(scheme_type::bfv);
    size_t poly_modulus_degree = 4096;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
    parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree, 30)); // 增大明文模数

    // 创建SEAL上下文
    context = std::make_shared<SEALContext>(parms);

    // 生成密钥
    KeyGenerator keygen(*context);
    secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    encryptor = std::make_unique<Encryptor>(*context, public_key);
    evaluator = std::make_unique<Evaluator>(*context);
    decryptor = std::make_unique<Decryptor>(*context, secret_key);
    batch_encoder = std::make_unique<BatchEncoder>(*context);
}

// 加密工资数据
void MainWindow::encryptData()
{
    initializeSEAL();

    bool ok1, ok2;
    int salary1 = ui->salary1Input->text().toInt(&ok1);
    int salary2 = ui->salary2Input->text().toInt(&ok2);

    if (!ok1 || !ok2 || salary1 < 0 || salary2 < 0) {
        QMessageBox::warning(this, "输入错误", "请输入有效的非负整数。");
        return;
    }

    // 检查输入是否超出明文模数
    uint64_t plain_modulus_value = context->first_context_data()->parms().plain_modulus().value();
    if (static_cast<uint64_t>(salary1) >= plain_modulus_value || static_cast<uint64_t>(salary2) >= plain_modulus_value) {
        QMessageBox::warning(this, "输入错误", QString("输入值超出明文模数范围 (%1)。").arg(plain_modulus_value));
        return;
    }

    // 编码和加密第一个工资
    std::vector<uint64_t> salary1_vec(batch_encoder->slot_count(), 0ULL);
    salary1_vec[0] = static_cast<uint64_t>(salary1);
    Plaintext plain_salary1;
    batch_encoder->encode(salary1_vec, plain_salary1);
    encryptor->encrypt(plain_salary1, encrypted_salary1);

    // 编码和加密第二个工资
    std::vector<uint64_t> salary2_vec(batch_encoder->slot_count(), 0ULL);
    salary2_vec[0] = static_cast<uint64_t>(salary2);
    Plaintext plain_salary2;
    batch_encoder->encode(salary2_vec, plain_salary2);
    encryptor->encrypt(plain_salary2, encrypted_salary2);

    // 将密文转换为 Base64 并显示（限制长度）
    QString ciphertext1 = toBase64(encrypted_salary1).left(100) + "...";
    QString ciphertext2 = toBase64(encrypted_salary2).left(100) + "...";
    QString output = QString("工资 1 密文 (前100字符):\n%1\n\n工资 2 密文 (前100字符):\n%2").arg(ciphertext1).arg(ciphertext2);
    ui->ciphertextOutput->setText(output);

    ui->statusLabel->setText("数据加密成功！");
}

// 比较加密后的工资
void MainWindow::compareData()
{
    // 检查密文是否有效
    if (encrypted_salary1.size() == 0 || encrypted_salary2.size() == 0) {
        QMessageBox::warning(this, "错误", "请先加密数据。");
        return;
    }

    // 计算差值：salary1 - salary2
    Ciphertext diff;
    evaluator->sub(encrypted_salary1, encrypted_salary2, diff);

    // 检查噪声预算
    auto noise_budget = decryptor->invariant_noise_budget(diff);
    if (noise_budget == 0) {
        QMessageBox::warning(this, "错误", "噪声预算耗尽，解密可能不准确。");
        return;
    }
    qDebug() << "Noise budget after subtraction:" << noise_budget;

    // 解密差值
    Plaintext plain_diff;
    decryptor->decrypt(diff, plain_diff);
    std::vector<uint64_t> result_vec;
    batch_encoder->decode(plain_diff, result_vec);

    // 处理负数
    int64_t difference = static_cast<int64_t>(result_vec[0]);
    uint64_t plain_modulus_value = context->first_context_data()->parms().plain_modulus().value();
    if (difference > plain_modulus_value / 2) {
        difference -= plain_modulus_value; // 转换为负数
    }

    // 判断结果
    QString result;
    if (difference > 0) {
        result = "工资 1 大于工资 2。";
    } else if (difference < 0) {
        result = "工资 2 大于工资 1。";
    } else {
        result = "工资 1 等于工资 2。";
    }

    // 验证正确性
    bool ok1, ok2;
    int original_salary1 = ui->salary1Input->text().toInt(&ok1);
    int original_salary2 = ui->salary2Input->text().toInt(&ok2);
    QString verification = QString("验证：原始差值 (工资1 - 工资2) = %1").arg(original_salary1 - original_salary2);

    ui->resultLabel->setText(result + "\n" + verification);
}
