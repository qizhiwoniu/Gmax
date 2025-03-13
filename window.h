#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include "workspace2dwidget.h"
#include "workspace3dwidget.h"
struct WindowPrivate;
namespace Ui {
class window;
}

class window : public QMainWindow
{
    Q_OBJECT
private:
    WindowPrivate* d;///< private data - pimpl
    friend struct WindowPrivate;
protected:
    virtual void closeEvent(QCloseEvent* event) override;
public:
    explicit window(QWidget *parent = nullptr);
    ~window();
    ads::CDockWidget* createWorkspaceDockWidget(int type); // 声明工作区窗口创建函数
private slots:

    void on_action_11_triggered();

    void on_action_14_triggered();

    void on_action_15_triggered();

    void on_action_17_triggered();

    void on_action_18_triggered();

    void lockWorkspace(bool Value);
    void createEditor();
    void showMainWindow();
private:
    Ui::window *ui;
    ads::CDockManager* DockManager;
    QByteArray m_ba;  // 添加 m_ba 变量
    ads::CDockManager* m_pMgr;
    QLabel *m_pLabel;  // Optional: for status bar or other purposes


};

#endif // WINDOW_H





