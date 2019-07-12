#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPixmap>
#include <QImage>
#include <QMouseEvent>
#include <QDebug>
#include "image.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    image(nullptr)
{
    ui->setupUi(this);

    for (int i = 1; i <= 32; i++)
    {
        QString picName = "Лб_1_0";
        if (i < 10)
        {
            picName += "0";
        }
        picName += QString::number(i);
        ui->comboBox->addItem(picName);
    }
    ui->image->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    if (image != nullptr)
    {
        image->write();
        delete image;
    }
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (image == nullptr)
    {
        return false;
    }

    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *mEvent = (QMouseEvent*)event;

        ui->infoLabel->setText(image->getObjectInfo(mEvent->pos()));
    }

    return false;
}

void MainWindow::on_pushButton_clicked()
{
    if (image != nullptr)
    {
        delete image;
    }

    image = new Image(ui->comboBox->currentText());
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_pushButton_2_clicked()
{
    if (image == nullptr)
    {
        qDebug() << "Image not opened";
        return;
    }

    image->createCaptureWindows();
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_pushButton_3_clicked()
{
    if (image == nullptr)
    {
        qDebug() << "Image not opened";
        return;
    }

    image->swapCapturesSize();
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_pushButton_4_clicked()
{
    if (image != nullptr)
    {
        image->write();
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    if (image == nullptr)
    {
        qDebug() << "Image not opened";
        return;
    }
    image->swapMinMax();
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_pushButton_6_clicked()
{
    if (image == nullptr)
    {
        qDebug() << "Image not opened";
        return;
    }
    image->rotateMax(true);
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_pushButton_7_clicked()
{
    if (image == nullptr)
    {
        qDebug() << "Image not opened";
        return;
    }
    image->rotateMax(false);
    ui->image->setPixmap(image->getPixmap());
}

void MainWindow::on_image_customContextMenuRequested(const QPoint &pos)
{

}

void MainWindow::on_image_linkActivated(const QString &link)
{

}
