#include "newprojectdialog.h"
#include "ui_newprojectdialog.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
NewProjectDialog::NewProjectDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewProjectDialog)
{
    ui->setupUi(this);
}

NewProjectDialog::~NewProjectDialog()
{
    delete ui;
}
// 实现 getProjectName 方法
QString NewProjectDialog::getProjectName() const
{
    return ui->projectNameEdit->text();  // 假设项目名称输入框的 objectName 是 projectNameEdit
}

// 实现 getProjectPath 方法
QString NewProjectDialog::getProjectPath() const
{
    return ui->projectPathEdit->text();  // 假设项目路径输入框的 objectName 是 projectPathEdit
}

void NewProjectDialog::on_browseButton_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "选择项目路径");
    if (!path.isEmpty()) {
        ui->projectPathEdit->setText(path);  // 将选择的路径设置到输入框中
    }
}


void NewProjectDialog::on_buttonBox_accepted()
{
    QString projectName = ui->projectNameEdit->text();
    QString projectPath = ui->projectPathEdit->text();

    if (projectName.isEmpty() || projectPath.isEmpty()) {
        QMessageBox::warning(this, "错误", "项目名称和路径不能为空！");
        return;
    }

    // 创建项目文件路径
    QString filePath = projectPath + "/" + projectName + ".yu";

    // 创建文件
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "Project Name: " << projectName << "\n";
        out << "Project Path: " << projectPath << "\n";
        file.close();

        // 关闭对话框并返回 Accepted
        this->accept();
    } else {
        QMessageBox::critical(this, "错误", "无法创建项目文件！");
    }
}

