#include <QSettings>
#include "appconfig.h"
#include "appinfo.h"

static QSettings m_config(COMPANY, CONFIG);
static QSettings m_meta_config(COMPANY, CONFIG_META);

QSettings *config() {
    return &m_config;
}

QVariant config_value(QString key)
{
    return m_config.value(key);
}

void  set_config_value(QString key, QVariant value)
{
    m_config.setValue(key, value);
}

bool config_value_exists(QString key)
{
    return m_config.contains(key);
}

QSettings *meta_config() {
    return &m_meta_config;
}

QVariant meta_config_value(QString key)
{
    return m_meta_config.value(key);
}

void  set_meta_config_value(QString key, QVariant value)
{
    m_meta_config.setValue(key, value);
}

bool meta_config_key_exists(QString key)
{
    return m_meta_config.contains(key);
}
