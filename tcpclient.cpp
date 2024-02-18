#include "tcpclient.h"
//#include "CreateJson.h"
#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <qtcpserver.h>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
#include <iostream>
#include <sstream>
#include <string>
#include <QtWidgets>

/*
 * constructor of the TcpClient class
 * 1. initializes the Nickname string with the nickname that was provided,
 *    as well as the QObject and the userInterfase
 *
 * 2. Creates a new Tcpserver class for... well the tcpserver functions of the p2p chat
 *
 * 3. Connects and listens
 *
 */
TcpClient::TcpClient(QObject *parent, QString NickName)
    : QObject(parent), userInterface(this), nickname(NickName)
{
    // Create the server socket and listen for connections
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
    server->listen(QHostAddress::Any, 24042);
}

/*
 * 1. Receive to the connection from the QTcpServer and store it in a QTcpSocket object
 *
 * 2. Receive the list of peers in string format
 *
 * 3. Send the list of peers the the new connection
 *
 * 4. If there is new data available from the new socket, the readFromAll slot will be said to handle it
 *
 * 5. Add the new socket to the m_sockets list
 *
 * 6. Send a newConnection signal with the socket as the parameter4
 *    this can be received in other parts of the program
 */
void TcpClient::handleNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    std::string peers = getPeers();
    socket->write(peers.c_str());
    connect(socket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
    m_sockets.append(socket);

    emit newConnection(socket);
}

/*
 * Iterate through all the sockets and send the "message" to all
 * the other pc's (connections)
 *
*/
void TcpClient::sendToAll(QString message)
{
    for (QTcpSocket *socket : m_sockets)
    {
        socket->write(message.toUtf8());
    }
}

/*
 * 1. Go through the entire list of sockets and check if there are newly received messages
 *
 * 2. Retrieve the message and send (emit) the message under the name: newMessageReceived
*/
void TcpClient::readFromAll()
{
    for (QTcpSocket *socket : m_sockets)
    {
        while (socket->bytesAvailable() > 0)
        {
            QString message = QString::fromUtf8(socket->readAll());
            emit newMessageReceived(message);
        }
    }
}

void TcpClient::firstConnect(std::string firstIp, int firstPort)
{

    QTcpSocket *firstSocket = new QTcpSocket(this);
    // Get me a damn socket! Grrrr
    firstSocket->connectToHost(firstIp.c_str(), firstPort);
    firstSocket->write("Hello there!");
    firstSocket->waitForBytesWritten(1000);
    firstSocket->waitForReadyRead(1000);

    if (firstSocket->waitForConnected())
    {
        connect(firstSocket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
        m_sockets.append(firstSocket);
        emit newConnection(firstSocket);
        qDebug() << "Connected to: " << firstIp.c_str() << ":" << firstSocket;

        std::string recv;
        recv = firstSocket->readAll();

        // Parse the received buffer filled with new IP and Ports.
        std::istringstream ss(recv);
        std::string token, newIP, newPort;
        getline(ss, token); // discard first line

        while (getline(ss, token))
        {
            std::string temp;
            std::stringstream tokenStream(token);
            getline(tokenStream, newIP, ':');
            getline(tokenStream, newPort);

            QTcpSocket *socket = new QTcpSocket(this);

            socket->connectToHost(newIP.c_str(), atoi(newPort.c_str()));
            socket->write("Hello there!");
            socket->waitForBytesWritten(1000);
            socket->waitForReadyRead(1000);

            temp = socket->readAll();
            // Are we connected?
            if (socket->state() == QAbstractSocket::ConnectedState)
            {
                std::cout << "Connected to " << newIP << ":" << newPort << " successfully!" << std::endl;
                connect(socket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
                m_sockets.append(socket);
                emit newConnection(socket);
            }
            else
            {
                std::cout << "Failed to connect to " << newIP << ":" << newPort << "." << std::endl;
            }
        }
    }
    else
    {
        qDebug() << "Failed to connect to: " << firstIp.c_str() << ":" << firstSocket;
    }
}

std::string TcpClient::getPeers(void)
{
    QString peerList = "NEWCON";
    peerList += '\n';
    for (QTcpSocket *socket : m_sockets)
    {
        if(socket->state()== QTcpSocket::ConnectedState)
        {
            QString address = QHostAddress(socket->peerAddress().toIPv4Address()).toString();
            peerList += address + ":" + "24042" + '\n';
        }
    }

    std::string temp = peerList.toStdString();
    return temp;
}
