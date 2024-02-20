#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include "userinterface.h"
#include <string>

// This class inherits from the QObject class, this creates the possibility to use signal-slot communication in Qt
class TcpClient : public QObject
{
    Q_OBJECT

public:
    // Constructor and deconstructor
    explicit TcpClient(QObject *parent = nullptr, QString NickName = nullptr);
    ~TcpClient(){};
    void firstConnect(std::string firstIp, int firstPort);
    // Getters and setters
    std::string getPeers(void);
    QString getNickName(void)   {return nickname;};
    QString getIP(void)         {if(!m_sockets.empty()){return m_sockets.first()->localAddress().toString();} else{return "";}};
    quint16 getPort(void)       {return server->serverPort();};
    void setNickName(QString NickName){nickname = NickName;};
    Userinterface* getUI(void){return &userInterface;};

signals:
    // The different signal "channels" to send data and notify the right slot about certain events
    void sendMessage(QString message);
    void newMessageReceived(QString message);
    void newConnection(QTcpSocket *socket);

public slots:
    // Slots to handle the different signals and the data they bring
    void handleNewConnection();
    void sendToAll(QString message);
    void readFromAll();

private:
    // List for all the connections
    QList<QTcpSocket *> m_sockets;
    // Pointer to the TCP server instance
    QTcpServer *server;
    // Instance of the Userinterface class
    Userinterface userInterface;

    // Variables
    QString nickname;
    QString port;
    QString ip;
};

#endif // TCPCLIENT_H
