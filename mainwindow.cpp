#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chat_message.h"
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
    //ui->plainTextEdit->setText("lol");



}

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
//socket->write("0006Privet");
}

void MainWindow::onReadyRead()
{

    QByteArray s;
    QByteArray message;
    bool is_logged_in = false;
    while(socket->bytesAvailable()>0)
    {
        s=socket->read(4);

        int length=partly_encrypt_header(s,0);
        if (length>0&&length<512)
        {
            s=socket->read(4);
            int id=partly_encrypt_header(s,0);
            if(id==1){

                    ui->textEdit1->append("Succesfull log in");
                    s=socket->read(length);
                    me.id=partly_encrypt_header(s,0);
                    s=socket->read(4);
                    length=partly_encrypt_header(s,0);
                    s=socket->read(4);
                    id=partly_encrypt_header(s,0);
                    if(id==1)
                    {
                        s=socket->read(length);
                        number_of_users = s[0]+48;
                        for(int i=0;i<number_of_users;++i)//IMPORTANT MOMENT - IF WE WANT MORE/ THAN 9 debug this
                            {
                                s=socket->read(4);
                                length=partly_encrypt_header(s,0);
                                s=socket->read(4);
                                id=partly_encrypt_header(s,0);
                                s=socket->read(length);
                                int k=0;
                                while (s[k]!=' ')
                                {
                                    users[i].username[k]=s[k];
                                    k++;
                                }
                                k++;
                                users[i].id=s[k]+48;

                            }
                    }
                    is_logged_in=true;
            }
            else
            {

                    if(is_logged_in)
                    {
                        char* c = new char[2];
                        c[1]=id%10+48;
                        c[0]=(id/10)%10+48;


                        ui->textEdit1->append(c);

                        s=socket->read(length);

                        ui->textEdit1->append(s);
                    }

            }

        }

        //if (partly_encrypt_header(s,0)+8==s.length())
        //{
//            qDebug()<<partly_encrypt_header(s,0);

//            qDebug()<<partly_encrypt_header(s,4);
//            for(int i=0;i<partly_encrypt_header(s,0);++i)
//            {
//                qDebug()<<s[i+8];
//            }
//            qDebug()<<endl;
//            qDebug()<<s.length()<<partly_encrypt_header(s,0)+8<<endl;
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



    char* tmp = new char[512];
    if(me.id==0)
    {

        char* data=new char[512];
        std::sprintf(data, "%04d", utf8_text.length());
        std::strcat(data, utf8_text.c_str());
        tmp = data;
    }
    else
        tmp=crypt_message(utf8_text,me.id);

    socket->write(tmp);
}
