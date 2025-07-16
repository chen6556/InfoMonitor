#pragma once
#include <QGuiApplication>
#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QSystemTrayIcon>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon m_TrayIcon;
    QMenu m_TrayMenu;
    QAction m_QuitAction;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void Init();

    void SystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void ChangeTheme(const QString &theme);
};