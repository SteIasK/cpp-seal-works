#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <seal/seal.h>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_compareButton_clicked();
    void on_compareFloatButton_clicked();
    void on_loadFileButton_clicked();

private:
    Ui::MainWindow *ui;

    // SEAL related members
    std::unique_ptr<seal::EncryptionParameters> bfv_params;
    std::unique_ptr<seal::EncryptionParameters> ckks_params;
    std::unique_ptr<seal::SEALContext> bfv_context;
    std::unique_ptr<seal::SEALContext> ckks_context;
    std::unique_ptr<seal::KeyGenerator> bfv_keygen;
    std::unique_ptr<seal::KeyGenerator> ckks_keygen;
    std::unique_ptr<seal::PublicKey> bfv_public_key;
    std::unique_ptr<seal::SecretKey> bfv_secret_key;
    std::unique_ptr<seal::PublicKey> ckks_public_key;
    std::unique_ptr<seal::SecretKey> ckks_secret_key;
    std::unique_ptr<seal::Encryptor> bfv_encryptor;
    std::unique_ptr<seal::Decryptor> bfv_decryptor;
    std::unique_ptr<seal::Encryptor> ckks_encryptor;
    std::unique_ptr<seal::Decryptor> ckks_decryptor;
    std::unique_ptr<seal::Evaluator> bfv_evaluator;
    std::unique_ptr<seal::Evaluator> ckks_evaluator;

    void initializeSEAL();
    bool compareBFV(int64_t value1, int64_t value2);
    bool compareCKKS(double value1, double value2);
};
#endif // MAINWINDOW_H
