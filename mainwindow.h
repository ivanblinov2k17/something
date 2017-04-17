#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT

public:
explicit MainWindow(QWidget *parent = 0);
~MainWindow();
QTcpSocket* socket;

private:
Ui::MainWindow *ui;

private slots:
void onSocketConnection();
void onReadyRead();
void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
