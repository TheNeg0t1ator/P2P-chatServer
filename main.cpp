
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
    /*
     * Create a QT application that will be used for all the gui elements
     * and create an instance of the TcpClient class
     */
    QApplication a(argc, argv);
    QString name;
    TcpClient client;

   /*
     * If 3 or more arguments (IP addresses, ports, name, etc.) are given
     * then proceed but print a warning if there are less then 3 arguments given
     */
    if(argc > 1){
        name = argv[1];
        client.setNickName(name);
    }else{
        name = "this shit empty";
        client.setNickName(name);
        qWarning() << "ERROR: Not enough arguments were given";
    }
  // start the QT event loop (user input, network events, other events...)
    if (argc >= 4){
        client.firstConnect(argv[2], atoi(argv[3]));
    }
    return a.exec();
}
