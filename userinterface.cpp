#include "userinterface.h"
#include "Json.h"
#include "tcpclient.h"
#include "chatlog.hpp"  // Include your fileHandler class
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QApplication>
#include <QtWidgets>

Userinterface::Userinterface(TcpClient *client) : QMainWindow(), Client(client) {
    // Instantiate your fileHandler

    logFileHandler * logHandler = new logFileHandler(JSON);

    logHandler->getFileHandler()->init();  // Initialize log filetest
    logHandler->getFileHandler()->setFileName("C:/temp/test.json");

    // Create main window
    setWindowTitle("P2P Chat");
    //setFixedSize(800, 500);

    // Create widgets
    QLabel *outputLabel = new QLabel("Enter message:");
    QLineEdit *inputLineEdit = new QLineEdit();
    QPushButton *sendButton = new QPushButton("Send");
    QLabel *inputLabel = new QLabel("Chat:");
    QTextEdit *receivedTextEdit = new QTextEdit();
    receivedTextEdit->setReadOnly(true);
    receivedTextEdit->setTextInteractionFlags(Qt::TextBrowserInteraction | Qt::TextSelectableByMouse | Qt::LinksAccessibleByMouse);
    QLabel *debugLabel = new QLabel("Debug messages:");
    QPlainTextEdit *debugTextEdit = new QPlainTextEdit();
    debugTextEdit->setReadOnly(true);
    debugTextEdit->setMaximumHeight(110);
    debugTextEdit->setMaximumBlockCount(100);
    debugTextEdit->setStyleSheet("color: red;");

    // Create layout and central widget
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(outputLabel);
    layout->addWidget(inputLineEdit);
    layout->addWidget(sendButton);
    layout->addWidget(inputLabel);
    layout->addWidget(receivedTextEdit);
    layout->addWidget(debugLabel);
    layout->addWidget(debugTextEdit);
    setCentralWidget(centralWidget);


    window()->setLayout(layout);

    // Load existing messages from the log file and display them
    logHandler->loadMessages(receivedTextEdit);

    // Create menu bar
    createMenus();


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
            QMessageBox::warning(this, "Error", "Message cannot be empty!");
            return;
        }

        // Intercept for JSON
        message = createJSON(Client->getNickName(), Client->getIP(), Client->getPort(), message);
        receivedTextEdit->setMarkdown(receivedTextEdit->toMarkdown() + "\n" + JSONtoMessage(message));
        qDebug() << "Message sent: " << message;
        debugTextEdit->appendPlainText("Message sent: " + message);

        // Store the std::string in a variable to extend its lifetime
        std::string messageStr = message.toStdString();
        const char *MessageToLog = messageStr.c_str();

        // Log the sent message
        QString AppendCSV = JSONtoQString(MessageToLog);


        messageStr = AppendCSV.toStdString();
        const char *log = messageStr.c_str();

        std::cout << __func__ << " filename: " << logHandler->getFileHandler()->getFileName() << std::endl;
        if (!logHandler->appendJSON(message.toStdString().c_str()))
        {
            debugTextEdit->appendPlainText("Error writing to log file");
        }
        Client->sendToAll(message);
        inputLineEdit->clear();
    };

    QObject::connect(sendButton, &QPushButton::clicked, messageProcessingFunc);
    QObject::connect(inputLineEdit, &QLineEdit::returnPressed, messageProcessingFunc);
    // Show window
    show();
}

void Userinterface::createMenus() {
    // Create file menu
    fileMenu = menuBar()->addMenu("&File");

    // Create actions
    loadAction = new QAction("&Load", this);
    exitAction = new QAction("&Exit", this);

    // Add actions to the file menu
    fileMenu->addAction(loadAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // Connect actions to slots
    connect(loadAction, &QAction::triggered, this, &Userinterface::loadFile);
    connect(exitAction, &QAction::triggered, this, &Userinterface::exitApp);
}

void Userinterface::loadFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "JSON Files (*.json);;CSV Files (*.csv)");
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot load file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString fileContent = in.readAll();
    file.close();

    // You can do something with the content here, e.g., display it
    // receivedTextEdit->setText(fileContent);
}

void Userinterface::exitApp() {
    QApplication::quit();
}


