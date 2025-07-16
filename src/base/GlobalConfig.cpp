#include "GlobalConfig.hpp"


GlobalConfig &GlobalConfig::Config()
{
    static GlobalConfig config;
    return config;
}

QVariant GlobalConfig::Value(QAnyStringView key)
{
    return m_Setting.value(key);
}

QVariant GlobalConfig::Value(QAnyStringView key, const QVariant &defaultValue)
{
    return m_Setting.value(key, defaultValue);
}

void GlobalConfig::SetValue(QAnyStringView key, const QVariant &value)
{
    m_Setting.setValue(key, value);
}

GlobalConfig::GlobalConfig()
    : m_Setting("./config.ini", QSettings::Format::IniFormat)
{
}