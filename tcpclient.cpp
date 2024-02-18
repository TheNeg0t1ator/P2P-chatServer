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
 * 1. Initializes the Nickname string with the nickname that was provided
 *
 * 2. Creates a new Tcpserver instance for... well the tcpserver functions of the p2p chat
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
 * 1. Receive a new connection from the QTcpServer and store it in a QTcpSocket object
 *
 * 2. Receive the list of peers in string format
 *
 * 3. Send the list of peers the the new connection
 *
 * 4. If there is new data available from the new socket, the readFromAll slot will handle it
 *
 * 5. Add the new socket to the m_sockets list
 *
 * 6. Send a newConnection signal with the socket as the parameter
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
 * 2. Retrieve the message and emit the newMessageReceived signal.
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
    /*
     * 1. Get me a damn socket! Grrrr
     *
     * 2. Connect to the "host" with the information that was provided as arguments to main
     *
     * 3. Write a "Hello there!" message to that connection
     *
     * 4. Wait for that message to be written (1 sec) and wait for the message to be send (1 sec)
     *
    */
    QTcpSocket *firstSocket = new QTcpSocket(this);

    firstSocket->connectToHost(firstIp.c_str(), firstPort);
    firstSocket->write("Hello there!");
    firstSocket->waitForBytesWritten(1000);
    firstSocket->waitForReadyRead(1000);

    // if the connection is successfull then...
    if (firstSocket->waitForConnected())
    {
        /* 1. Connect to readyRead signal of the firstSocket object, slot readFromAll
         *
         * 2. Add the host socket and emit the signal that there is a new socket with the socket
         *    itself as the parameter
        */
        connect(firstSocket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
        m_sockets.append(firstSocket);
        emit newConnection(firstSocket);
        qDebug() << "Connected to: " << firstIp.c_str() << ":" << firstSocket;

        // Receive the message from the host, this should be the list of the other ip's and ports


        std::string recv;
        recv = firstSocket->readAll();

        // Parse the received buffer filled with new IP and Ports.
        std::istringstream ss(recv);
        std::string token, newIP, newPort;
        // discard first line
        getline(ss, token);

        // And for each line in the string, do the following...
        while (getline(ss, token))
        {
            // Create a temp string and a stringstream token to keep track of the current character in the string
            std::string temp;
            std::stringstream tokenStream(token);
            // extract both the IP address and the port
            getline(tokenStream, newIP, ':');
            getline(tokenStream, newPort);

            /*
             * 1. Create a new socket
             *
             * 2. send message
             *
             * 3. Wait for the message to be written and send
            */
            QTcpSocket *socket = new QTcpSocket(this);

            socket->connectToHost(newIP.c_str(), atoi(newPort.c_str()));
            socket->write("Hello there!");
            socket->waitForBytesWritten(1000);
            socket->waitForReadyRead(1000);

            // Wait for a message from that socket aka. the "Hello there"
            temp = socket->readAll();
            /*
             * 1. Are we connected? If so then...
             *
             * 2. Connect to the readyRead signal from the Tcpclient, slot readFromAll
             *
             * 3. Append the socket to the socketlist and send the socket to the newConnection signal
            */
            if (socket->state() == QAbstractSocket::ConnectedState)
            {
                std::cout << "Connected to " << newIP << ":" << newPort << " successfully!" << std::endl;
                connect(socket, SIGNAL(readyRead()), this, SLOT(readFromAll()));
                m_sockets.append(socket);
                emit newConnection(socket);
            }
            // Otherwise a error will be printed
            else
            {
                qWarning() << "Failed to connect to " << newIP << ":" << newPort << ".";
            }
        }
    }
    else
    {
        qDebug() << "Failed to connect to: " << firstIp.c_str() << ":" << firstSocket;
    }
}

/*
 * 1. Initialize a new peerlist with the string "NEWCON"
 *
 * 2. Go through the list of sockets and check if they are connected
 *
 * 3. If they are then convert the IP address to a string format and add the port information to that string.
 *    Do this for every connection and add them together as a complete string
 *
 * 4. Place this string in the "temp" string and return that string
 *
*/
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
