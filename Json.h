#ifndef JSON_H
#define JSON_H

#include "jsonparser.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QString>

/*
 * 1. Create a QdateTime object and a json object
 *
 * 2. The json is constructed using the given data (ip, port, id, message, ...)
 *
 * 3. Create a JSON string from that json object and return it
 */
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

/*
 * 1. Create a json parser instance with the provided Json QString
 *
 * 2. Create a string containing the ID, Timestamp and the message parsed from the Json string
 *
 * 3. Return that string
*/
QString JSONtoMessage(QString Json){

    JsonParser Parser(Json);
    QString output;

    if(Parser.getId().isEmpty() || Parser.getTimestamp().isEmpty() || Parser.getMessage().isEmpty()){
        return "";
    }

    output = Parser.getId() +" "+ Parser.getTimestamp() +"\n"+ Parser.getMessage();
    return output;
}
#endif // JSON_H
