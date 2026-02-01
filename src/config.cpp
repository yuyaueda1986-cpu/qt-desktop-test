#include "config.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

bool ConfigLoader::load(const QString &filePath, AppConfig &config, QString &errorMessage)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        errorMessage = QString("Cannot open file: %1").arg(filePath);
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        errorMessage = QString("JSON parse error at offset %1: %2")
                           .arg(parseError.offset)
                           .arg(parseError.errorString());
        return false;
    }

    if (!doc.isObject()) {
        errorMessage = "JSON root must be an object";
        return false;
    }

    QJsonObject root = doc.object();

    // Parse window configuration
    if (root.contains("window")) {
        QJsonObject windowObj = root["window"].toObject();
        config.window.title = windowObj["title"].toString("Designed Interface");
        config.window.width = windowObj["width"].toInt(1920);
        config.window.height = windowObj["height"].toInt(1080);
        config.window.backgroundColor = windowObj["background_color"].toString("#2E3440");
    }

    // Parse widgets array
    if (root.contains("widgets")) {
        QJsonArray widgetsArray = root["widgets"].toArray();
        for (const QJsonValue &widgetValue : widgetsArray) {
            if (!widgetValue.isObject()) {
                continue;
            }

            QJsonObject widgetObj = widgetValue.toObject();
            WidgetConfig widgetConfig;

            widgetConfig.id = widgetObj["id"].toString();
            widgetConfig.type = widgetObj["type"].toString();

            // Parse geometry
            if (widgetObj.contains("geometry")) {
                widgetConfig.geometry = parseGeometry(widgetObj["geometry"].toObject().toVariantMap());
            }

            // Parse style
            if (widgetObj.contains("style")) {
                widgetConfig.style = widgetObj["style"].toObject().toVariantMap();
            }

            // Parse props
            if (widgetObj.contains("props")) {
                widgetConfig.props = widgetObj["props"].toObject().toVariantMap();
            }

            // Parse events
            if (widgetObj.contains("events")) {
                widgetConfig.events = widgetObj["events"].toObject().toVariantMap();
            }

            config.widgets.append(widgetConfig);
        }
    }

    return true;
}

QRect ConfigLoader::parseGeometry(const QVariantMap &geoMap)
{
    int x = geoMap.value("x", 0).toInt();
    int y = geoMap.value("y", 0).toInt();
    int width = geoMap.value("width", 100).toInt();
    int height = geoMap.value("height", 30).toInt();
    return QRect(x, y, width, height);
}
