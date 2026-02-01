#include "mainwindow.h"
#include "widgetfactory.h"
#include "shapewidget.h"
#include <QKeyEvent>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>

// Properties Dialog class
class PropertiesDialog : public QDialog {
    Q_OBJECT

public:
    explicit PropertiesDialog(MainWindow *mainWindow, QWidget *parent = nullptr)
        : QDialog(parent)
        , m_mainWindow(mainWindow)
    {
        setWindowTitle("Properties");
        setModal(true);

        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->setContentsMargins(15, 15, 15, 15);
        mainLayout->setSpacing(10);

        m_radioWindow = new QRadioButton("Window Mode", this);
        m_radioFullscreen = new QRadioButton("Fullscreen Mode", this);

        if (m_mainWindow->isFullscreen()) {
            m_radioFullscreen->setChecked(true);
        } else {
            m_radioWindow->setChecked(true);
        }

        mainLayout->addWidget(m_radioWindow);
        mainLayout->addWidget(m_radioFullscreen);

        QHBoxLayout *buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();

        QPushButton *cancelButton = new QPushButton("Cancel", this);
        QPushButton *okButton = new QPushButton("OK", this);
        okButton->setDefault(true);

        buttonLayout->addWidget(cancelButton);
        buttonLayout->addWidget(okButton);
        mainLayout->addLayout(buttonLayout);

        connect(okButton, &QPushButton::clicked, this, &PropertiesDialog::onOkClicked);
        connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

        adjustSize();
        setFixedSize(size());
    }

private slots:
    void onOkClicked()
    {
        m_mainWindow->setFullscreen(m_radioFullscreen->isChecked());
        accept();
    }

private:
    MainWindow *m_mainWindow;
    QRadioButton *m_radioWindow;
    QRadioButton *m_radioFullscreen;
};

// MainWindow implementation
MainWindow::MainWindow(const AppConfig &config, QWidget *parent)
    : QMainWindow(parent)
{
    // Set window title from config
    QString title = config.window.title;
    if (title.isEmpty()) {
        title = "Designed Interface";
    }
    setWindowTitle(title);

    // Resize to JSON-specified window dimensions
    resize(config.window.width, config.window.height);

    // Create central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);

    // Apply background color via QPalette (spec recommendation)
    if (!config.window.backgroundColor.isEmpty()) {
        QPalette pal = m_centralWidget->palette();
        QColor bgColor(config.window.backgroundColor);
        pal.setColor(QPalette::Window, bgColor);
        m_centralWidget->setAutoFillBackground(true);
        m_centralWidget->setPalette(pal);
    }

    // Setup widgets from config
    setupWidgets(config);
}

void MainWindow::setupWidgets(const AppConfig &config)
{
    // Iterate in array order; array order = Z-order (earlier = further back).
    // Qt child widgets are painted in creation order, so we just create them in order.
    for (const WidgetConfig &widgetConfig : config.widgets) {
        QWidget *widget = nullptr;

        if (WidgetFactory::isShapeType(widgetConfig.type)) {
            // Shape types: custom QPainter rendering
            widget = new ShapeWidget(widgetConfig, m_centralWidget);
        } else {
            // Native widget types
            widget = WidgetFactory::create(widgetConfig, m_centralWidget);
        }

        if (widget) {
            widget->show();
        }
    }
}

void MainWindow::setFullscreen(bool fullscreen)
{
    if (fullscreen && !m_isFullscreen) {
        showFullScreen();
        m_isFullscreen = true;
    } else if (!fullscreen && m_isFullscreen) {
        showNormal();
        m_isFullscreen = false;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F12) {
        if (event->modifiers() & Qt::ControlModifier) {
            setFullscreen(!m_isFullscreen);
        } else {
            showPropertiesDialog();
        }
        event->accept();
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::showPropertiesDialog()
{
    PropertiesDialog dialog(this, this);
    dialog.exec();
}

#include "mainwindow.moc"
