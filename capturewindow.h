#ifndef CAPTUREWINDOW_H
#define CAPTUREWINDOW_H
#include <QImage>

class CaptureWindow
{
public:
    CaptureWindow();
    CaptureWindow(int x, int y, int w, int h);

    void setImage(QImage* image);
    void setMap(QVector <QPoint>& map);
    void setScale(double scale);
    int getSquare() const;
    QPoint getCenter() const;
    void setCenter(const QPoint& center);
    void scan();
    void print() const;
    void print(QColor lineColor) const;
    QString getDebugInfo() const;
    void clearImage();
    void rotate(const QPoint& center, bool isCLoskwise);
    QRect getBoundingRect() const;

private:
    QPoint leftPoint;
    int width, height, square;
    QPoint center;
    QImage *image;
    QVector <QVector<bool>> selfMap;

    bool reprint;
    void printPicture() const;

    double distance(const QPoint& left, const QPoint& right) const;
    void pointConnect(const QPoint& from, const QPoint& to);
    int neighbourCount(int x, int y) const;
    int neighbourCount(int x, int y, bool) const;
    int angleFromCenter(QPoint& point) const;
};

#endif // CAPTUREWINDOW_H
