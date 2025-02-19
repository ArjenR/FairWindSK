//
// Created by Raffaele Montella on 20/02/25.
//

#include "ImageViewerWidget.hpp"

#include <QApplication>

// Original source code https://github.com/p-ranav/ImageViewer-Qt6

namespace fairwindsk::ui::mydata {

    ImageViewerWidget::ImageViewerWidget(QWidget *parent) : QGraphicsView(parent) {
  setScene(&m_scene);
  m_scene.addItem(&m_item);
  setDragMode(QGraphicsView::ScrollHandDrag);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setResizeAnchor(QGraphicsView::AnchorViewCenter);
  setStyleSheet("background: transparent;");
}

void ImageViewerWidget::setPixmap(const QPixmap &pixmap, const int desiredWidth,
                            const int desiredHeight) {
  // Reset transformation before setting the new pixmap
  m_item.resetTransform();

  // Set the QGraphicsPixmapItem's pixmap
  m_item.setPixmap(pixmap);

  // Enable smooth transformation by setting interpolation mode
  m_item.setTransformationMode(Qt::SmoothTransformation);

  // Calculate the scale factors to achieve the desired width and height while
  // maintaining aspect ratio
  if (pixmap.width() > desiredWidth || pixmap.height() > desiredHeight) {
    qreal scaleFactorWidth = static_cast<qreal>(desiredWidth) / pixmap.width();
    qreal scaleFactorHeight =
        static_cast<qreal>(desiredHeight) / pixmap.height();
    qreal scaleFactor = qMin(scaleFactorWidth, scaleFactorHeight);

    // Scale the QGraphicsPixmapItem
    QGraphicsView::resetTransform();
    scale(scaleFactor);
  } else {
    QGraphicsView::resetTransform();
  }

  // Center the QGraphicsPixmapItem in the window
  auto offset = -QRectF(pixmap.rect()).center();
  m_item.setOffset(offset);

  // Set the scene rect
  m_scene.setSceneRect(-pixmap.width() / 2.0, -pixmap.height() / 2.0,
                       pixmap.width(), pixmap.height());
}

QPixmap ImageViewerWidget::pixmap() const { return m_item.pixmap(); }

void ImageViewerWidget::scale(qreal s) { QGraphicsView::scale(s, s); }

void ImageViewerWidget::resize(int desiredWidth, int desiredHeight) {
  const auto &pixmap = m_item.pixmap();

  if (pixmap.width() > desiredWidth || pixmap.height() > desiredHeight) {
    qreal scaleFactorWidth = static_cast<qreal>(desiredWidth) / pixmap.width();
    qreal scaleFactorHeight =
        static_cast<qreal>(desiredHeight) / pixmap.height();
    qreal scaleFactor = qMin(scaleFactorWidth, scaleFactorHeight);
    QGraphicsView::resetTransform();
    scale(scaleFactor);
  } else {
    QGraphicsView::resetTransform();
  }

  // Center the QGraphicsPixmapItem in the window
  auto offset = -QRectF(pixmap.rect()).center();
  m_item.setOffset(offset);

  // Set the scene rect
  m_scene.setSceneRect(-pixmap.width() / 2.0, -pixmap.height() / 2.0,
                       pixmap.width(), pixmap.height());
}

void ImageViewerWidget::zoomIn() { scale(1.2); }

void ImageViewerWidget::zoomOut() { scale(0.8); }

bool ImageViewerWidget::event(QEvent *event) {
  if (event->type() == QEvent::NativeGesture) {
    return nativeGestureEvent(static_cast<QNativeGestureEvent *>(event));
  } else if (event->type() == QEvent::Wheel) {
    wheelEvent(static_cast<QWheelEvent *>(event));
    return true;
  } else {
    return QGraphicsView::event(event);
  }
}

void ImageViewerWidget::wheelEvent(QWheelEvent *event) {
  // Check if the wheel event was generated by a mouse
  if (event->source() == Qt::MouseEventNotSynthesized) {
    if (event->angleDelta().y() > 0) {
      scale(ImageViewerWidget::ZOOM_IN_SCALE);
    } else {
      scale(ImageViewerWidget::ZOOM_OUT_SCALE);
    }
  }
  QGraphicsView::wheelEvent(event);
}

bool ImageViewerWidget::nativeGestureEvent(QNativeGestureEvent *event) {
  if (event->gestureType() == Qt::ZoomNativeGesture) {
    qreal scaleFactor = event->value();

    if (scaleFactor > 0.0) {
      scale(ImageViewerWidget::ZOOM_IN_SCALE);
    } else if (scaleFactor < 0.0) {
      scale(ImageViewerWidget::ZOOM_OUT_SCALE);
    }

    return true;
  }

  return false;
}

void ImageViewerWidget::keyPressEvent(QKeyEvent *event) {
  if (parentWidget()) {
    QApplication::sendEvent(parentWidget(), event);
  }
}

} // fairwindsk::ui::mydata