
#include "tcpclient.h"
#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
#include <string>
#include <QtWidgets>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString name;
    TcpClient client;

    if(argc > 1){
        name = argv[1];
        client.setNickName(name);
    }else{
        name = "this shit empty";
        client.setNickName(name);
    }
    if (argc >= 4){
        client.firstConnect(argv[2], atoi(argv[3]));
    }
    return a.exec();
}
