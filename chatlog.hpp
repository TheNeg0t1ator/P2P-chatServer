// chatlog.hpp
#ifndef CHATLOG_HPP
#define CHATLOG_HPP
#include "Json.h"
#include <fstream>
#include <vector>
#include <QJsonArray>

enum FileType {
    CSV = 1,
    TXT = 2,
    JSON = 3,
    XML = 4
};

class fileHandler {
public:
    fileHandler() : fileName("Chat.log"), fileopen(false) {}
    virtual ~fileHandler(){}

    bool init(){
        if (!fileGood()){
            createFile();
            return true;
        }
        return false;
    }

    virtual std::vector<QString> readFromFile() { return {}; }
    virtual bool appendJSON(const QString&) { return false; }

    void clearFile();

    void setFileName(const char* name){
        fileName = name;
    }

    const char* getFileName(){
        return fileName;
    }

    bool appendToFile(const char *text);

protected:
    const char* fileName;
    bool fileopen;
    std::fstream file;

    bool fileGood(){
        file.open(fileName, std::ios::in);
        bool good = file.good();
        file.close();
        return good;
    }

    void createFile(){
        file.open(fileName, std::ios::out);
        file.close();
    }

    bool openFile();
    void closeFile(){
        if(file.is_open()) file.close();
        fileopen = false;
    }
};

void fileHandler::clearFile() {
    std::ofstream file(getFileName(), std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        std::cout << "File cleared successfully." << std::endl;
        file.close();
    } else {
        std::cout << "Error: Unable to clear file." << std::endl;
    }
}


bool fileHandler::openFile(){
    if (!fileopen)
    {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::app);
        if (!file.good()) {
            file.clear();
            file.open(fileName, std::ios::out);
        }
        fileopen = true;
    }
    return fileopen;
}

bool fileHandler::appendToFile(const char *text) {
    if (openFile()) {
        std::cout << __func__ << ": Adding to file: " << text << std::endl;
        
        file.seekp(0, std::ios::end);  // Move to the end of file
        file << text;
        
        if (file.fail()) {
            std::cerr << "Error: Failed to write to file. Stream state: " 
                      << (file.rdstate() & std::ios::failbit ? "failbit set " : "")
                      << (file.rdstate() & std::ios::badbit ? "badbit set " : "")
                      << (file.rdstate() & std::ios::eofbit ? "eofbit set " : "")
                      << (file.rdstate() & std::ios::goodbit ? "goodbit set " : "")
                      << std::endl;
            file.clear();  // Clear error flags
            return false;
        }
        
        file.flush();  // Ensure data is flushed to disk
        closeFile();
        return true;
    } else {
        std::cerr << "Error: Unable to open file." << std::endl;
        return false;
    }
}

class CsvFileHandler : public fileHandler {
public:
    CsvFileHandler() { setFileName("log.csv"); }
    virtual ~CsvFileHandler() {}

    std::vector<QString> readFromFile() override { return {}; }
    bool appendJSON(const QString& JSONmessage) override {
        QString AppendCSV = JSONtoQString(JSONmessage);  // Assuming JSONtoQString converts JSON to CSV string
        return appendToFile(AppendCSV.toStdString().c_str());
    }
};

class TxtFileHandler : public fileHandler {
public:
    TxtFileHandler() { setFileName("log.txt"); }
    virtual ~TxtFileHandler() {}

    std::vector<QString> readFromFile() override { return {}; }
    bool appendJSON(const QString& JSONmessage) override {
        QString AppendTXT = JSONtoTXT(JSONmessage);  // Assuming JSONtoTXT converts JSON to TXT string
        return appendToFile(AppendTXT.toStdString().c_str());
    }
};

class JsonFileHandler : public fileHandler {
public:
    JsonFileHandler() { setFileName("log.json"); }
    virtual ~JsonFileHandler() {}

    void readBackFile();
    std::vector<QString> readFromFile() override;
    bool appendJSON(const QString& JSONmessage) override;
};


void JsonFileHandler::readBackFile() {
    std::ifstream file(getFileName(), std::ios::in | std::ios::binary);
    if (file.is_open()) {
        std::string fileContent((std::istreambuf_iterator<char>(file)),
                                 std::istreambuf_iterator<char>());
        std::cout << "Content of the file after writing: " << fileContent << std::endl;
        file.close();
    } else {
        std::cout << "Error: Unable to open file for reading." << std::endl;
    }
}

std::vector<QString> JsonFileHandler::readFromFile() {
    std::vector<QString> messages;
    if (openFile()) {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(QString::fromStdString(content).toUtf8());
        if (!jsonDoc.isNull() && jsonDoc.isArray()) {
            QJsonArray jsonArray = jsonDoc.array();
            for (const QJsonValue& value : jsonArray) {
                messages.push_back(QString(QJsonDocument(value.toObject()).toJson(QJsonDocument::Compact)));
            }
        }
    }
    return messages;
}

bool JsonFileHandler::appendJSON(const QString& JSONmessage) {
    std::cout << "Appending JSON: " << JSONmessage.toStdString() << std::endl;

    std::vector<QString> messages = readFromFile();
    messages.push_back(JSONmessage);

    QJsonArray jsonArray;
    for (const QString& msg : messages) {
        QJsonDocument doc = QJsonDocument::fromJson(msg.toUtf8());
        if (!doc.isNull()) {
            jsonArray.append(doc.object());
        } else {
            std::cout << "Error: Failed to parse message: " << msg.toStdString() << std::endl;
        }
    }

    QJsonDocument newDoc(jsonArray);
    QByteArray jsonBytes = newDoc.toJson(QJsonDocument::Compact);
    std::string jsonString = jsonBytes.toStdString();

    std::cout << "Final JSON to write: " << jsonString << std::endl;

    if (openFile()) {
        std::cout << "File opened successfully before writing." << std::endl;
    } else {
        std::cout << "Error: Failed to open file before writing." << std::endl;
        return false;
    }

    if (appendToFile(jsonString.c_str())) {
        std::cout << "Successfully wrote JSON to file." << std::endl;
        // Read back and display content
        readBackFile();
        return true;
    } else {
        std::cout << "Error: Failed to write JSON to file." << std::endl;
        return false;
    }
}


class logFileHandler {
public:
    logFileHandler(enum FileType type = CSV) {
        fileType = type;
        switch (fileType) {
        case CSV:
            file = new CsvFileHandler();
            break;
        case TXT:
            file = new TxtFileHandler();
            break;
        case JSON:
            file = new JsonFileHandler();
            break;
        default:
            file = nullptr;
            break;
        }
    }
    ~logFileHandler() {
        delete file;
    }
    bool init(){
        return file->init();
    }
    void setFileName(const char* name){
        file->setFileName(name);
    }
    bool appendJSON(const QString& JSONmessage){
        return file->appendJSON(JSONmessage);
    }
    std::vector<QString> readFromFile(){
        return file->readFromFile();
    }
    fileHandler* getFileHandler() { return file; }

private:
    enum FileType fileType;
    fileHandler* file;
};

#endif

