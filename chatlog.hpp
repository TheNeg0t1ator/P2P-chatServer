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


bool fileHandler::openFile() {
    if (!fileopen) {
        // Open the file in binary mode for writing
        file.open(fileName, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
        if (!file.is_open()) {
            // If the file doesn't exist, create it
            file.clear();
            file.open(fileName, std::ios::out | std::ios::binary);
            file.close();
            file.open(fileName, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
        }
        fileopen = file.is_open();
    }
    return fileopen;
}

bool fileHandler::appendToFile(const char *text) {
    if (openFile()) {
        
        file.seekp(0, std::ios::end);
        file.write(text, std::strlen(text));
        if (file.fail()) {
            std::cerr << "Error: Failed to write to file. Stream state: " 
                      << (file.rdstate() & std::ios::failbit ? "failbit set " : "")
                      << (file.rdstate() & std::ios::badbit ? "badbit set " : "")
                      << (file.rdstate() & std::ios::eofbit ? "eofbit set " : "")
                      << std::endl;
            file.clear();  // Clear error flags
            closeFile();   // Ensure file is closed
            return false;
        }
        
        file.flush();
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

    // Step 1: Read the existing JSON content from the file
    QJsonArray jsonArray;

    std::ifstream fileIn(fileName, std::ios::in | std::ios::binary);
    if (fileIn.is_open()) {
        std::string content((std::istreambuf_iterator<char>(fileIn)),
                             std::istreambuf_iterator<char>());
        fileIn.close();

        if (!content.empty()) {
            QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(content).toUtf8());
            if (doc.isArray()) {
                jsonArray = doc.array(); // Load the existing JSON array
            } else {
                std::cerr << "Error: The existing file content is not a valid JSON array." << std::endl;
                return false;
            }
        }
    }

    // Step 2: Add the new JSON message to the array
    QJsonDocument newDoc = QJsonDocument::fromJson(JSONmessage.toUtf8());
    if (!newDoc.isNull() && newDoc.isObject()) {
        jsonArray.append(newDoc.object());
    } else {
        std::cerr << "Error: The new JSON message is not a valid JSON object." << std::endl;
        return false;
    }

    // Step 3: Convert the JSON array to a string
    QJsonDocument updatedDoc(jsonArray);
    QByteArray jsonBytes = updatedDoc.toJson(QJsonDocument::Compact);
    std::string jsonString = jsonBytes.toStdString();

    // Step 4: Write the updated JSON array back to the file
    std::ofstream fileOut(fileName, std::ios::out | std::ios::trunc | std::ios::binary);
    if (fileOut.is_open()) {
        fileOut.write(jsonString.c_str(), jsonString.size());
        if (fileOut.fail()) {
            std::cerr << "Error: Failed to write JSON to file." << std::endl;
            fileOut.close();
            return false;
        }
        fileOut.close();
    } else {
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return false;
    }

    std::cout << "Successfully wrote JSON to file." << std::endl;
    return true;
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

