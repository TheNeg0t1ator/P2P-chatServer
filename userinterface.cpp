#include "userinterface.h"
#include "Json.h"
#include "tcpclient.h"
#include "chatlog.hpp"  // Include your fileHandler class
#include <QCoreApplication>
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QInputDialog>
#include <QApplication>
#include <QtWidgets>

Userinterface::Userinterface(TcpClient * client) : Client(client) {
    // Instantiate your fileHandler
    logFileHandler * logHandler = new logFileHandler(TXT);
    logHandler->getFileHandler()->init();  // Initialize log filetest
    logHandler->getFileHandler()->setFileName("C:/Users/kobed/Desktop/p2p/test.txt");

    // Create main window
    window.setWindowTitle("P2P Chat");
    //window.setFixedSize(800, 500);

    // Create widgets
    QLabel* outputLabel = new QLabel("Enter message:");
    QLineEdit* inputLineEdit = new QLineEdit();
    QPushButton* sendButton = new QPushButton("Send");
    QLabel* inputLabel = new QLabel("Chat:");
    QTextEdit* receivedTextEdit = new QTextEdit();
    receivedTextEdit->setReadOnly(true);
    receivedTextEdit->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
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



    QObject::connect(client, &TcpClient::newConnection, [client, debugTextEdit](QTcpSocket *socket)
                     {
                         qDebug() << "New connection from: " << socket->peerAddress().toString();
                         debugTextEdit->appendPlainText("New connection from: " + socket->peerAddress().toString());
                         QObject::connect(socket, &QTcpSocket::readyRead, client, &TcpClient::readFromAll);
                     });

    QObject::connect(client, &TcpClient::newMessageReceived, [this, logHandler, receivedTextEdit](QString message)
                     {
                         qDebug() << "New message received: " << message;
                         receivedTextEdit->setMarkdown(receivedTextEdit->toMarkdown() + "\n" + JSONtoMessage(message));
                         // Log the received message
                         logHandler->appendJSON(message.toStdString().c_str());
                     });


    auto messageProcessingFunc = [this, logHandler, debugTextEdit, receivedTextEdit, inputLineEdit]
    {
        QString message = inputLineEdit->text();
        if (message.isEmpty())
        {
            // Show message box with error
            QMessageBox::warning(&window, "Error", "Message cannot be empty!");
            return;
        }

        //intercept for json
        message = createJSON(Client->getNickName(), Client->getIP(), Client->getPort(), message);
        receivedTextEdit->setMarkdown(receivedTextEdit->toMarkdown() + "\n" + JSONtoMessage(message));
        qDebug() << "Message sent: " << message;
        debugTextEdit->appendPlainText("Message sent: " + message);

        // Store the std::string in a variable to extend its lifetime
        std::string messageStr = message.toStdString();
        const char * MessageToLog = messageStr.c_str();

        // Log the sent message
        //logHandler.appendJSON(MessageToLog);
        QString AppendCSV = JSONtoQString(MessageToLog);

        messageStr = AppendCSV.toStdString();
        const char * log = messageStr.c_str();

        std::cout << __func__ << " filename: " << logHandler->getFileHandler()->getFileName() << std::endl;
        if(!logHandler->appendJSON(message.toStdString().c_str())){
            debugTextEdit->appendPlainText("Error writing to log file");
        }
        Client->sendToAll(message);
        inputLineEdit->clear();
    };

    QObject::connect(sendButton, &QPushButton::clicked, messageProcessingFunc);
    QObject::connect(inputLineEdit, &QLineEdit::returnPressed, messageProcessingFunc);
    // Show window
    window.show();

}

