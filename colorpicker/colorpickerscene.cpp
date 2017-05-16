#include "colorpickerscene.hpp"
#include <QApplication>
#include <QClipboard>
#include <QGraphicsEllipseItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QTimer>

ColorPickerScene::ColorPickerScene(QPixmap *pixmap, QWidget *parentWidget)
: QGraphicsScene(), QGraphicsView(this, parentWidget) {
    setFrameShape(QFrame::NoFrame); // Time taken to solve: A george99g and 38 minutes.
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    setCursor(QCursor(Qt::CrossCursor));
    setMouseTracking(true);

    pItem = addPixmap(*pixmap);
    pItem->setZValue(-2);
    ellipse = addEllipse(QRectF(QCursor::pos(), QSize(20, 20)), QPen(Qt::cyan), Qt::NoBrush);
    QFont font("Monospace");
    font.setStyleHint(QFont::Monospace);
    text = addText("#hiyouu", font);
    textBackground = addRect(text->boundingRect(), Qt::NoPen, QBrush(Qt::black));
    text->setPos(QCursor::pos() + QPoint(25, 0));
    textBackground->setPos(text->pos());
    textBackground->setZValue(-1);
    color = pItem->pixmap().toImage().pixelColor(QCursor::pos());
    text->setPlainText(color.name());
    ellipse->setBrush(color);
}

void ColorPickerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    ellipse->setRect(QRectF(event->scenePos(), QSize(20, 20)));
    color = pItem->pixmap().toImage().pixelColor(event->scenePos().toPoint());
    text->setPos(QCursor::pos() + QPoint(25, 0));
    text->setPlainText(color.name());
    textBackground->setPos(text->pos());
    ellipse->setBrush(color);
}

void ColorPickerScene::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Return) QApplication::clipboard()->setText(color.name());
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Escape) close();
}

void ColorPickerScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *) {
    QApplication::clipboard()->setText(color.name());
    close();
}
