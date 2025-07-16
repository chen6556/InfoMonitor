#include <QDir>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "base/UniqueResource.hpp"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_TrayIcon(this), m_TrayMenu(this), m_QuitAction(this)
{
    ui->setupUi(this);
    Init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    hide();
    return event->ignore();
}

void MainWindow::Init()
{
    QDir::setCurrent(QGuiApplication::applicationDirPath());

    m_TrayIcon.setIcon(QIcon(":/images/InfoMonitor.svg"));
    m_TrayIcon.setToolTip("InfoMonitor");

    m_QuitAction.setText(tr("Quit"));
    m_QuitAction.connect(&m_QuitAction, &QAction::triggered, []() { QGuiApplication::quit(); });

    m_TrayMenu.addAction(&m_QuitAction);
    m_TrayIcon.setContextMenu(&m_TrayMenu);
    m_TrayIcon.show();
    m_TrayIcon.connect(&m_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::SystemTrayIconActivated);

    ChangeTheme("default-light");
}

void MainWindow::SystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::ActivationReason::DoubleClick:
        showNormal();
        break;
    default:
        break;
    }
}

void MainWindow::ChangeTheme(const QString &theme)
{
    if (QFile file(":/theme/theme/" + theme + ".qss"); file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        setStyleSheet(file.readAll());
        file.close();
        update();
    }
}