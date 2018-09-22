#ifndef APPCONFIG_H
#define APPCONFIG_H
#include <QSettings>

// Defining config strings
// ...
// Defining meta config strings
#define LAST_OPENED_WINDOW_SIZE "last_opened_window_size"
#define MAIN_SCREEN_LAYOUT "main_screen_layout"

QSettings *config();
QVariant  config_value(QString key);
void      set_config_value(QString key, QVariant value);
bool      config_key_exists(QString key);

QSettings *meta_config();
QVariant  meta_config_value(QString key);
void      set_meta_config_value(QString key, QVariant value);
bool      meta_config_key_exists(QString key);

#endif // APPCONFIG_H
