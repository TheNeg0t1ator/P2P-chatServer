#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <QWidget>

class TcpClient;

class Userinterface
{
public:
    Userinterface(TcpClient * client);
    QWidget* getWindow(void){return &window;};
    void QuitError(QString Error);
    ~Userinterface(){};
private:
    TcpClient * Client;
    QWidget window;
};

#endif // USERINTERFACE_H
