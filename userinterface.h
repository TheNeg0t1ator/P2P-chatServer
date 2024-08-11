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
    void saveFile();
    void loadFile();
    void exitApp();

private:
    void createMenus();

    TcpClient *Client;

    // New Menu Elements
    QMenu *fileMenu;
    QAction *saveAction;
    QAction *loadAction;
    QAction *exitAction;

    // Other UI elements go here, like text edit, etc.
};

#endif // USERINTERFACE_H

