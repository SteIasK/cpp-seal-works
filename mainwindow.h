/*#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <seal/seal.h>

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
    void encryptData();
    void compareData();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<seal::SEALContext> context;
    seal::SecretKey secret_key;
    std::unique_ptr<seal::Encryptor> encryptor;
    std::unique_ptr<seal::Evaluator> evaluator;
    std::unique_ptr<seal::Decryptor> decryptor;
    std::unique_ptr<seal::BatchEncoder> batch_encoder;
    seal::Ciphertext encrypted_salary1;
    seal::Ciphertext encrypted_salary2;

    void initializeSEAL();
};

#endif // MAINWINDOW_H
