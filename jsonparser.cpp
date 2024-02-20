#include "jsonparser.h"
#include <QJsonDocument>
#include <QJsonObject>

/*
 * This is the constructor, it will initialize the id, ip, port, timestamp and message to empty strings in the beginning
 *
 * 1. Parse the Json string to a Json document and store it in the "jsonDoc" variable
 *
 * 2. If the json document isn't NULL and represents a JSON object then proceed with the parsing
 *
 * 3. Extract (convert) the Json document to a Json object
 *
 * 4. Now it will extract and stort the id, ip, port, timestamp and the message in the corresponding variables
 *
*/
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

// Getters for the variables
QString JsonParser::getId() const { return id; }
QString JsonParser::getIp() const { return ip; }
QString JsonParser::getPort() const { return port; }
QString JsonParser::getTimestamp() const { return timestamp; }
QString JsonParser::getMessage() const { return message; }



