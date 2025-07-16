#pragma once
#include <QNetworkReply>
#include "ui/Element.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class TimeAndWeather;}
QT_END_NAMESPACE

class TimeAndWeather : public Element
{
    Q_OBJECT
private:
    Ui::TimeAndWeather *ui;

public:
    TimeAndWeather(QWidget *parent = nullptr);

    ~TimeAndWeather();

    static double K2C(const double value);

private:
    void Init();

    void UpdateTime();

    void UpdateWeather();

    void UpdateWeatherInfo(QNetworkReply *reply);

    void UpdateForecast();

    void UpdateForecastInfo(QNetworkReply *reply);
};