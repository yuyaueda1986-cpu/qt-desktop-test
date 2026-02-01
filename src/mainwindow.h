#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include "config.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(const AppConfig &config, QWidget *parent = nullptr);

    bool isFullscreen() const { return m_isFullscreen; }
    void setFullscreen(bool fullscreen);

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setupWidgets(const AppConfig &config);
    void showPropertiesDialog();

    QWidget *m_centralWidget;
    bool m_isFullscreen = false;
};

#endif // MAINWINDOW_H
