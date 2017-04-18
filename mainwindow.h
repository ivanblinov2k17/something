#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QDebug>
#include <QHostAddress>
#include <string>
#include "chat_message.h"

namespace Ui {
class MainWindow;
}
static int number_of_users=0;
class MainWindow : public QMainWindow
{
Q_OBJECT

public:
explicit MainWindow(QWidget *parent = 0);
~MainWindow();
QTcpSocket* socket;
chat_user* users=new chat_user[256];
chat_user me;

private:
Ui::MainWindow *ui;

private slots:
void onSocketConnection();
void onReadyRead();
void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
