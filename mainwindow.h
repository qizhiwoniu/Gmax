#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qlistwidget.h>
#include <QListWidgetItem>
#include <QMenu>
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
    void on_NewButton_clicked();

    void on_recentProjectsList_itemDoubleClicked(QListWidgetItem *item);
    void showContextMenu(const QPoint &pos);  // 右键菜单

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
