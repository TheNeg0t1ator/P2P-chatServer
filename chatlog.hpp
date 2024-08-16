// chatlog.hpp
#ifndef CHATLOG_HPP
#define CHATLOG_HPP

#include "Json.h"
#include <fstream>
#include <vector>
#include <QJsonArray>
#include <QTextEdit>

enum FileType {
    CSV = 1,
    TXT = 2,
    JSON = 3,
    XML = 4
};

class fileHandler {
public:
    fileHandler();
    virtual ~fileHandler();

    bool init();

    virtual std::vector<QString> readFromFile();
    virtual bool appendJSON(const QString&);

    void clearFile();

    void setFileName(const char* name);
    const char* getFileName();

    bool appendToFile(const char *text);

protected:
    const char* fileName;
    bool fileopen;
    std::fstream file;

    bool fileGood();
    void createFile();
    bool openFile();
    void closeFile();
};

class CsvFileHandler : public fileHandler {
public:
    CsvFileHandler();
    virtual ~CsvFileHandler();

    std::vector<QString> readFromFile() override;
    bool appendJSON(const QString& JSONmessage) override;
};

class TxtFileHandler : public fileHandler {
public:
    TxtFileHandler();
    virtual ~TxtFileHandler();

    std::vector<QString> readFromFile() override;
    bool appendJSON(const QString& JSONmessage) override;
};

class JsonFileHandler : public fileHandler {
public:
    JsonFileHandler();
    virtual ~JsonFileHandler();

    void readBackFile();
    std::vector<QString> readFromFile() override;
    bool appendJSON(const QString& JSONmessage) override;
};

class logFileHandler {
public:
    logFileHandler(enum FileType type = CSV);
    ~logFileHandler();

    bool init();
    void setFileName(const char* name);
    bool appendJSON(const QString& JSONmessage);
    std::vector<QString> readFromFile();
    fileHandler* getFileHandler();
    void loadMessages(QTextEdit* receivedTextEdit);

private:
    enum FileType fileType;
    fileHandler* file;
};

#endif // CHATLOG_HPP
