#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QPushButton>
#include <QKeyEvent>
#include <QCommandLineParser>
#include <QTimer>

/*
 * Known Limitations (WSL2/WSLg):
 *
 * Multi-monitor fullscreen issues:
 * - When running under WSLg (WSL2 GUI subsystem), the --fullscreen option
 *   may not work correctly on non-primary monitors.
 * - Symptoms include: F12 key not responding, dialogs appearing behind
 *   the fullscreen window, or focus issues.
 * - This is a limitation of WSLg's Wayland/X11 bridge, not this application.
 * - Workaround: Use fullscreen on the primary monitor only.
 */

// Configuration from command-line options
struct Config {
    int width = 800;
    int height = 600;
    bool startFullscreen = false;
};

static Config config;

// Forward declaration
class MainWindow;

// Properties Dialog
class PropertiesDialog : public QDialog {
    Q_OBJECT
public:
    explicit PropertiesDialog(MainWindow *mainWindow, QWidget *parent = nullptr);

private slots:
    void onOkClicked();

private:
    MainWindow *m_mainWindow;
    QRadioButton *m_radioWindow;
    QRadioButton *m_radioFullscreen;
};

// Main Window
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    bool isFullscreen() const { return m_isFullscreen; }
    void setFullscreen(bool fullscreen);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void showPropertiesDialog();

    bool m_isFullscreen = false;
};

// MainWindow implementation
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Qt Desktop Test");
    resize(config.width, config.height);

    // Create label content
    QLabel *label = new QLabel("F12: Properties dialog\nCtrl+F12: Toggle fullscreen", this);
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);

    m_isFullscreen = config.startFullscreen;

    // Apply fullscreen after window is fully realized (delayed)
    if (config.startFullscreen) {
        QTimer::singleShot(0, this, [this]() {
            showFullScreen();
            activateWindow();
        });
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
            // Ctrl+F12: Toggle fullscreen directly
            setFullscreen(!m_isFullscreen);
        } else {
            // F12: Show properties dialog
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

// PropertiesDialog implementation
PropertiesDialog::PropertiesDialog(MainWindow *mainWindow, QWidget *parent)
    : QDialog(parent)
    , m_mainWindow(mainWindow)
{
    setWindowTitle("Properties");
    setModal(true);
    setFixedSize(sizeHint());

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    // Radio buttons for mode selection
    m_radioWindow = new QRadioButton("Window Mode", this);
    m_radioFullscreen = new QRadioButton("Fullscreen Mode", this);

    // Reflect current state
    if (m_mainWindow->isFullscreen()) {
        m_radioFullscreen->setChecked(true);
    } else {
        m_radioWindow->setChecked(true);
    }

    mainLayout->addWidget(m_radioWindow);
    mainLayout->addWidget(m_radioFullscreen);

    // Button layout (horizontal, right-aligned)
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    QPushButton *okButton = new QPushButton("OK", this);
    okButton->setDefault(true);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);
    mainLayout->addLayout(buttonLayout);

    // Connect button handlers
    connect(okButton, &QPushButton::clicked, this, &PropertiesDialog::onOkClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    adjustSize();
    setFixedSize(size());
}

void PropertiesDialog::onOkClicked()
{
    m_mainWindow->setFullscreen(m_radioFullscreen->isChecked());
    accept();
}

// Print usage
void printUsage(const QString &progName)
{
    qInfo("Usage: %s [OPTIONS]\n", qPrintable(progName));
    qInfo("Options:");
    qInfo("  --geometry WxH   Set window size (e.g. --geometry 1024x768)");
    qInfo("  --fullscreen     Start in fullscreen mode");
    qInfo("  --help           Show this help message");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Qt Desktop Test");

    // Parse command-line arguments
    QStringList args = app.arguments();
    for (int i = 1; i < args.size(); ++i) {
        if (args[i] == "--geometry" && i + 1 < args.size()) {
            // Parse "WxH" format
            QString geometry = args[i + 1];
            QStringList parts = geometry.split('x', Qt::SkipEmptyParts);
            if (parts.size() == 2) {
                bool okW, okH;
                int w = parts[0].toInt(&okW);
                int h = parts[1].toInt(&okH);
                if (okW && okH && w > 0 && h > 0) {
                    config.width = w;
                    config.height = h;
                } else {
                    qWarning("Invalid geometry format: %s (use WxH, e.g. 1024x768)",
                             qPrintable(geometry));
                    return 1;
                }
            } else {
                qWarning("Invalid geometry format: %s (use WxH, e.g. 1024x768)",
                         qPrintable(geometry));
                return 1;
            }
            ++i; // Skip the value argument
        } else if (args[i] == "--fullscreen") {
            config.startFullscreen = true;
        } else if (args[i] == "--help" || args[i] == "-h") {
            printUsage(args[0]);
            return 0;
        }
    }

    MainWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
