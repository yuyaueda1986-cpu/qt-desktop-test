#include "shapewidget.h"
#include <QPainter>
#include <QPen>
#include <QPolygonF>
#include <QtMath>

ShapeWidget::ShapeWidget(const WidgetConfig &config, QWidget *parent)
    : QWidget(parent)
    , m_type(config.type)
    , m_props(config.props)
{
    setObjectName(config.id);
    setGeometry(config.geometry);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);
}

QColor ShapeWidget::parseColor(const QString &s)
{
    if (s == "transparent" || s.isEmpty()) {
        return Qt::transparent;
    }
    return QColor(s);
}

void ShapeWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_type == "Line") {
        drawLine(painter);
    } else if (m_type == "Rect") {
        drawRect(painter);
    } else if (m_type == "Ellipse") {
        drawEllipse(painter);
    } else if (m_type == "Triangle") {
        drawTriangle(painter);
    } else if (m_type == "Diamond") {
        drawDiamond(painter);
    } else if (m_type == "Arrow") {
        drawArrow(painter);
    } else if (m_type == "Star") {
        drawStar(painter);
    }
}

void ShapeWidget::drawLine(QPainter &painter)
{
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();
    QString direction = m_props.value("direction", "horizontal").toString();

    QPen pen(strokeColor, strokeWidth);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    double w = width();
    double h = height();
    QPointF p1, p2;

    if (direction == "horizontal") {
        p1 = QPointF(0, h / 2.0);
        p2 = QPointF(w, h / 2.0);
    } else if (direction == "vertical") {
        p1 = QPointF(w / 2.0, 0);
        p2 = QPointF(w / 2.0, h);
    } else if (direction == "diagonal-se") {
        p1 = QPointF(0, 0);
        p2 = QPointF(w, h);
    } else if (direction == "diagonal-ne") {
        p1 = QPointF(0, h);
        p2 = QPointF(w, 0);
    }

    painter.drawLine(p1, p2);
}

void ShapeWidget::drawRect(QPainter &painter)
{
    QColor fillColor = parseColor(m_props.value("fill_color", "transparent").toString());
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();
    double borderRadius = m_props.value("border_radius", 0).toDouble();

    QPen pen(strokeColor, strokeWidth);
    if (strokeWidth <= 0) {
        pen = QPen(Qt::NoPen);
    }
    painter.setPen(pen);

    if (fillColor == Qt::transparent) {
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(fillColor);
    }

    // Inset by half stroke width so the stroke stays inside the widget bounds
    double offset = strokeWidth / 2.0;
    QRectF rect(offset, offset, width() - strokeWidth, height() - strokeWidth);

    if (borderRadius > 0) {
        painter.drawRoundedRect(rect, borderRadius, borderRadius);
    } else {
        painter.drawRect(rect);
    }
}

void ShapeWidget::drawEllipse(QPainter &painter)
{
    QColor fillColor = parseColor(m_props.value("fill_color", "transparent").toString());
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();

    QPen pen(strokeColor, strokeWidth);
    if (strokeWidth <= 0) {
        pen = QPen(Qt::NoPen);
    }
    painter.setPen(pen);

    if (fillColor == Qt::transparent) {
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(fillColor);
    }

    double offset = strokeWidth / 2.0;
    QRectF rect(offset, offset, width() - strokeWidth, height() - strokeWidth);
    painter.drawEllipse(rect);
}

void ShapeWidget::drawTriangle(QPainter &painter)
{
    QColor fillColor = parseColor(m_props.value("fill_color", "transparent").toString());
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();
    QString direction = m_props.value("direction", "up").toString();

    QPen pen(strokeColor, strokeWidth);
    if (strokeWidth <= 0) {
        pen = QPen(Qt::NoPen);
    }
    painter.setPen(pen);

    if (fillColor == Qt::transparent) {
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(fillColor);
    }

    double w = width();
    double h = height();
    QPolygonF polygon;

    if (direction == "up") {
        polygon << QPointF(w / 2.0, 0) << QPointF(w, h) << QPointF(0, h);
    } else if (direction == "down") {
        polygon << QPointF(0, 0) << QPointF(w, 0) << QPointF(w / 2.0, h);
    } else if (direction == "left") {
        polygon << QPointF(w, 0) << QPointF(w, h) << QPointF(0, h / 2.0);
    } else if (direction == "right") {
        polygon << QPointF(0, 0) << QPointF(w, h / 2.0) << QPointF(0, h);
    }

    painter.drawPolygon(polygon);
}

void ShapeWidget::drawDiamond(QPainter &painter)
{
    QColor fillColor = parseColor(m_props.value("fill_color", "transparent").toString());
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();

    QPen pen(strokeColor, strokeWidth);
    if (strokeWidth <= 0) {
        pen = QPen(Qt::NoPen);
    }
    painter.setPen(pen);

    if (fillColor == Qt::transparent) {
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(fillColor);
    }

    double w = width();
    double h = height();

    QPolygonF polygon;
    polygon << QPointF(w / 2.0, 0)
            << QPointF(w, h / 2.0)
            << QPointF(w / 2.0, h)
            << QPointF(0, h / 2.0);

    painter.drawPolygon(polygon);
}

void ShapeWidget::drawArrow(QPainter &painter)
{
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();
    QString direction = m_props.value("direction", "right").toString();

    QPen pen(strokeColor, strokeWidth);
    painter.setPen(pen);
    painter.setBrush(strokeColor);

    double w = width();
    double h = height();
    QPointF start, end;

    if (direction == "right") {
        start = QPointF(0, h / 2.0);
        end   = QPointF(w, h / 2.0);
    } else if (direction == "left") {
        start = QPointF(w, h / 2.0);
        end   = QPointF(0, h / 2.0);
    } else if (direction == "up") {
        start = QPointF(w / 2.0, h);
        end   = QPointF(w / 2.0, 0);
    } else if (direction == "down") {
        start = QPointF(w / 2.0, 0);
        end   = QPointF(w / 2.0, h);
    }

    painter.drawLine(start, end);
    drawArrowHead(painter, end, direction, qMax(8.0, strokeWidth * 4));
}

void ShapeWidget::drawArrowHead(QPainter &painter, const QPointF &tip, const QString &direction, double size)
{
    QPolygonF head;

    if (direction == "right") {
        head << tip
             << QPointF(tip.x() - size, tip.y() - size / 2.0)
             << QPointF(tip.x() - size, tip.y() + size / 2.0);
    } else if (direction == "left") {
        head << tip
             << QPointF(tip.x() + size, tip.y() - size / 2.0)
             << QPointF(tip.x() + size, tip.y() + size / 2.0);
    } else if (direction == "up") {
        head << tip
             << QPointF(tip.x() - size / 2.0, tip.y() + size)
             << QPointF(tip.x() + size / 2.0, tip.y() + size);
    } else if (direction == "down") {
        head << tip
             << QPointF(tip.x() - size / 2.0, tip.y() - size)
             << QPointF(tip.x() + size / 2.0, tip.y() - size);
    }

    painter.setPen(Qt::NoPen);
    painter.drawPolygon(head);
}

void ShapeWidget::drawStar(QPainter &painter)
{
    QColor fillColor = parseColor(m_props.value("fill_color", "transparent").toString());
    QColor strokeColor = parseColor(m_props.value("stroke_color", "#ECEFF4").toString());
    double strokeWidth = m_props.value("stroke_width", 2).toDouble();
    int points = m_props.value("points", 5).toInt();

    QPen pen(strokeColor, strokeWidth);
    if (strokeWidth <= 0) {
        pen = QPen(Qt::NoPen);
    }
    painter.setPen(pen);

    if (fillColor == Qt::transparent) {
        painter.setBrush(Qt::NoBrush);
    } else {
        painter.setBrush(fillColor);
    }

    double w = width();
    double h = height();
    double cx = w / 2.0;
    double cy = h / 2.0;
    double R = qMin(w, h) / 2.0;   // outer radius
    double r = R * 0.4;             // inner radius

    QPolygonF polygon;
    int totalVertices = points * 2;
    for (int i = 0; i < totalVertices; ++i) {
        double angle = M_PI * i / points - M_PI / 2.0;
        double radius = (i % 2 == 0) ? R : r;
        double vx = cx + radius * qCos(angle);
        double vy = cy + radius * qSin(angle);
        polygon << QPointF(vx, vy);
    }

    painter.drawPolygon(polygon);
}
