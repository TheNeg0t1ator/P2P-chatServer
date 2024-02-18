#include "jsonparser.h"
#include <QJsonDocument>
#include <QJsonObject>

JsonParser::JsonParser(const QString& jsonString)
    : id(""), ip(""), port(""), timestamp(""), message("") {
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObj = jsonDoc.object();
        id = jsonObj.value("id").toString();
        ip = jsonObj.value("ip").toString();
        port = jsonObj.value("port").toString();
        timestamp = jsonObj.value("timestamp").toString();
        message = jsonObj.value("message").toString();
    }
}

QString JsonParser::getId() const { return id; }
QString JsonParser::getIp() const { return ip; }
QString JsonParser::getPort() const { return port; }
QString JsonParser::getTimestamp() const { return timestamp; }
QString JsonParser::getMessage() const { return message; }



