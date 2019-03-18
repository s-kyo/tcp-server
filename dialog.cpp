#include "dialog.h"
#include "ui_dialog.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("TCP server for ns-3 clients"));
    this->resize(800,450);
    server = new QTcpServer(this);
    socket = nullptr;
    max_packet_len = 1024;
    auto_reply = false;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::newConnect()
{
    socket = server->nextPendingConnection();
    ui->listWidget_client->addItem(tr("%1 : %2").arg(socket->peerAddress().toString()).arg(socket->peerPort()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(ReceiveMessage()));
}

void Dialog::SendMessage()
{
    if(socket->isWritable()){
        QString str = ui->lineEdit_send->text();
        ui->lineEdit_send->clear();
        socket->write(str.toLocal8Bit().data(), max_packet_len);
    }
}

void Dialog::ReceiveMessage()
{
    char *buffer = new char[max_packet_len+1];
    socket->read(buffer, max_packet_len);
    ui->plainTextEdit_info->appendPlainText(tr("[%1]%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(buffer));
    if(auto_reply){
        socket->write(buffer, max_packet_len);
    }
}

void Dialog::on_pushButton_start_clicked()
{
    if(server->isListening()){
        ui->plainTextEdit_info->appendPlainText(tr("server is listening"));
        return;
    }
    quint16 port = ui->lineEdit_port->text().toUShort();
    if(!server->listen(QHostAddress::Any, port)){
        ui->plainTextEdit_info->appendPlainText(server->errorString());
        server->close();
        return;
    }
    ui->plainTextEdit_info->appendHtml(tr("<font color = \"Green\">create server at port %1</font>").arg(port));
    ui->pushButton_stop->setEnabled(true);
    ui->pushButton_start->setEnabled(false);
    connect(server, SIGNAL(newConnection()),this,SLOT(newConnect()));
}

void Dialog::on_pushButton_stop_clicked()
{
    if(socket)socket->close();
    server->disconnect();
    server->close();
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setEnabled(false);
    ui->plainTextEdit_info->appendHtml(tr("<font color = \"red\">server stopped</font>"));
}

void Dialog::on_pushButton_clear_info_clicked()
{
    ui->plainTextEdit_info->clear();
}

void Dialog::on_pushButton_send_clicked()
{
    SendMessage();
    ui->plainTextEdit_info->appendHtml("<font color=\"Green\">send a message</font>");
}

void Dialog::on_checkBox_stateChanged(int arg1)
{
    if(arg1){
        auto_reply = true;
    }else{
        auto_reply = false;
    }
}

void Dialog::on_lineEdit_send_returnPressed()
{
    SendMessage();
}
