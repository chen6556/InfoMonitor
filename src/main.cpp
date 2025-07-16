#include <QApplication>
#include <QTranslator>
#include <QSharedMemory>
#include <QMessageBox>

#include "ui/MainWindow.hpp"


int main(int argc, char *argv[])
{
    QSharedMemory memory("NotificationClient");
    if (memory.attach())
    {
        QMessageBox::warning(nullptr, QApplication::tr("Warning"), QApplication::tr("Application is already running."));
        return 0;
    }
    memory.create(1);

    QApplication a(argc, argv);
    a.setStyle("Fusion");

    QTranslator translator;
    switch (QLocale::system().language())
    {
    case QLocale::Language::Chinese:
        if (translator.load(":/translation/translations/zh_cn.qm"))
        {
            a.installTranslator(&translator);
        }
        break;
    default:
        break;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
