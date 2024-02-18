#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>

class JsonParser {
public:
    JsonParser(const QString& jsonString);
    ~JsonParser(){};
    QString getId() const;
    QString getIp() const;
    QString getPort() const;
    QString getTimestamp() const;
    QString getMessage() const;

private:
    QString id;
    QString ip;
    QString port;
    QString timestamp;
    QString message;
};

#endif // JSONPARSER_H
