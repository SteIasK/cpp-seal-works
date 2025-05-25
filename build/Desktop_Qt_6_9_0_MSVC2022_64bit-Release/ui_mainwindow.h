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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *input1;
    QLabel *label_2;
    QLineEdit *input2;
    QHBoxLayout *horizontalLayout;
    QPushButton *compareButton;
    QPushButton *compareFloatButton;
    QPushButton *loadFileButton;
    QLabel *resultLabel;
    QLabel *encryptedLabel;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(600, 500);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName("groupBox");
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setObjectName("gridLayout");
        label = new QLabel(groupBox);
        label->setObjectName("label");

        gridLayout->addWidget(label, 0, 0, 1, 1);

        input1 = new QLineEdit(groupBox);
        input1->setObjectName("input1");

        gridLayout->addWidget(input1, 0, 1, 1, 1);

        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        input2 = new QLineEdit(groupBox);
        input2->setObjectName("input2");

        gridLayout->addWidget(input2, 1, 1, 1, 1);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        compareButton = new QPushButton(centralwidget);
        compareButton->setObjectName("compareButton");

        horizontalLayout->addWidget(compareButton);

        compareFloatButton = new QPushButton(centralwidget);
        compareFloatButton->setObjectName("compareFloatButton");

        horizontalLayout->addWidget(compareFloatButton);

        loadFileButton = new QPushButton(centralwidget);
        loadFileButton->setObjectName("loadFileButton");

        horizontalLayout->addWidget(loadFileButton);


        verticalLayout->addLayout(horizontalLayout);

        resultLabel = new QLabel(centralwidget);
        resultLabel->setObjectName("resultLabel");
        resultLabel->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(resultLabel);

        encryptedLabel = new QLabel(centralwidget);
        encryptedLabel->setObjectName("encryptedLabel");
        encryptedLabel->setMinimumHeight(60);
        encryptedLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        encryptedLabel->setWordWrap(true);

        verticalLayout->addWidget(encryptedLabel);

        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "\350\226\252\350\265\204\345\212\240\345\257\206\346\257\224\350\276\203\347\263\273\347\273\237", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\350\276\223\345\205\245\345\214\272\345\237\237", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\226\252\350\265\2041\357\274\232", nullptr));
        input1->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\347\254\254\344\270\200\344\270\252\350\226\252\350\265\204\345\200\274", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\350\226\252\350\265\2042\357\274\232", nullptr));
        input2->setPlaceholderText(QCoreApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\347\254\254\344\272\214\344\270\252\350\226\252\350\265\204\345\200\274", nullptr));
        compareButton->setText(QCoreApplication::translate("MainWindow", "\346\225\264\346\225\260\346\257\224\350\276\203", nullptr));
        compareFloatButton->setText(QCoreApplication::translate("MainWindow", "\346\265\256\347\202\271\346\225\260\346\257\224\350\276\203", nullptr));
        loadFileButton->setText(QCoreApplication::translate("MainWindow", "\344\273\216\346\226\207\344\273\266\345\212\240\350\275\275", nullptr));
        resultLabel->setText(QCoreApplication::translate("MainWindow", "\346\257\224\350\276\203\347\273\223\346\236\234\345\260\206\345\234\250\350\277\231\351\207\214\346\230\276\347\244\272", nullptr));
        encryptedLabel->setText(QCoreApplication::translate("MainWindow", "\345\212\240\345\257\206\346\225\260\346\215\256\345\260\206\345\234\250\350\277\231\351\207\214\346\230\276\347\244\272", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
