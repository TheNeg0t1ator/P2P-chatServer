#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include <QMainWindow>

class TcpClient;

class Userinterface : public QMainWindow
{
    Q_OBJECT

public:
    Userinterface(TcpClient *client);
    ~Userinterface(){};

private slots:
    void loadFile();
    void exitApp();

private:
    void createMenus();

    TcpClient *Client;

    // Menu Elements
    QMenu *fileMenu;
    QAction *loadAction;
    QAction *exitAction;

    // Other UI elements go here, like text edit, etc.
};

#endif // USERINTERFACE_H


