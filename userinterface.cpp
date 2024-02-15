#include "userinterface.h"
#include "tcpclient.h"
#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
#include <QtWidgets>


Userinterface::Userinterface(TcpClient * client) : Client(client) {

    // Create main window
    window.setWindowTitle("P2P Chat");
    window.setFixedSize(800, 500);

    // Create widgets
    QLabel* outputLabel = new QLabel("Enter message:");
    QLineEdit* inputLineEdit = new QLineEdit();
    QPushButton* sendButton = new QPushButton("Send");
    QLabel* inputLabel = new QLabel("Received messages:");
    QPlainTextEdit* receivedTextEdit = new QPlainTextEdit();
    receivedTextEdit->setReadOnly(true);
    receivedTextEdit->setMaximumBlockCount(100);
    QLabel* debugLabel = new QLabel("Debug messages:");
    QPlainTextEdit* debugTextEdit = new QPlainTextEdit();
    debugTextEdit->setReadOnly(true);
    debugTextEdit->setMaximumHeight(110);
    debugTextEdit->setMaximumBlockCount(100);
    debugTextEdit->setStyleSheet("color: red;");

    // Create layout
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(outputLabel);
    layout->addWidget(inputLineEdit);
    layout->addWidget(sendButton);
    layout->addWidget(inputLabel);
    layout->addWidget(receivedTextEdit);
    layout->addWidget(debugLabel);
    layout->addWidget(debugTextEdit);

    window.setLayout(layout);



    QObject::connect(client, &TcpClient::newConnection, [&](QTcpSocket *socket)
                     {
                         qDebug() << "New connection from: " << socket->peerAddress().toString();
                         debugTextEdit->appendPlainText("New connection from: " + socket->peerAddress().toString());
                         QObject::connect(socket, &QTcpSocket::readyRead, client, &TcpClient::readFromAll);
                     });

    QObject::connect(client, &TcpClient::newMessageReceived, [&](QString message)
                     {
                         qDebug() << "New message received: " << message;
                         receivedTextEdit->appendPlainText(message);
                     });

    QObject::connect(sendButton, &QPushButton::clicked, [&]()
                     {
                         QString message = inputLineEdit->text();
                         if (message.isEmpty())
                         {
                             // Show message box with error
                             QMessageBox::warning(&window, "Error", "Message cannot be empty!");
                             return;
                         }
                         qDebug() << "Message sent: " << message;
                         debugTextEdit->appendPlainText("Message sent: " + message);
                         Client->sendToAll(message);
                         inputLineEdit->clear();
                     });

    // Show window
    window.show();

}