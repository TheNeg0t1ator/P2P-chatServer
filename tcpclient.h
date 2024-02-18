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


class TcpClient : public QObject
{
    Q_OBJECT

public:
    explicit TcpClient(QObject *parent = nullptr, QString NickName = nullptr);
    ~TcpClient(){};
    std::string getPeers(void);
    void firstConnect(std::string firstIp, int firstPort);
    QString getNickName(void)   {return nickname;};
    QString getIP(void)         {if(!m_sockets.empty()){return m_sockets.first()->localAddress().toString();} else{return "";}};
    quint16 getPort(void)       {return server->serverPort();};
    void setNickName(QString NickName){nickname = NickName;};
    Userinterface* getUI(void){return &userInterface;};

signals:
    void sendMessage(QString message);
    void newMessageReceived(QString message);
    void newConnection(QTcpSocket *socket);

public slots:
    void handleNewConnection();
    void sendToAll(QString message);
    void readFromAll();

private:
    QList<QTcpSocket *> m_sockets;
    QTcpServer *server;
    Userinterface userInterface;

    QString nickname;
    QString port;
    QString ip;
};

#endif // TCPCLIENT_H
