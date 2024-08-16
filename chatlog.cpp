// chatlog.cpp
#include "chatlog.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

// fileHandler methods
fileHandler::fileHandler() : fileName("Chat.log"), fileopen(false) {}

fileHandler::~fileHandler() {}

bool fileHandler::init() {
    if (!fileGood()) {
        createFile();
        return true;
    }
    return false;
}

std::vector<QString> fileHandler::readFromFile() {
    return {};
}

bool fileHandler::appendJSON(const QString&) {
    return false;
}

void fileHandler::clearFile() {
    std::ofstream file(getFileName(), std::ios::out | std::ios::trunc);
    if (file.is_open()) {
        std::cout << "File cleared successfully." << std::endl;
        file.close();
    } else {
        std::cout << "Error: Unable to clear file." << std::endl;
    }
}

void fileHandler::setFileName(const char* name) {
    fileName = name;
}

const char* fileHandler::getFileName() {
    return fileName;
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

bool fileHandler::fileGood() {
    file.open(fileName, std::ios::in);
    bool good = file.good();
    file.close();
    return good;
}

void fileHandler::createFile() {
    file.open(fileName, std::ios::out);
    file.close();
}

bool fileHandler::openFile() {
    if (!fileopen) {
        file.open(fileName, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
        if (!file.is_open()) {
            file.clear();
            file.open(fileName, std::ios::out | std::ios::binary);
            file.close();
            file.open(fileName, std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
        }
        fileopen = file.is_open();
    }
    return fileopen;
}

void fileHandler::closeFile() {
    if (file.is_open()) file.close();
    fileopen = false;
}

// CsvFileHandler methods
CsvFileHandler::CsvFileHandler() {
    setFileName("log.csv");
}

CsvFileHandler::~CsvFileHandler() {}

std::vector<QString> CsvFileHandler::readFromFile() {
    return {};
}

bool CsvFileHandler::appendJSON(const QString& JSONmessage) {
    QString AppendCSV = JSONtoQString(JSONmessage);
    return appendToFile(AppendCSV.toStdString().c_str());
}

// TxtFileHandler methods
TxtFileHandler::TxtFileHandler() {
    setFileName("log.txt");
}

TxtFileHandler::~TxtFileHandler() {}

std::vector<QString> TxtFileHandler::readFromFile() {
    return {};
}

bool TxtFileHandler::appendJSON(const QString& JSONmessage) {
    QString AppendTXT = JSONtoTXT(JSONmessage);
    return appendToFile(AppendTXT.toStdString().c_str());
}

// JsonFileHandler methods
JsonFileHandler::JsonFileHandler() {
    setFileName("log.json");
}

JsonFileHandler::~JsonFileHandler() {}

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

    QJsonArray jsonArray;

    std::ifstream fileIn(fileName, std::ios::in | std::ios::binary);
    if (fileIn.is_open()) {
        std::string content((std::istreambuf_iterator<char>(fileIn)),
                             std::istreambuf_iterator<char>());
        fileIn.close();

        if (!content.empty()) {
            QJsonDocument doc = QJsonDocument::fromJson(QString::fromStdString(content).toUtf8());
            if (doc.isArray()) {
                jsonArray = doc.array();
            } else {
                std::cerr << "Error: The existing file content is not a valid JSON array." << std::endl;
                return false;
            }
        }
    }

    QJsonDocument newDoc = QJsonDocument::fromJson(JSONmessage.toUtf8());
    if (!newDoc.isNull() && newDoc.isObject()) {
        jsonArray.append(newDoc.object());
    } else {
        std::cerr << "Error: The new JSON message is not a valid JSON object." << std::endl;
        return false;
    }

    QJsonDocument updatedDoc(jsonArray);
    QByteArray jsonBytes = updatedDoc.toJson(QJsonDocument::Indented);
    std::string jsonString = jsonBytes.toStdString();

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

// logFileHandler methods
logFileHandler::logFileHandler(enum FileType type) {
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

logFileHandler::~logFileHandler() {
    delete file;
}

bool logFileHandler::init() {
    return file->init();
}

void logFileHandler::setFileName(const char* name) {
    file->setFileName(name);
}

bool logFileHandler::appendJSON(const QString& JSONmessage) {
    return file->appendJSON(JSONmessage);
}

std::vector<QString> logFileHandler::readFromFile() {
    return file->readFromFile();
}

fileHandler* logFileHandler::getFileHandler() {
    return file;
}

void logFileHandler::loadMessages(QTextEdit* receivedTextEdit) {
    std::vector<QString> messages = readFromFile();
    for (const QString& message : messages) {
        receivedTextEdit->setMarkdown(receivedTextEdit->toMarkdown() + "\n" + JSONtoMessage(message));
    }
}
