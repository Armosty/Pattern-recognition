#ifndef IMAGE_H
#define IMAGE_H
#include <QString>
#include <QVector>
#include <QImage>
#include <QPixmap>
#include "capturewindow.h"

class Image
{
public:
    Image();
    Image(const QString& file);
    ~Image();

    QPixmap getPixmap();
    void createCaptureWindows();
    void swapCapturesSize();
    void swapMinMax();
    void rotateMax(bool isClockwise);
    void write();
    QString getObjectInfo(const QPoint& pos) const;

private:
    QImage *image;
    QVector <QVector <bool>> map;
    QVector <CaptureWindow> captures;
    int height, width;

    bool captured, resized;
};

#endif // IMAGE_H
