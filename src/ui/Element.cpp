#include "Element.hpp"
#include "ui_Element.h"


Element::Element(QWidget *parent)
    : QFrame(parent), ui(new Ui::Element)
{
    ui->setupUi(this);
    Init();
}

Element::~Element()
{
    delete ui;
}

void Element::Init()
{
}