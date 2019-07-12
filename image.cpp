#include <QQueue>
#include "image.h"
#include <QDebug>
#include <QImageWriter>

Image::Image()
{

}

Image::Image(const QString& file)
{
    image = new QImage(":/Resources/Images/" + file);
    height = image->height();
    width = image->width();

    map.resize(height);

    for (int y = 0; y < height; y++)
    {
        map[y].resize(width);
        for (int x = 0; x < width; x++)
        {
            QColor color = image->pixelColor(x, y);
            int mapValue = (color.red() + color.green() + color.blue()) / 3;
            map[y][x] = 0;
            if (mapValue <= 127)
            {
                map[y][x] = 1;
            }
        }
    }

    captured = false;
    resized = false;
}

QPixmap Image::getPixmap()
{
    return QPixmap::fromImage(*image);
}

void Image::createCaptureWindows()
{
    if (captured)
    {
        qDebug() << "Image is captured already";
        return;
    }

    QVector <QVector <bool>> copyMap = map;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (copyMap[y][x])
            {
                QQueue <QPoint> q;
                q.enqueue(QPoint(x, y));
                QVector <QPoint> figure;

                int minX = width, minY = height, maxX = 0, maxY = 0;

                while (!q.isEmpty())
                {
                    QPoint current = q.dequeue();
                    figure.push_back(current);
                    if (!copyMap[current.y()][current.x()])
                    {
                        continue;
                    }

                    minX = qMin(minX, current.x());
                    minY = qMin(minY, current.y());
                    maxX = qMax(maxX, current.x());
                    maxY = qMax(maxY, current.y());

                    copyMap[current.y()][current.x()] = false;
                    for (int dx = -1; dx <= 1; dx++)
                    {
                        for (int dy = -1; dy <= 1; dy++)
                        {
                            if (dx == 0 && dy == 0)
                            {
                                continue;
                            }
                            if (current.x() + dx >= 0 && current.x() + dx < width &&
                                current.y() + dy >= 0 && current.y() + dy < height &&
                                copyMap[current.y() + dy][current.x() + dx])
                            {
                                q.enqueue(QPoint(current.x() + dx, current.y() + dy));
                            }
                        }
                    }
                }

                CaptureWindow newWindow = CaptureWindow(minX - 1, minY - 1, maxX - minX + 2, maxY - minY + 2);
                newWindow.setImage(image);
                newWindow.setMap(figure);
                newWindow.scan();
                captures.push_back(newWindow);
            }
        }
    }

    qSort(captures.begin(), captures.end(),
          [](const CaptureWindow& left, const CaptureWindow& right)
    {
        return left.getSquare() < right.getSquare();
    });

    for (int i = 0; i < captures.size(); i++)
    {
        if (i == 0)
        {
            captures[i].print(QColor(0, 0, 255));
        }
        else if (i == captures.size() - 1)
        {
            captures[i].print(QColor(255, 0, 0));
        }
        else
        {
            captures[i].print();
        }
    }

    captured = true;
}

void Image::swapCapturesSize()
{
    if (!captured)
    {
        qDebug() << "Image is not captured";
        return;
    }
    if (resized)
    {
        qDebug() << "Image is resized already";
        return;
    }

    double k = sqrt(captures.back().getSquare() / captures.front().getSquare());
    captures.front().setScale(k + 1);
    captures.front().print();

    captures.back().setScale(1 / (k + 1));
    captures.back().print();

    qSort(captures.begin(), captures.end(),
          [](const CaptureWindow& left, const CaptureWindow& right)
    {
        return left.getSquare() < right.getSquare();
    });

    for (int i = 0; i < captures.size(); i++)
    {
        if (i == 0)
        {
            captures[i].print(QColor(0, 0, 255));
        }
        else if (i == captures.size() - 1)
        {
            captures[i].print(QColor(255, 0, 0));
        }
        else
        {
            captures[i].print();
        }
    }

    resized = true;
}

void Image::swapMinMax()
{
    if (!captured)
    {
        qDebug() << "Image is not captured";
        return;
    }

    for (CaptureWindow& wnd : captures)
    {
        wnd.clearImage();
    }

    QPoint minCenter = captures.front().getCenter();
    captures.front().setCenter(captures.back().getCenter());
    captures.back().setCenter(minCenter);

    captures.front().print(QColor(0, 0, 255));
    for (int i = 1; i < captures.size() - 1; i++)
    {
        captures[i].print();
    }
    captures.back().print(QColor(255, 0, 0));
}

void Image::rotateMax(bool isClockwise)
{
    if (!captured)
    {
        qDebug() << "Image is not captured";
        return;
    }
    if (!resized)
    {
        qDebug() << "Image is not resized";
        return;
    }

    for (CaptureWindow& wnd : captures)
    {
        wnd.clearImage();
    }

    captures.back().rotate(captures.front().getCenter(), isClockwise);

    captures.front().print(QColor(0, 0, 255));
    for (int i = 1; i < captures.size() - 1; i++)
    {
        captures[i].print();
    }
    captures.back().print(QColor(255, 0, 0));

}

void Image::write()
{
    QImageWriter qiw("Answer.png");
    qiw.write(*image);
}

QString Image::getObjectInfo(const QPoint &pos) const
{
    if (!captured)
    {
        return "";
    }

    for (const CaptureWindow& wnd : captures)
    {
        if(wnd.getBoundingRect().contains(pos))
        {
            return wnd.getDebugInfo();
        }
    }

    return "";
}

Image::~Image()
{
    delete image;
}
