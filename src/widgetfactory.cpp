#include "widgetfactory.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSlider>
#include <QSpinBox>
#include <QProgressBar>
#include <QFrame>
#include <QPixmap>
#include <QIcon>
#include <QFont>

static const QStringList shapeTypes = {
    "Line", "Rect", "Ellipse", "Triangle", "Diamond", "Arrow", "Star"
};

bool WidgetFactory::isShapeType(const QString &type)
{
    return shapeTypes.contains(type);
}

QWidget* WidgetFactory::create(const WidgetConfig &config, QWidget *parent)
{
    // Shape types are not native widgets; they are drawn by ShapeWidget
    if (isShapeType(config.type)) {
        return nullptr;
    }

    QWidget *widget = nullptr;
    const QString &type = config.type;

    if (type == "Button") {
        widget = createButton(config, parent);
    } else if (type == "Label") {
        widget = createLabel(config, parent);
    } else if (type == "Entry") {
        widget = createEntry(config, parent);
    } else if (type == "Checkbox") {
        widget = createCheckbox(config, parent);
    } else if (type == "Switch") {
        widget = createSwitch(config, parent);
    } else if (type == "Combo") {
        widget = createCombo(config, parent);
    } else if (type == "Slider") {
        widget = createSlider(config, parent);
    } else if (type == "Spin") {
        widget = createSpin(config, parent);
    } else if (type == "Image") {
        widget = createImage(config, parent);
    } else if (type == "Progress") {
        widget = createProgress(config, parent);
    } else if (type == "Separator") {
        widget = createSeparator(config, parent);
    } else {
        qWarning("Unknown widget type: %s", qPrintable(type));
        return nullptr;
    }

    if (widget) {
        widget->setObjectName(config.id);
        widget->setGeometry(config.geometry);
    }

    return widget;
}

QWidget* WidgetFactory::createButton(const WidgetConfig &config, QWidget *parent)
{
    QPushButton *button = new QPushButton(parent);

    QString label = config.props.value("label").toString();
    if (!label.isEmpty()) {
        button->setText(label);
    }

    QString iconName = config.props.value("icon_name").toString();
    if (!iconName.isEmpty()) {
        button->setIcon(QIcon::fromTheme(iconName));
    }

    applyStyle(button, "QPushButton", config.style);
    return button;
}

QWidget* WidgetFactory::createLabel(const WidgetConfig &config, QWidget *parent)
{
    QLabel *label = new QLabel(parent);

    QString text = config.props.value("label").toString();
    label->setText(text);

    // font_size is in props per spec
    int fontSize = config.props.value("font_size", 0).toInt();
    if (fontSize > 0) {
        QFont font = label->font();
        font.setPixelSize(fontSize);
        label->setFont(font);
    }

    applyStyle(label, "QLabel", config.style);
    return label;
}

QWidget* WidgetFactory::createEntry(const WidgetConfig &config, QWidget *parent)
{
    QLineEdit *lineEdit = new QLineEdit(parent);

    QString text = config.props.value("text").toString();
    if (!text.isEmpty()) {
        lineEdit->setText(text);
    }

    QString placeholder = config.props.value("placeholder").toString();
    if (!placeholder.isEmpty()) {
        lineEdit->setPlaceholderText(placeholder);
    }

    applyStyle(lineEdit, "QLineEdit", config.style);
    return lineEdit;
}

QWidget* WidgetFactory::createCheckbox(const WidgetConfig &config, QWidget *parent)
{
    QCheckBox *checkBox = new QCheckBox(parent);

    QString label = config.props.value("label").toString();
    if (!label.isEmpty()) {
        checkBox->setText(label);
    }

    // Spec uses "checked" for Checkbox
    bool checked = config.props.value("checked", false).toBool();
    checkBox->setChecked(checked);

    applyStyle(checkBox, "QCheckBox", config.style);
    return checkBox;
}

QWidget* WidgetFactory::createSwitch(const WidgetConfig &config, QWidget *parent)
{
    // Qt6 doesn't have a native switch; use styled QCheckBox
    QCheckBox *switchWidget = new QCheckBox(parent);

    QString label = config.props.value("label").toString();
    if (!label.isEmpty()) {
        switchWidget->setText(label);
    }

    bool active = config.props.value("active", false).toBool();
    switchWidget->setChecked(active);

    // Apply base style then switch-like indicator styling
    applyStyle(switchWidget, "QCheckBox", config.style);
    QString existing = switchWidget->styleSheet();
    QString switchStyle = QStringLiteral(
        "QCheckBox::indicator { width: 40px; height: 20px; border-radius: 10px; background-color: #555; }"
        "QCheckBox::indicator:checked { background-color: #4CAF50; }"
    );
    switchWidget->setStyleSheet(existing + switchStyle);

    return switchWidget;
}

QWidget* WidgetFactory::createCombo(const WidgetConfig &config, QWidget *parent)
{
    QComboBox *comboBox = new QComboBox(parent);

    // Spec: items is a comma-separated string
    QString itemsStr = config.props.value("items").toString();
    if (!itemsStr.isEmpty()) {
        QStringList items = itemsStr.split(',');
        for (const QString &item : items) {
            comboBox->addItem(item.trimmed());
        }
    }

    int activeIndex = config.props.value("active_index", 0).toInt();
    if (activeIndex >= 0 && activeIndex < comboBox->count()) {
        comboBox->setCurrentIndex(activeIndex);
    }

    applyStyle(comboBox, "QComboBox", config.style);
    return comboBox;
}

QWidget* WidgetFactory::createSlider(const WidgetConfig &config, QWidget *parent)
{
    QSlider *slider = new QSlider(Qt::Horizontal, parent);

    int min = config.props.value("min", 0).toInt();
    int max = config.props.value("max", 100).toInt();
    int value = config.props.value("value", 0).toInt();
    int step = config.props.value("step", 1).toInt();

    slider->setMinimum(min);
    slider->setMaximum(max);
    slider->setValue(value);
    slider->setSingleStep(step);

    applyStyle(slider, "QSlider", config.style);
    return slider;
}

QWidget* WidgetFactory::createSpin(const WidgetConfig &config, QWidget *parent)
{
    // Spec says QSpinBox (integer)
    QSpinBox *spinBox = new QSpinBox(parent);

    int min = config.props.value("min", 0).toInt();
    int max = config.props.value("max", 100).toInt();
    int step = config.props.value("step", 1).toInt();
    int value = config.props.value("value", 0).toInt();

    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setSingleStep(step);
    spinBox->setValue(value);

    applyStyle(spinBox, "QSpinBox", config.style);
    return spinBox;
}

QWidget* WidgetFactory::createImage(const WidgetConfig &config, QWidget *parent)
{
    QLabel *imageLabel = new QLabel(parent);
    imageLabel->setScaledContents(true);

    QString filePath = config.props.value("file_path").toString();
    QString altText = config.props.value("alt_text", "Image").toString();

    if (!filePath.isEmpty()) {
        QPixmap pixmap(filePath);
        if (!pixmap.isNull()) {
            imageLabel->setPixmap(pixmap.scaled(config.geometry.width(),
                                                 config.geometry.height(),
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));
        } else {
            // File not found or unreadable; show alt_text
            imageLabel->setText(altText);
            imageLabel->setAlignment(Qt::AlignCenter);
        }
    } else {
        // No file_path; show alt_text as fallback
        imageLabel->setText(altText);
        imageLabel->setAlignment(Qt::AlignCenter);
    }

    applyStyle(imageLabel, "QLabel", config.style);
    return imageLabel;
}

QWidget* WidgetFactory::createProgress(const WidgetConfig &config, QWidget *parent)
{
    QProgressBar *progressBar = new QProgressBar(parent);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);

    // Spec: value is 0.0-1.0, convert to 0-100 for Qt
    double value = config.props.value("value", 0.0).toDouble();
    progressBar->setValue(static_cast<int>(value * 100));

    bool showText = config.props.value("show_text", false).toBool();
    progressBar->setTextVisible(showText);

    applyStyle(progressBar, "QProgressBar", config.style);
    return progressBar;
}

QWidget* WidgetFactory::createSeparator(const WidgetConfig &config, QWidget *parent)
{
    QFrame *separator = new QFrame(parent);

    QString orientation = config.props.value("orientation", "horizontal").toString();
    if (orientation == "vertical") {
        separator->setFrameShape(QFrame::VLine);
    } else {
        separator->setFrameShape(QFrame::HLine);
    }
    separator->setFrameShadow(QFrame::Sunken);

    applyStyle(separator, "QFrame", config.style);
    return separator;
}

void WidgetFactory::applyStyle(QWidget *widget, const QString &qtClassName, const QVariantMap &style)
{
    if (style.isEmpty()) {
        return;
    }

    QStringList rules;

    // Spec: style only contains background_color and color
    QString bgColor = style.value("background_color").toString();
    if (!bgColor.isEmpty() && bgColor != "transparent") {
        rules << QStringLiteral("background-color: %1").arg(bgColor);
    } else if (bgColor == "transparent") {
        rules << QStringLiteral("background-color: transparent");
    }

    QString color = style.value("color").toString();
    if (!color.isEmpty() && color != "transparent") {
        rules << QStringLiteral("color: %1").arg(color);
    } else if (color == "transparent") {
        rules << QStringLiteral("color: transparent");
    }

    if (!rules.isEmpty()) {
        widget->setStyleSheet(QStringLiteral("%1 { %2; }").arg(qtClassName, rules.join("; ")));
    }
}
