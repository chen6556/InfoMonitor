#pragma once
#include <QMainWindow>
#include <QCloseEvent>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QTranslator>


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
    QAction m_ShowAction;
    QAction m_QuitAction;
    QTranslator *m_Translator;

public:
    MainWindow(QTranslator *translator, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void Init();

    void SystemTrayIconActivated(QSystemTrayIcon::ActivationReason reason);

    void ChangeTheme(const QString &theme);

    void Retranslate(const QString &lang);

    void ReadSetting();

    void ShowMessage(const QString &title, const QString &message);
};