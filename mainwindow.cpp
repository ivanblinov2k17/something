#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cstring>
MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(connected()),this,SLOT(onSocketConnection()));
    socket->connectToHost(QHostAddress("192.168.1.41"),8000);
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    ui->textEdit->setText("Enter your message here");
    ui->textBrowser->setText("lol");
}
class chat_message
{
    int id;
    int body_len;
    char* message = new char[512];
    chat_message()
    {
        this->id=0;
        this->body_len=0;
    }
};

int partly_encrypt_header(QByteArray _data,int i)
{
    return (_data[3+i]-48+(_data[2+i]-48)*10+(_data[1+i]-48)*100+(_data[0+i]-48)*1000);
}
char* crypt_message(std::string _data,int id)
{
    char* data=new char[512];
    std::sprintf(data, "%04d", _data.length());
    char* id_ = new char[12];

    std::sprintf(id_, "%04d", id);
    std::strcat(data, id_);
    std::strcat(data, _data.c_str());
    for(int i=8;i<_data.length();++i)
    {
        data[i]=_data[i-8];
    }

    return data;
}

void MainWindow::onSocketConnection()
{
qDebug()<<"Socket connected";

}

void MainWindow::onReadyRead()
{

    QByteArray s;
    QByteArray message;
    while(socket->canReadLine())
    {
        s=socket->readLine();
        //if (partly_encrypt_header(s,0)+8==s.length())
        //{
            qDebug()<<partly_encrypt_header(s,0);

            qDebug()<<partly_encrypt_header(s,4);
            for(int i=0;i<partly_encrypt_header(s,0);++i)
            {
                qDebug()<<s[i+8];
            }
            qDebug()<<endl;
            qDebug()<<s.length()<<" "<<partly_encrypt_header(s,0)+8<<endl;
        //}

    }

}

MainWindow::~MainWindow()
{
delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString str = ui->textEdit->toPlainText();

    // Either this if you use UTF-8 anywhere
    std::string utf8_text = str.toUtf8().constData();



    char* sos = new char[512];
    sos=crypt_message(utf8_text,27);
    qDebug()<<sos;

    socket->write(sos);
}
