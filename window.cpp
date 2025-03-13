#include "window.h"
#include "ui_window.h"
#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include "DockWidget.h"
#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockComponentsFactory.h"
#include "DockManager.h"
#include "DockSplitter.h"
#include "DockWidget.h"
#include "FloatingDockContainer.h"

#include <QTime>
#include <QLabel>
#include <QTextEdit>
#include <QFrame>
#include <QTreeView>
#include <QFileSystemModel>
#include <QBoxLayout>
#include <QSettings>
#include <QDockWidget>
#include <QDebug>
#include <QResizeEvent>
#include <QAction>
#include <QWidgetAction>
#include <QComboBox>
#include <QInputDialog>
#include <QRubberBand>
#include <QPlainTextEdit>
#include <QTableWidget>
#include <QScreen>
#include <QStyle>
#include <QMessageBox>
#include <QMenu>
#include <QToolButton>
#include <QToolBar>
#include <QPointer>
#include <QMap>
#include <QElapsedTimer>


#define VERSION 1  // 定义版本号

window::~window()
    {
        delete ui;
    }

    void window::on_action_11_triggered()
    {
        m_ba = DockManager->saveState(VERSION);
        m_pLabel->setText("保存布局.");
    }

    void window::on_action_14_triggered()
    {
        bool bRestore = DockManager->restoreState(m_ba, VERSION);
        if (bRestore)
            m_pLabel->setText("加载布局成功.");
    }
static QIcon svgIcon(const QString& File)
{
        // This is a workaround, because in item views SVG icons are not
        // properly scaled and look blurry or pixelate
        QIcon SvgIcon(File);
        SvgIcon.addPixmap(SvgIcon.pixmap(92));
        return SvgIcon;
}

static QString featuresString(ads::CDockWidget* DockWidget)
{
    auto f = DockWidget->features();
    return QString("c%1 m%2 f%3")
        .arg(f.testFlag(ads::CDockWidget::DockWidgetClosable) ? "+" : "-")
        .arg(f.testFlag(ads::CDockWidget::DockWidgetMovable) ? "+" : "-")
        .arg(f.testFlag(ads::CDockWidget::DockWidgetFloatable) ? "+" : "-");
}
static void appendFeaturStringToWindowTitle(ads::CDockWidget* DockWidget)
{
    DockWidget->setWindowTitle(DockWidget->windowTitle()
        +  QString(" (%1)").arg(featuresString(DockWidget)));
}

struct WindowPrivate
{
    window* _this;
    Ui::window ui;
    QAction* SavePerspectiveAction = nullptr;
    QWidgetAction* PerspectiveListAction = nullptr;
    QComboBox* PerspectiveComboBox = nullptr;
    ads::CDockManager* DockManager = nullptr;
    ads::CDockWidget* WindowTitleTestDockWidget = nullptr;
    QPointer<ads::CDockWidget> LastDockedEditor;
    QPointer<ads::CDockWidget> LastCreatedFloatingEditor;

    WindowPrivate(window* _public) : _this(_public) {}

    void createActions();
    void createContent();
    ads::CDockWidget* createEditorWidget()
    {
        static int EditorCount = 0;
        QPlainTextEdit* w = new QPlainTextEdit();
        w->setPlaceholderText("This is an editor. If you close the editor, it will be "
                              "deleted. Enter your text here.");
        w->setStyleSheet("border: none");
        ads::CDockWidget* DockWidget = DockManager->createDockWidget(QString("Editor %1").arg(EditorCount++));
        DockWidget->setWidget(w);
        DockWidget->setFeature(ads::CDockWidget::CustomCloseHandling, true);
        ui.menu_5->addAction(DockWidget->toggleViewAction());

        QMenu* OptionsMenu = new QMenu(DockWidget);
        OptionsMenu->setTitle(QObject::tr("Options"));
        OptionsMenu->setToolTip(OptionsMenu->title());
        auto MenuAction = OptionsMenu->menuAction();
        // The object name of the action will be set for the QToolButton that
        // is created in the dock area title bar. You can use this name for CSS
        // styling
        MenuAction->setObjectName("optionsMenu");
        DockWidget->setTitleBarActions({OptionsMenu->menuAction()});
        auto a = OptionsMenu->addAction(QObject::tr("Clear Editor"));
        w->connect(a, SIGNAL(triggered()), SLOT(clear()));

        return DockWidget;
    }
    ads::CDockWidget* createFileSystemTreeDockWidget()
    {
        static int FileSystemCount = 0;
        QTreeView* w = new QTreeView();
        w->setFrameShape(QFrame::NoFrame);
        QFileSystemModel* m = new QFileSystemModel(w);
        m->setRootPath(QDir::currentPath());
        w->setModel(m);
        w->setRootIndex(m->index(QDir::currentPath()));
        ads::CDockWidget* DockWidget = DockManager->createDockWidget(QString("资源面板")
                                                                         .arg(FileSystemCount++));
        DockWidget->setWidget(w);
        ui.menu_5->addAction(DockWidget->toggleViewAction());
        w->setFocusPolicy(Qt::NoFocus);
        //auto ToolBar = DockWidget->createDefaultToolBar();
        return DockWidget;
    }
    ads::CDockWidget* createWorkspaceDockWidget(int type)
    {
        static int WorkspaceCount = 0;
        QWidget* workspace = nullptr;

        if (type == 0) {
            workspace = new Workspace2DWidget();
        } else if (type == 1) {
            workspace = new Workspace3DWidget();
        }

        // 创建 DockWidget
        ads::CDockWidget* DockWidget = DockManager->createDockWidget(QString("工作区 %1").arg(WorkspaceCount++));
        DockWidget->setWidget(workspace);
        ui.menu_5->addAction(DockWidget->toggleViewAction());

        return DockWidget;
    }
    ads::CDockWidget* createOutputDockWidget()
    {
        static int OutputCount = 0;

        // 创建一个 QPlainTextEdit 作为输出窗口
        QPlainTextEdit* outputTextEdit = new QPlainTextEdit();
        outputTextEdit->setReadOnly(true); // 设置为只读，防止用户编辑
        outputTextEdit->setPlaceholderText("消息输出窗口"); // 设置占位文本
        outputTextEdit->setStyleSheet("border: none"); // 设置无边框样式

        ads::CDockWidget* DockWidget = DockManager->createDockWidget(QString("输出面板").arg(OutputCount++));
        DockWidget->setWidget(outputTextEdit); // 将 QPlainTextEdit 设置为 DockWidget 的内容
        ui.menu_5->addAction(DockWidget->toggleViewAction()); // 将 DockWidget 的切换视图操作添加到菜单中
        // 设置 DockWidget 的特性
        DockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
        DockWidget->setFeature(ads::CDockWidget::DockWidgetMovable, true);
        DockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, true);

        return DockWidget;
    }
    ads::CDockWidget* createAttributeDockWidget()
    {
        static int AttributeCount = 0;

        // 创建一个 QTableWidget 作为属性表格
        QTableWidget* attributeTable = new QTableWidget();
        attributeTable->setColumnCount(2); // 两列：属性名和属性值
        attributeTable->setHorizontalHeaderLabels({"属性", "值"}); // 设置表头
        attributeTable->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed); // 允许编辑
        attributeTable->setSelectionBehavior(QAbstractItemView::SelectRows); // 选择整行
        attributeTable->setStyleSheet("border: none"); // 设置无边框样式

        // 添加一些示例属性
        QStringList attributes = {"名称", "类型", "宽度", "高度", "颜色"};
        attributeTable->setRowCount(attributes.size());
        for (int i = 0; i < attributes.size(); ++i) {
            QTableWidgetItem* nameItem = new QTableWidgetItem(attributes[i]);
            QTableWidgetItem* valueItem = new QTableWidgetItem("未设置");
            attributeTable->setItem(i, 0, nameItem); // 设置属性名
            attributeTable->setItem(i, 1, valueItem); // 设置属性值
        }

        // 创建一个 DockWidget
        ads::CDockWidget* DockWidget = DockManager->createDockWidget(QString("属性窗口").arg(AttributeCount++));
        DockWidget->setWidget(attributeTable); // 将 QTableWidget 设置为 DockWidget 的内容
        ui.menu_5->addAction(DockWidget->toggleViewAction()); // 将 DockWidget 的切换视图操作添加到菜单中

        // 设置 DockWidget 的特性
        DockWidget->setFeature(ads::CDockWidget::DockWidgetClosable, true);
        DockWidget->setFeature(ads::CDockWidget::DockWidgetMovable, true);
        DockWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, true);

        return DockWidget;
    }

private slots:
    void on_action_20_triggered();
};
window::window(QWidget *parent) :
    QMainWindow(parent),
    d(new WindowPrivate(this))
{
    using namespace ads;
    d->ui.setupUi(this);
    setWindowTitle(QApplication::instance()->applicationName());
    d->createActions();


    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);

    CDockManager::setAutoHideConfigFlags({CDockManager::DefaultAutoHideConfig});

    d->DockManager = new CDockManager(this);
    d->DockManager->setDockWidgetToolBarStyle(Qt::ToolButtonIconOnly, ads::CDockWidget::StateFloating);

#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
#else
    connect(d->PerspectiveComboBox, SIGNAL(textActivated(QString)),
            d->DockManager, SLOT(openPerspective(QString)));
#endif

    d->createContent();
    // Default window geometry - center on screen
    resize(1280, 720);
    setGeometry(QStyle::alignedRect(
        Qt::LeftToRight, Qt::AlignCenter, frameSize(),
        QGuiApplication::primaryScreen()->availableGeometry()
        ));

    //d->restoreState();
    //	d->restorePerspectives();
}

void window::showMainWindow()
{
    static MainWindow* w = nullptr;
    if (!w) {
        w = new MainWindow();
    }
    w->show();
    w->raise();
    w->activateWindow();
}
void window::createEditor()
{
    QObject* Sender = sender();
    QVariant vFloating = Sender->property("Floating");
    bool Floating = vFloating.isValid() ? vFloating.toBool() : true;
    QVariant vTabbed = Sender->property("Tabbed");
    bool Tabbed = vTabbed.isValid() ? vTabbed.toBool() : true;
    auto DockWidget = d->createEditorWidget();
    DockWidget->setFeature(ads::CDockWidget::DockWidgetDeleteOnClose, true);
    DockWidget->setFeature(ads::CDockWidget::DockWidgetForceCloseWithArea, true);
    connect(DockWidget, SIGNAL(closeRequested()), SLOT(onEditorCloseRequested()));

    if (Floating)
    {
        auto FloatingWidget = d->DockManager->addDockWidgetFloating(DockWidget);
        FloatingWidget->move(QPoint(20, 20));
        d->LastCreatedFloatingEditor = DockWidget;
        d->LastDockedEditor.clear();
        return;
    }


    ads::CDockAreaWidget* EditorArea = d->LastDockedEditor ? d->LastDockedEditor->dockAreaWidget() : nullptr;
    if (EditorArea)
    {
        if (Tabbed)
        {
            // Test inserting the dock widget tab at a given position instead
            // of appending it. This function inserts the new dock widget as
            // first tab
            d->DockManager->addDockWidgetTabToArea(DockWidget, EditorArea, 0);
        }
        else
        {
            d->DockManager->setConfigFlag(ads::CDockManager::EqualSplitOnInsertion, true);
            d->DockManager->addDockWidget(ads::RightDockWidgetArea, DockWidget, EditorArea);
        }
    }
    else
    {
        if (d->LastCreatedFloatingEditor)
        {
            d->DockManager->addDockWidget(ads::RightDockWidgetArea, DockWidget, d->LastCreatedFloatingEditor->dockAreaWidget());
        }
        else
        {
            d->DockManager->addDockWidget(ads::TopDockWidgetArea, DockWidget);
        }
    }
    d->LastDockedEditor = DockWidget;
}
//============================================================================
void WindowPrivate::createContent()
{

    auto FileSystemWidget = createFileSystemTreeDockWidget();
    FileSystemWidget->setFeature(ads::CDockWidget::DockWidgetFloatable, true);
    appendFeaturStringToWindowTitle(FileSystemWidget);
    DockManager->addDockWidget(ads::RightDockWidgetArea, FileSystemWidget);

    // Test custom factory - we inject a help button into the title bar
    DockManager->setComponentsFactory(new ads::CDockComponentsFactory());
    auto TopDockArea = DockManager->addDockWidget(ads::RightDockWidgetArea, FileSystemWidget);
    DockManager->setComponentsFactory(ads::CDockComponentsFactory::factory());

    // Test dock area docking
    auto RighDockArea = DockManager->addDockWidget(ads::LeftDockWidgetArea, createWorkspaceDockWidget(0), TopDockArea);
    auto BottomDockArea = DockManager->addDockWidget(ads::BottomDockWidgetArea, createOutputDockWidget(), RighDockArea);
    DockManager->addDockWidget(ads::CenterDockWidgetArea, createWorkspaceDockWidget(1), RighDockArea);
    auto LabelDockWidget = createAttributeDockWidget();
    DockManager->addDockWidget(ads::LeftDockWidgetArea, LabelDockWidget);

    // Tests CustomCloseHandling without DeleteOnClose
    LabelDockWidget->setFeature(ads::CDockWidget::CustomCloseHandling, true);
    LabelDockWidget->setWindowTitle(LabelDockWidget->windowTitle() + "");
    QObject::connect(LabelDockWidget, &ads::CDockWidget::closeRequested, [LabelDockWidget, this]()
                     {
                         int Result = QMessageBox::question(_this, "Custom Close Request",
                                                            "Do you really want to close this dock widget?");
                         if (QMessageBox::Yes == Result)
                         {
                             LabelDockWidget->closeDockWidget();
                         }
                     });

#ifdef Q_OS_WIN
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    DockManager->addDockWidget(ads::CenterDockWidgetArea, createActiveXWidget(), RighDockArea);
#endif
#endif

    for (auto DockWidget : DockManager->dockWidgetsMap())
    {
        _this->connect(DockWidget, SIGNAL(viewToggled(bool)), SLOT(onViewToggled(bool)));
        _this->connect(DockWidget, SIGNAL(visibilityChanged(bool)), SLOT(onViewVisibilityChanged(bool)));
    }
}
//============================================================================
void WindowPrivate::createActions()
{

    QAction* b = ui.toolBar->addAction("home");
    b->setIcon(svgIcon(":/images/home-outline.svg"));
    QObject::connect(b, &QAction::triggered, _this, &window::showMainWindow);

    ui.toolBar->addSeparator();
    QAction* a = ui.toolBar->addAction("Lock Workspace");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/lock_outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);

    a = ui.toolBar->addAction("Create Floating Editor");
    a->setProperty("Floating", true);
    a->setIcon(svgIcon(":/images/note_add.svg"));
    a->setToolTip("Creates floating dynamic dockable editor windows that are deleted on close");
    _this->connect(a, SIGNAL(triggered()), SLOT(createEditor()));
    ui.menuTests->addAction(a);

    ui.toolBar->addSeparator();

    a = ui.toolBar->addAction("File");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/document-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    a = ui.toolBar->addAction("Folder");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/folder-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    a = ui.toolBar->addAction("Save");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/save-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    ui.toolBar->addSeparator();
    a = ui.toolBar_2->addAction("bug");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/bug-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    a = ui.toolBar_2->addAction("Play");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/play-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    a = ui.toolBar_2->addAction("stop");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/stop-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    ui.toolBar->addSeparator();
    a = ui.toolBar_3->addAction("setting");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/settings-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
    a = ui.toolBar_3->addAction("help");
    a->setCheckable(true);
    a->setChecked(false);
    a->setIcon(svgIcon(":/images/help-circle-outline.svg"));
    QObject::connect(a, &QAction::triggered, _this, &window::lockWorkspace);
}
//============================================================================
void window::lockWorkspace(bool Value)
{
    if (Value)
    {
       d->DockManager->lockDockWidgetFeaturesGlobally();
    }
    else
    {
       d->DockManager->lockDockWidgetFeaturesGlobally(ads::CDockWidget::NoDockWidgetFeatures);
    }
}
//============================================================================
void window::closeEvent(QCloseEvent* event)
{
    //	d->saveState();

    // 删除 DockManager
    if (d->DockManager) {
        d->DockManager->deleteLater();
    }

    // 调用基类的 closeEvent 处理
    QWidget::closeEvent(event);
}

