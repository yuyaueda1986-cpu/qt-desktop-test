#ifndef WIDGETFACTORY_H
#define WIDGETFACTORY_H

#include "config.h"
#include <QWidget>

class WidgetFactory {
public:
    // Create a widget from configuration. Returns nullptr for shape types.
    static QWidget* create(const WidgetConfig &config, QWidget *parent);

    // Check if a type is a shape (rendered via QPainter, not a native widget)
    static bool isShapeType(const QString &type);

private:
    // Widget creators for each type
    static QWidget* createButton(const WidgetConfig &config, QWidget *parent);
    static QWidget* createLabel(const WidgetConfig &config, QWidget *parent);
    static QWidget* createEntry(const WidgetConfig &config, QWidget *parent);
    static QWidget* createCheckbox(const WidgetConfig &config, QWidget *parent);
    static QWidget* createSwitch(const WidgetConfig &config, QWidget *parent);
    static QWidget* createCombo(const WidgetConfig &config, QWidget *parent);
    static QWidget* createSlider(const WidgetConfig &config, QWidget *parent);
    static QWidget* createSpin(const WidgetConfig &config, QWidget *parent);
    static QWidget* createImage(const WidgetConfig &config, QWidget *parent);
    static QWidget* createProgress(const WidgetConfig &config, QWidget *parent);
    static QWidget* createSeparator(const WidgetConfig &config, QWidget *parent);

    // Helper to apply style (background_color, color) via stylesheet
    static void applyStyle(QWidget *widget, const QString &qtClassName, const QVariantMap &style);
};

#endif // WIDGETFACTORY_H
