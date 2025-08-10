#include <map>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QSslConfiguration>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include "TimeAndWeather.hpp"
#include "ui_TimeAndWeather.h"


TimeAndWeather::TimeAndWeather(QWidget *parent)
    : Element(parent), ui(new Ui::TimeAndWeather)
{
    ui->setupUi(this);
    Init();
}

TimeAndWeather::~TimeAndWeather()
{
    delete ui;
}

double TimeAndWeather::K2C(const double value)
{
    return value - 273.15;
}

NetworkReplyCoro TimeAndWeather::UpdateWeather()
{
    QUrlQuery query;
    query.addQueryItem("lat", GlobalConfig::Config().Value("TimeAndWeather/Latitude").toString());
    query.addQueryItem("lon", GlobalConfig::Config().Value("TimeAndWeather/Longituden").toString());
    query.addQueryItem("lang", GlobalConfig::Config().Value("TimeAndWeather/Lang").toString());
    query.addQueryItem("appid", GlobalConfig::Config().Value("TimeAndWeather/WeatherAPIKey").toString());
    QUrl url(GlobalConfig::Config().Value("TimeAndWeather/WeatherAPI").toString());
    url.setQuery(query);
    QNetworkRequest request;
    request.setUrl(url);
    request.setTransferTimeout();
    QNetworkReply *reply = co_await UniqueResource::Resource().NetworkManager.get(request);
    if (reply->error() == QNetworkReply::NetworkError::NoError)
    {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();
        QJsonObject main = root.value("main").toObject();
        QJsonObject weather = root.value("weather").toArray().first().toObject();
        QJsonObject wind = root.value("wind").toObject();
        QJsonObject clouds = root.value("clouds").toObject();

        QString weatherIcon = weather.value("icon").toString();
        QString mainInfo = weather.value("description").toString() + ' '
            + QString::number(K2C(main.value("temp").toDouble())) + "℃";
        QString humidityInfo = tr("Humidity: ") + QString::number(main.value("humidity").toDouble()) + '%';
        QString feelsLikeInfo = tr("Feels like: ") + QString::number(K2C(main.value("feels_like").toDouble())) + "℃";
        QString windInfo = tr("Wind: ") + QString::number(wind.value("deg").toDouble())
            + "° " + QString::number(wind.value("speed").toDouble()) + "m/s";
        QString pressureInfo = tr("Pressure: ") + QString::number(main.value("grnd_level").toDouble()) + "hPa";
        QString visibilityInfo = tr("Visibility: ") + QString::number(root.value("visibility").toDouble()) + 'm';
        QString cloudsInfo = tr("Clouds: ") + QString::number(clouds.value("all").toDouble()) + '%';

        ui->lb_WeatherIcon->setPixmap(QPixmap(":/icons/weatherIcons/" + weatherIcon + ".png"));
        ui->lb_CurrentWeather->setText(mainInfo);
        ui->lb_Humidity->setText(humidityInfo);
        ui->lb_FeelsLike->setText(feelsLikeInfo);
        ui->lb_Wind->setText(windInfo);
        ui->lb_Pressure->setText(pressureInfo);
        ui->lb_Visibility->setText(visibilityInfo);
        ui->lb_Clouds->setText(cloudsInfo);
    }
    else
    {
        qDebug() << reply->errorString();
        ui->lb_CurrentWeather->clear();
        ui->lb_WeatherIcon->clear();
        ui->lb_Humidity->clear();
        ui->lb_FeelsLike->clear();
        ui->lb_Wind->clear();
        ui->lb_Pressure->clear();
        ui->lb_Visibility->clear();
        ui->lb_Clouds->clear();
    }
    reply->deleteLater();
}

NetworkReplyCoro TimeAndWeather::UpdateForecast()
{
    QUrlQuery query;
    query.addQueryItem("lat", GlobalConfig::Config().Value("TimeAndWeather/Latitude").toString());
    query.addQueryItem("lon", GlobalConfig::Config().Value("TimeAndWeather/Longituden").toString());
    query.addQueryItem("lang", GlobalConfig::Config().Value("TimeAndWeather/Lang").toString());
    query.addQueryItem("cnt", "5");
    query.addQueryItem("appid", GlobalConfig::Config().Value("TimeAndWeather/WeatherAPIKey").toString());
    QUrl url(GlobalConfig::Config().Value("TimeAndWeather/ForecastAPI").toString());
    url.setQuery(query);
    QNetworkRequest request;
    request.setUrl(url);
    request.setTransferTimeout();
    QNetworkReply *reply = co_await UniqueResource::Resource().NetworkManager.get(request);
    if (reply->error() == QNetworkReply::NetworkError::NoError)
    {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();
        QJsonArray list = root.value("list").toArray();
        
        ui->lw_Forecast->clear();
        QDate day = QDate::currentDate();
        const std::map<QString, QString> dayNames = {{"Mon", tr("Monday")},
            {"Tue", tr("Tuesday")}, {"Wed", tr("Wednesday")}, {"Thu", tr("Thursday")},
            {"Fri", tr("Friday")}, {"Sat", tr("Saturday")}, {"Sun", tr("Sunday")}};
        for (int i = 0; i < 5; ++i)
        {
            QJsonObject item = list.at(i).toObject();
            QJsonObject main = item.value("main").toObject();
            QJsonObject weather = item.value("weather").toArray().first().toObject();

            QString forcecast = weather.value("description").toString() + ' '
                + QString::number(K2C(main.value("temp_min").toDouble())) + "℃~"
                + QString::number(K2C(main.value("temp_max").toDouble())) + "℃ "
                + (i == 0 ? tr("Today") : (i == 1 ? tr("Tomorrow") : dayNames.at(day.toString("ddd"))));
            QString icon = weather.value("icon").toString();
            QListWidgetItem *listWidgetItem = new QListWidgetItem(QIcon(":/icons/weatherIcons/"
                + icon + ".png"), forcecast, ui->lw_Forecast);
            listWidgetItem->setSizeHint(QSize(-1, 30));
            ui->lw_Forecast->addItem(listWidgetItem);
            day = day.addDays(1);
        }
    }
    else
    {
        qDebug() << reply->errorString();
    }
    reply->deleteLater();
}

void TimeAndWeather::Init()
{
    connect(&UniqueResource::Resource().Timer1, &QTimer::timeout, this, &TimeAndWeather::UpdateTime);
    connect(&UniqueResource::Resource().Timer600, &QTimer::timeout, this, &TimeAndWeather::UpdateWeather);
    connect(&UniqueResource::Resource().Timer3600, &QTimer::timeout, this, &TimeAndWeather::UpdateForecast);
}

void TimeAndWeather::UpdateTime()
{
    ui->lb_Time->setText(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
}
