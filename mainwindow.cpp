#include <QListWidget>
#include "mainwindow.h"
#include "window.h"
#include "ui_mainwindow.h"
#include "newprojectdialog.h"
#include <QSettings>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 启用右键菜单
    ui->recentProjectsList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->recentProjectsList, &QListWidget::customContextMenuRequested, this, &MainWindow::showContextMenu);

    // 加载最近项目列表
    QSettings settings("MyCompany", "MyApp");
    QStringList projects = settings.value("recentProjects").toStringList();
    ui->recentProjectsList->addItems(projects);
    setWindowIcon(QIcon("G-max.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_NewButton_clicked()
{
    NewProjectDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        QString projectName = dialog.getProjectName();
        QString projectPath = dialog.getProjectPath();
        // 保存到最近项目列表
        // 打开新窗口
        // 组合项目信息
        QString projectInfo = projectName + " - " + projectPath;

        // 添加到最近项目列表
        ui->recentProjectsList->addItem(projectInfo);

        // 保存到最近项目列表
        QSettings settings("MyCompany", "MyApp");
        QStringList projects = settings.value("recentProjects").toStringList();
        projects.append(projectInfo);
        settings.setValue("recentProjects", projects);
    }
}


void MainWindow::on_recentProjectsList_itemDoubleClicked(QListWidgetItem *item)
{
    QStringList parts = item->text().split(" - ");
    QString projectPath = parts.last();  // 获取最后一个元素
    QString projectName = parts.first();
      // 打开新窗口
    class window *projectWindow = new class window();
    projectWindow->setWindowTitle("Gmax:" + projectName);
    projectWindow->show();

    this->hide();  // 隐藏当前窗口
}

void MainWindow::showContextMenu(const QPoint &pos)
{
    // 获取右键点击的项目
    QListWidgetItem *item = ui->recentProjectsList->itemAt(pos);
    if (!item) return;  // 如果没有点击到项目，直接返回

    // 创建右键菜单
    QMenu contextMenu(this);
    QAction *deleteAction = contextMenu.addAction("删除");

    // 连接删除操作的信号和槽
    connect(deleteAction, &QAction::triggered, this, [this, item]() {

        // 从 QSettings 中移除项目
        QSettings settings("MyCompany", "MyApp");
        QStringList projects = settings.value("recentProjects").toStringList();
        projects.removeOne(item->text());  // 移除匹配的项目
        settings.setValue("recentProjects", projects);
          // 从列表中移除项目
          delete ui->recentProjectsList->takeItem(ui->recentProjectsList->row(item));

    });

    // 显示右键菜单
    contextMenu.exec(ui->recentProjectsList->viewport()->mapToGlobal(pos));
}

