
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
     * Create a QT application that will be used for all the gui elements,
     * an instance of the TcpClient class
     * and a Qstring that will contain your name in Unicode
     */
    QApplication a(argc, argv);
    QString name;
    TcpClient client;

   /*
     * If more then one argument (IP addresses, ports, name, etc.) is given
     * then proceed but print a warning if there are 1 or less arguments given
     */
    if(argc > 1){
        name = argv[1];
        client.setNickName(name);
    }else{
        name = "this shit empty";
        client.setNickName(name);
        client.getUI()->QuitError("Not enough arguments");
    }
    // If there are 4 or more arguments given then start the first connection of the pc
    if (argc >= 4){
        client.firstConnect(argv[2], atoi(argv[3]));
    }
    // start the QT event loop (user input, network events, other events...)
    return a.exec();
}
