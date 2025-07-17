#include <QDir>
#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "base/UniqueResource.hpp"


MainWindow::MainWindow(QTranslator *translator, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow),
    m_TrayIcon(this), m_TrayMenu(this), m_ShowAction(this),
    m_QuitAction(this), m_Translator(translator)
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
    QDir::setCurrent(QApplication::applicationDirPath());

    m_TrayIcon.setIcon(QIcon(":/images/InfoMonitor.svg"));
    m_TrayIcon.setToolTip("InfoMonitor");

    connect(&m_ShowAction, &QAction::triggered, [this]() { isVisible() ? hide() : showNormal(); });
    connect(&m_QuitAction, &QAction::triggered, []() { QApplication::quit(); });

    m_TrayMenu.addAction(&m_ShowAction);
    m_TrayMenu.addAction(&m_QuitAction);
    m_TrayIcon.setContextMenu(&m_TrayMenu);
    m_TrayIcon.show();
    connect(&m_TrayIcon, &QSystemTrayIcon::activated, this, &MainWindow::SystemTrayIconActivated);

    connect(ui->cb_Language, &QComboBox::currentTextChanged, [this](const QString &lang) { Retranslate(lang); });
    connect(ui->cb_Theme, &QComboBox::currentTextChanged, [this](const QString &theme) { ChangeTheme(theme); });

    ReadSetting();
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
        GlobalConfig::Config().SetValue("Theme", theme);
        setStyleSheet(file.readAll());
        file.close();
        update();
    }
    else
    {
        GlobalConfig::Config().SetValue("Theme", "default-light");
        QFile file2(":/theme/theme/default-light.qss");
        file2.open(QIODevice::OpenModeFlag::ReadOnly);
        setStyleSheet(file2.readAll());
        file2.close();
        update();
    }
}

void MainWindow::Retranslate(const QString &lang)
{
    if (m_Translator->load(":/translation/translations/" + lang + ".qm"))
    {
        GlobalConfig::Config().SetValue("TimeAndWeather/Lang", lang);
        GlobalConfig::Config().SetValue("Language", lang);
        QApplication::installTranslator(m_Translator);
    }
    else
    {
        GlobalConfig::Config().SetValue("TimeAndWeather/Lang", "en");
        GlobalConfig::Config().SetValue("Language", "en");
        QApplication::removeTranslator(m_Translator);
    }
    m_ShowAction.setText(tr("Show"));
    m_QuitAction.setText(tr("Quit"));
    ui->retranslateUi(this);

    ui->TimeAndWeatherElement->UpdateForecast();
    ui->TimeAndWeatherElement->UpdateWeather();
}

void MainWindow::ReadSetting()
{
    ui->cb_Language->setCurrentText(GlobalConfig::Config().Value("Language").toString());
    Retranslate(GlobalConfig::Config().Value("Language").toString());
    ui->cb_Theme->setCurrentText(GlobalConfig::Config().Value("Theme").toString());
    ChangeTheme(GlobalConfig::Config().Value("Theme").toString());
}

void MainWindow::ShowMessage(const QString &title, const QString &message)
{
    m_TrayIcon.showMessage(title, message);
}