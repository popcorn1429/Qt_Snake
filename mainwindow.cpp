#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"

#include <QMessageBox>
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    thresholdSquared(100)
{
    ui->setupUi(this);

    //init random function seed
    qsrand(static_cast<uint>(std::time(nullptr)));

    //
    world = new World(ui->worldWidget->x(), ui->worldWidget->y(), ui->worldWidget->width(), ui->worldWidget->height());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::animate);
    timer->start(50);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::animate()
{
    if (!world->moveOn())
    {
        timer->stop();
        if (QMessageBox::warning(this, "Game Over", "Crashed! Game Over!", QMessageBox::Ok)) {
            qApp->quit();
        }
    }
    update();  //call paintEvent
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    world->paint(&painter);
    painter.end();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if ( event->x() > ui->worldWidget->x() && event->x() < ui->worldWidget->x() + ui->worldWidget->width()
         && event->y() > ui->worldWidget->y() && event->y() < ui->worldWidget->y() + ui->worldWidget->height()) {
        rod.setX(event->x());
        rod.setY(event->y());
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        if (distanceSquared(QPointF(event->pos()), rod) <= thresholdSquared) {
            return;
        }

        world->updateCtrlDir(rod, event->pos());
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //nothing ?
}


