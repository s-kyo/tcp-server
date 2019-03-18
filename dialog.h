#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtNetwork>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();



private:
    Ui::Dialog *ui;
    QTcpServer *server;
    QTcpSocket *socket;
    int max_packet_len;
    bool auto_reply;
private slots:
    void newConnect();
    void SendMessage();
    void ReceiveMessage();
    void on_pushButton_start_clicked();
    void on_pushButton_stop_clicked();
    void on_pushButton_clear_info_clicked();
    void on_pushButton_send_clicked();
    void on_checkBox_stateChanged(int arg1);
    void on_lineEdit_send_returnPressed();
};

#endif // DIALOG_H
