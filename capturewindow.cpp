#include "capturewindow.h"
#include <QDebug>
#include <QQueue>
#include <cmath>

CaptureWindow::CaptureWindow()
{

}

CaptureWindow::CaptureWindow(int x, int y, int w, int h)
    : leftPoint(x, y), width(w), height(h), square(0), image(nullptr), reprint(false)
{

}

void CaptureWindow::setImage(QImage *image)
{
    this->image = image;
}

void CaptureWindow::setMap(QVector<QPoint>& map)
{
    selfMap.resize(height);
    for (int y = 0; y < height; y++)
    {
        selfMap[y].resize(width);
    }
    for (const QPoint& point : map)
    {
        QPoint relativePoint = point - leftPoint;
        selfMap[relativePoint.y()][relativePoint.x()] = true;
    }
}

void CaptureWindow::scan()
{
    square = 0;
    int medianX = 0, medianY = 0;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (selfMap[y][x])
            {
                square++;
                medianX += x;
                medianY += y;
            }
        }
    }
    medianX /= square;
    medianY /= square;

    center = QPoint(leftPoint.x() + medianX, leftPoint.y() + medianY);
}

void CaptureWindow::setScale(double scale)
{
    QVector <QPoint> borders;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (selfMap[y][x] && neighbourCount(x, y) < 4)
            {
                borders.push_back(QPoint(x, y));
            }
        }
    }

    bool isCircle = false;
    QPoint relativeCenter = center - leftPoint;
    double radius = distance(relativeCenter, borders.front());
    int i;
    for (i = 1; i < borders.size(); i++)
    {
        if (qAbs(distance(relativeCenter, borders[i]) - radius) > 5)
        {
            break;
        }
    }
    if (i == borders.size())
    {
        isCircle = true;
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            selfMap[y][x] = false;
        }
    }
    clearImage();

    if (isCircle)
    {
        int newRadius = radius * scale;
        leftPoint = QPoint(center.x() - newRadius - 1, center.y() - newRadius - 1);
        width = height = newRadius * 2 + 2;

        relativeCenter = center - leftPoint;
        selfMap.resize(height);
        for (int y = 0; y < height; y++)
        {
            selfMap[y].resize(width);
            for (int x = 0; x < width; x++)
            {
                QPoint relativePoint(x, y);
                selfMap[y][x] = distance(relativePoint, relativeCenter) <= newRadius;
            }
        }
        scan();
        return;
    }

    qSort(borders.begin(), borders.end(),
          [&](const QPoint& left, const QPoint& right)
    {
        return distance(left, relativeCenter) < distance(right, relativeCenter);
    });

    QVector <QPoint> corners;

    for (int i = borders.size() - 1; i >= 0 && corners.size() < 5; i--)
    {
        int j = 0;
        QPoint currentRelative = borders[i] - relativeCenter;
        int currentAngle = angleFromCenter(currentRelative);
        for (; j < corners.size(); j++)
        {
            QPoint relativePoint = corners[j] - relativeCenter;
            int angleCorn = angleFromCenter(relativePoint);
            int diff = qAbs(angleCorn - currentAngle);
            if (diff >= 310)
            {
                diff = 360 - diff;
            }
            if (diff <= 50)
            {
                break;
            }
        }
        if (j == corners.size())
        {
            corners.push_back(borders[i]);
        }
    }

    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    for (QPoint& point : corners)
    {
        point -= relativeCenter;
        point *= scale;
        point += relativeCenter;

        QPoint displayPoint = leftPoint + point;
        point -= relativeCenter;

        minX = qMin(minX, displayPoint.x());
        minY = qMin(minY, displayPoint.y());
        maxX = qMax(maxX, displayPoint.x());
        maxY = qMax(maxY, displayPoint.y());
    }

    leftPoint = QPoint(minX - 1, minY - 1);
    width = maxX - minX + 2;
    height = maxY - minY + 2;
    relativeCenter = center - leftPoint;

    for (QPoint& point : corners)
    {
        point += relativeCenter;
    }

    selfMap.resize(height);
    for (int i = 0; i < height; i++)
    {
        selfMap[i].resize(width);
    }

    for (const QPoint& point : corners)
    {
        selfMap[point.y()][point.x()] = true;
    }

    qSort(corners.begin(), corners.end(),
          [&](QPoint& left, QPoint& right)
    {
        QPoint relativeLeft = left - relativeCenter;
        QPoint relativeRight = right - relativeCenter;
        return angleFromCenter(relativeLeft) < angleFromCenter(relativeRight);
    });

    for (int i = 0; i < corners.size() - 1; i++)
    {
        pointConnect(corners[i], corners[i + 1]);
    }
    pointConnect(corners[0], corners[corners.size() - 1]);

    QQueue <QPoint> q;
    q.enqueue(relativeCenter);
    selfMap[relativeCenter.y()][relativeCenter.x()] = true;

    QVector <QPoint> neighbour
    {
        { 0, -1 },
        { 1,  0 },
        { 0,  1 },
        {-1,  0 }
    };

    while (!q.isEmpty())
    {
        QPoint current = q.dequeue();

        for (QPoint& point : neighbour)
        {
            QPoint target = current + point;
            if (!selfMap[target.y()][target.x()])
            {
                selfMap[target.y()][target.x()] = true;
                q.enqueue(target);
            }
        }
    }
    scan();
}

int CaptureWindow::angleFromCenter(QPoint &relativePoint) const
{
    static QPoint xVector(width / 2, 0);
    static double xLength = sqrt(pow(xVector.x(), 2) + pow(xVector.y(), 2));
    static double pi = 3.14159265358979323846;

    if (relativePoint == QPoint(0, 0))
    {
        return 0;
    }

    double pointLength = sqrt(pow(relativePoint.x(), 2) + pow(relativePoint.y(), 2));
    double angle = acos((relativePoint.x() * xVector.x() + relativePoint.y() * xVector.y()) / (xLength * pointLength));
    int degreesAngle = angle * 180 / pi;
    if (relativePoint.y() > 0)
    {
        degreesAngle = 360 - degreesAngle;
    }

    return degreesAngle;
}

double CaptureWindow::distance(const QPoint &left, const QPoint &right) const
{
    return sqrt(pow(left.x() - right.x(), 2) +
                pow(left.y() - right.y(), 2));
}

void CaptureWindow::pointConnect(const QPoint &from, const QPoint &to)
{
    double dx = (to.x() - from.x()) / distance(from, to);
    double dy = (to.y() - from.y()) / distance(from, to);
    QPoint current(from);
    double currX = current.x();
    double currY = current.y();
    while (current != to)
    {
        if (current.x() != to.x())
        {
            currX += dx;
        }
        if (current.y() != to.y())
        {
            currY += dy;
        }

        current = QPoint(currX, currY);
        selfMap[current.y()][current.x()] = true;
    }
}

int CaptureWindow::getSquare() const
{
    return square;
}

QPoint CaptureWindow::getCenter() const
{
    return center;
}

void CaptureWindow::setCenter(const QPoint& center)
{
    this->center = center;
    leftPoint = QPoint(center.x() - width / 2, center.y() - height / 2);
    reprint = true;
}

QString CaptureWindow::getDebugInfo() const
{
    QString result = "X: " + QString::number(leftPoint.x()) + "\n";
    result += "Y: " + QString::number(leftPoint.y()) + "\n";
    result += "Width: " + QString::number(width) + "\n";
    result += "Height: " + QString::number(height) + "\n";
    result += "Square: " + QString::number(square) + "\n";
    result += "Center X: " + QString::number(center.x()) + "\n";
    result += "Center Y: " + QString::number(center.y()) + "\n";

    return result;
}

void CaptureWindow::print() const
{
    print(QColor(0, 255, 0));
}

void CaptureWindow::print(QColor lineColor) const
{
    if (image == nullptr)
    {
        qDebug() << "Image not captured!!";
        return;
    }

    if (reprint)
    {
        printPicture();
    }

    for (int x = qMax(leftPoint.x() - 1, 0);
         x < qMin(leftPoint.x() + width + 2, image->width());
         x++)
    {
        if (leftPoint.y() > 0)
        {
            image->setPixelColor(x, leftPoint.y() - 1, lineColor);
        }
        if (leftPoint.y() + height + 1 < image->height())
        {
            image->setPixelColor(x, leftPoint.y() + height + 1, lineColor);
        }
    }
    for (int y = qMax(leftPoint.y() - 1, 0);
         y < qMin(leftPoint.y() + height + 2, image->height());
         y++)
    {
        if (leftPoint.x() > 0)
        {
            image->setPixelColor(leftPoint.x() - 1, y, lineColor);
        }
        if (leftPoint.x() + width + 1 < image->width())
        {
            image->setPixelColor(leftPoint.x() + width + 1, y, lineColor);
        }
    }
    image->setPixelColor(center.x(), center.y(), lineColor);
}

void CaptureWindow::printPicture() const
{
    QVector <QVector<unsigned char>> opacityMask(height, QVector<unsigned char>(width, 0));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (selfMap[y][x])
            {
                opacityMask[y][x] = neighbourCount(x, y, true);
            }
        }
    }

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (leftPoint.x() + x < 0 || leftPoint.x() + x >= image->width() ||
                leftPoint.y() + y < 0 || leftPoint.y() + y >= image->height())
            {
                continue;
            }

            QColor color = image->pixelColor(leftPoint + QPoint(x, y));
            int medianValue = (color.red() + color.green() + color.blue()) / 3;
            if (medianValue <= 127)
            {
                continue;
            }
            int printColor = (1 - selfMap[y][x]) * 255;
            switch (opacityMask[y][x])
            {
            case 1:
                printColor = 220;
                break;
            case 2:
                printColor = 200;
                break;
            case 3:
                printColor = 180;
                break;
            case 4:
                printColor = 150;
                break;
            case 5:
                printColor = 70;
                break;
            case 6:
                printColor = 40;
                break;
            case 7:
                printColor = 30;
                break;
            case 8:
                printColor = 0;
                break;
            default:
                printColor = (1 - selfMap[y][x]) * 255;
                break;
            }
            image->setPixelColor(leftPoint + QPoint(x, y), QColor(printColor, printColor, printColor));
        }
    }
}

void CaptureWindow::clearImage()
{
    for (int y = leftPoint.y() - 2; y <= leftPoint.y() + height + 1; y++)
    {
        for (int x = leftPoint.x() - 2; x <= leftPoint.x() + width + 1; x++)
        {
            if (x >= 0 && x < image->width() &&
                y >= 0 && y < image->height())
            {
                image->setPixelColor(x, y, QColor(255, 255, 255));
            }
        }
    }
    reprint = true;
}

int CaptureWindow::neighbourCount(int x, int y) const
{
    int count = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == dy || dx + dy == 0 ||
                x + dx < 0 || x + dx >= width ||
                y + dy < 0 || y + dy >= height ||
                !selfMap[y + dy][x + dx])
            {
                continue;
            }
            count++;
        }
    }
    return count;
}

int CaptureWindow::neighbourCount(int x, int y, bool) const
{
    int count = 0;
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            if (dx == 0 && dy == 0 ||
                x + dx < 0 || x + dx >= width ||
                y + dy < 0 || y + dy >= height ||
                !selfMap[y + dy][x + dx])
            {
                continue;
            }
            count++;
        }
    }
    return count;
}

void CaptureWindow::rotate(const QPoint& center, bool isCLoskwise)
{
    clearImage();
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    QVector <QPoint> afterRotate;

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (!selfMap[y][x])
            {
                continue;
            }
            QPoint displayPoint = leftPoint + QPoint(x, y);
            QPoint relativePoint = displayPoint - center;
            QPoint inversePoint = QPoint(relativePoint.y(), relativePoint.x());

            if (isCLoskwise)
            {
                inversePoint.setY(-inversePoint.y());
            }
            else
            {
                inversePoint.setX(-inversePoint.x());
            }
            inversePoint += center;

            afterRotate.push_back(inversePoint);

            minX = qMin(minX, inversePoint.x());
            minY = qMin(minY, inversePoint.y());
            maxX = qMax(maxX, inversePoint.x());
            maxY = qMax(maxY, inversePoint.y());

        }
    }

    leftPoint = QPoint(minX - 1, minY - 1);
    width = maxX - minX + 2;
    height = maxY - minY + 2;

    selfMap.clear();
    selfMap.resize(height);
    for (int i = 0; i < height; i++)
    {
        selfMap[i].resize(width);
        selfMap[i].fill(false);
    }

    for (QPoint& point : afterRotate)
    {
        point -= leftPoint;
        selfMap[point.y()][point.x()] = true;
    }
    scan();
}

QRect CaptureWindow::getBoundingRect() const
{
    return QRect(leftPoint.x(), leftPoint.y(), width, height);
}

