#pragma once
#include <QSettings>


class GlobalConfig
{
private:
    QSettings m_Setting;

public:
    static GlobalConfig &Config();

    QVariant Value(QAnyStringView key);

    QVariant Value(QAnyStringView key, const QVariant &defaultValue);

    void SetValue(QAnyStringView key, const QVariant &value);

private:
    GlobalConfig();

    ~GlobalConfig() = default;

    GlobalConfig(const GlobalConfig &) = delete;

    const GlobalConfig &operator=(const GlobalConfig &) = delete;
};