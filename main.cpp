#include <QApplication>
#include <QFile>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 加载样式表
    QFile styleFile("E:\\QT\\QTProject\\SEAL\\style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        app.setStyleSheet(styleSheet);
    }

    MainWindow window;
    window.show();

    return app.exec();
}
