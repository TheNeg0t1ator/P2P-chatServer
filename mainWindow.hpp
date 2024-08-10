#ifndef mainWindow_HPP
#define mainWindow_HPP

// #include <QMainWindow>
// #include <QTextEdit>

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


class mainWindow
{
public:
    mainWindow(int argc, char *argv[]) : application(argc, argv) { init(argc, argv); }
    ~mainWindow() {}
    void init(int argc, char *argv[]);
    int run() { return application.exec(); }

private:
    QApplication application;
    TcpClient client;
};



void mainWindow::init(int argc, char *argv[]) {
    QString name;

    if (argc > 1) {
        name = argv[1];
        client.setNickName(name);
    } else {
        name = "this shit empty";
        client.setNickName(name);
    }
    if (argc >= 4) {
        client.firstConnect(argv[2], atoi(argv[3]));
    }
}

#endif
