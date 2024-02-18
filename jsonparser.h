#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>

class JsonParser {
public:
    // Json Parser constructor and deconstructor
    JsonParser(const QString& jsonString);
    ~JsonParser(){};
    // Getters for the different variables
    QString getId() const;
    QString getIp() const;
    QString getPort() const;
    QString getTimestamp() const;
    QString getMessage() const;

private:
    // The different variables
    QString id;
    QString ip;
    QString port;
    QString timestamp;
    QString message;
};

#endif // JSONPARSER_H
