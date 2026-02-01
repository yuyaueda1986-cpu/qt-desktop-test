#ifndef SHAPEWIDGET_H
#define SHAPEWIDGET_H

#include "config.h"
#include <QWidget>
#include <QColor>

// Custom widget that renders shape types (Line, Rect, Ellipse, etc.) via QPainter.
// One ShapeWidget is created per shape entry in the widgets array.
class ShapeWidget : public QWidget {
    Q_OBJECT

public:
    explicit ShapeWidget(const WidgetConfig &config, QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawLine(QPainter &painter);
    void drawRect(QPainter &painter);
    void drawEllipse(QPainter &painter);
    void drawTriangle(QPainter &painter);
    void drawDiamond(QPainter &painter);
    void drawArrow(QPainter &painter);
    void drawStar(QPainter &painter);

    // Helper: draw an arrowhead at the given point, pointing in the given direction
    void drawArrowHead(QPainter &painter, const QPointF &tip, const QString &direction, double size);

    static QColor parseColor(const QString &s);

    QString m_type;
    QVariantMap m_props;
};

#endif // SHAPEWIDGET_H
