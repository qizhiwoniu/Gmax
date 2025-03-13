#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Gmax");
    w.show();

    // 设置暗色模式样式表
    QString darkStyleSheet = R"(
        QMenuBar {
            background-color: #2E2E2E;
            color: #FFFFFF;
            border: 1px solid #444444;
        }
        QMenuBar::item {
            background-color: transparent;
            padding: 5px 10px;
        }
        QMenuBar::item:selected {
            background-color: #555555;
        }
        QMenu {
            background-color: #2E2E2E;
            color: #FFFFFF;
            border: 1px solid #444444;
        }
        QMenu::item {
            padding: 5px 20px;
        }
        QMenu::item:selected {
            background-color: #555555;
        }
    )";

    // 应用样式表
    a.setStyleSheet(darkStyleSheet);








    return a.exec();
}
