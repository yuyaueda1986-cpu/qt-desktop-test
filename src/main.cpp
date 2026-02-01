#include <QApplication>
#include <QTimer>
#include "config.h"
#include "mainwindow.h"

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

// Runtime configuration from command-line
struct RuntimeConfig {
    QString layoutPath;
    int width = 0;   // 0 means use JSON window dimensions
    int height = 0;
    bool startFullscreen = false;
};

void printUsage(const QString &progName)
{
    qInfo("Usage: %s [OPTIONS]\n", qPrintable(progName));
    qInfo("Options:");
    qInfo("  --layout FILE    JSON layout file path (required)");
    qInfo("  --geometry WxH   Set window size (e.g. --geometry 1024x768)");
    qInfo("  --fullscreen     Start in fullscreen mode");
    qInfo("  --help           Show this help message");
}

bool parseArguments(const QStringList &args, RuntimeConfig &config)
{
    for (int i = 1; i < args.size(); ++i) {
        if (args[i] == "--layout" && i + 1 < args.size()) {
            config.layoutPath = args[i + 1];
            ++i;
        } else if (args[i] == "--geometry" && i + 1 < args.size()) {
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
                    return false;
                }
            } else {
                qWarning("Invalid geometry format: %s (use WxH, e.g. 1024x768)",
                         qPrintable(geometry));
                return false;
            }
            ++i;
        } else if (args[i] == "--fullscreen") {
            config.startFullscreen = true;
        } else if (args[i] == "--help" || args[i] == "-h") {
            printUsage(args[0]);
            return false;
        } else {
            qWarning("Unknown option: %s", qPrintable(args[i]));
            printUsage(args[0]);
            return false;
        }
    }

    if (config.layoutPath.isEmpty()) {
        qWarning("Error: --layout option is required");
        printUsage(args[0]);
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Qt Dashboard");

    // Parse command-line arguments
    RuntimeConfig runtimeConfig;
    if (!parseArguments(app.arguments(), runtimeConfig)) {
        return 1;
    }

    // Load JSON configuration
    AppConfig appConfig;
    QString errorMessage;
    if (!ConfigLoader::load(runtimeConfig.layoutPath, appConfig, errorMessage)) {
        qCritical("Failed to load layout: %s", qPrintable(errorMessage));
        return 1;
    }

    // Create main window (uses JSON window.width/height by default)
    MainWindow window(appConfig);

    // Override with command-line geometry if specified
    if (runtimeConfig.width > 0 && runtimeConfig.height > 0) {
        window.resize(runtimeConfig.width, runtimeConfig.height);
    }

    // Apply fullscreen if requested
    if (runtimeConfig.startFullscreen) {
        QTimer::singleShot(0, &window, [&window]() {
            window.setFullscreen(true);
            window.activateWindow();
        });
    }

    window.show();

    return app.exec();
}
