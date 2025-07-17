#pragma once
#include <QFrame>
#include <QWidget>
#include "base/UniqueResource.hpp"
#include "base/GlobalConfig.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class Element;}
QT_END_NAMESPACE

class Element : public QFrame
{
    Q_OBJECT
private:
    Ui::Element *ui;

public:
    Element(QWidget *parent = nullptr);

    ~Element();

signals:
    void Message(const QString &title, const QString &message);

private:
    void Init();
};