#include "mainWindow.hpp"

void mainWindow::init(int argc, char *argv[]) {
    QString name;

    if (argc > 1) {
        name = argv[1];
        client.setNickName(name);
    } else {
        name = "this shit empty";
        client.setNickName(name);
    }
    if (argc >= 4) {
        client.firstConnect(argv[2], atoi(argv[3]));
    }
}