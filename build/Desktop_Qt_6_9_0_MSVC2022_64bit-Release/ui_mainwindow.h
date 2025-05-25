/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QLineEdit *salary1Input;
    QLabel *label_2;
    QLineEdit *salary2Input;
    QPushButton *encryptButton;
    QPushButton *compareButton;
    QLabel *statusLabel;
    QLabel *resultLabel;
    QLabel *ciphertextLabel;
    QTextEdit *ciphertextOutput;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(400, 400);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        label = new QLabel(centralwidget);
        label->setObjectName("label");

        verticalLayout->addWidget(label);

        salary1Input = new QLineEdit(centralwidget);
        salary1Input->setObjectName("salary1Input");

        verticalLayout->addWidget(salary1Input);

        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");

        verticalLayout->addWidget(label_2);

        salary2Input = new QLineEdit(centralwidget);
        salary2Input->setObjectName("salary2Input");

        verticalLayout->addWidget(salary2Input);

        encryptButton = new QPushButton(centralwidget);
        encryptButton->setObjectName("encryptButton");

        verticalLayout->addWidget(encryptButton);

        compareButton = new QPushButton(centralwidget);
        compareButton->setObjectName("compareButton");

        verticalLayout->addWidget(compareButton);

        statusLabel = new QLabel(centralwidget);
        statusLabel->setObjectName("statusLabel");

        verticalLayout->addWidget(statusLabel);

        resultLabel = new QLabel(centralwidget);
        resultLabel->setObjectName("resultLabel");

        verticalLayout->addWidget(resultLabel);

        ciphertextLabel = new QLabel(centralwidget);
        ciphertextLabel->setObjectName("ciphertextLabel");

        verticalLayout->addWidget(ciphertextLabel);

        ciphertextOutput = new QTextEdit(centralwidget);
        ciphertextOutput->setObjectName("ciphertextOutput");
        ciphertextOutput->setReadOnly(true);

        verticalLayout->addWidget(ciphertextOutput);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Homomorphic Encryption Demo", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\267\245\350\265\204 1:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\267\245\350\265\204 2:", nullptr));
        encryptButton->setText(QCoreApplication::translate("MainWindow", "\345\212\240\345\257\206\346\225\260\346\215\256", nullptr));
        compareButton->setText(QCoreApplication::translate("MainWindow", "\346\257\224\350\276\203\345\267\245\350\265\204", nullptr));
        statusLabel->setText(QCoreApplication::translate("MainWindow", "\347\212\266\346\200\201: \347\255\211\345\276\205\350\276\223\345\205\245", nullptr));
        resultLabel->setText(QCoreApplication::translate("MainWindow", "\347\273\223\346\236\234: ", nullptr));
        ciphertextLabel->setText(QCoreApplication::translate("MainWindow", "\345\212\240\345\257\206\345\257\206\346\226\207:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
