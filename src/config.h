#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QRect>
#include <QVariantMap>
#include <QList>

// Widget configuration from JSON
struct WidgetConfig {
    QString id;
    QString type;
    QRect geometry;           // x, y, width, height
    QVariantMap style;        // background_color, color
    QVariantMap props;        // Widget-specific properties
    QVariantMap events;       // Signal mappings (e.g. "clicked" -> "on_save")
};

// Window configuration from JSON
struct WindowConfig {
    QString title;
    int width = 1920;
    int height = 1080;
    QString backgroundColor;
};

// Complete application configuration
struct AppConfig {
    WindowConfig window;
    QList<WidgetConfig> widgets;
};

// Configuration loader class
class ConfigLoader {
public:
    static bool load(const QString &filePath, AppConfig &config, QString &errorMessage);

private:
    static QRect parseGeometry(const QVariantMap &geoMap);
};

#endif // CONFIG_H
