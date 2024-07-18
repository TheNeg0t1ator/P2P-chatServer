#ifndef JSON_H
#define JSON_H

#include "jsonparser.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

QString createJSON(QString id, QString ip, quint16 port, QString message) {

    QDateTime date = QDateTime::currentDateTime();
    QJsonObject jsonObj;
    jsonObj["id"] = id;
    jsonObj["ip"] = ip;
    jsonObj["port"] = port;
    jsonObj["timestamp"] = date.toString("dd/MM/yyyy hh:mm");
    jsonObj["message"] = message;

    QJsonDocument jsonDoc(jsonObj);
    return jsonDoc.toJson(QJsonDocument::Compact);
}

QString JSONtoMessage(QString Json){

    JsonParser Parser(Json);
    QString output;
    output = Parser.getId() +" "+ Parser.getTimestamp() +"\n"+ Parser.getMessage();
    return output;
}

QString JSONtoQString(QString Json){

    JsonParser Parser(Json);
    QString output;
    output = Parser.getId() + ", " + Parser.getIp() + ", " + Parser.getPort() + ", " + Parser.getTimestamp() + ", " + Parser.getMessage();
    return output;
}


#endif // JSON_H
