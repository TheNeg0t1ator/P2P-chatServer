#ifndef CHATLOG_HPP
#define CHATLOG_HPP
#include "Json.h"
#include <fstream>

class fileHandler {
public:
    fileHandler(){fileName = "Chat.log"; fileopen = false;};
    ~fileHandler();
    bool init(){
        if (!fileGood()){
            createFile();
            return true;
        }
        return false;
    };
    virtual void readFromFile();
    virtual bool appendJSON(const char* JSONmessage);
    void clearFile(){file.open(fileName, std::ios::out | std::ios::trunc); file.close();};
    void setFileName(const char* name){fileName = name;};
    const char* getFileName(){return fileName;};
protected:
    bool appendToFile(const char *text);

private:
    const char* fileName;
    bool fileopen;
    std::fstream file;
    bool fileGood(){return file.good();};
    void createFile(){file.open(fileName, std::ios::out); file.close();};
    void openFile(){file.open(fileName, std::ios::in | std::ios::out);};
    void closeFile(){file.close();};
    

};


bool fileHandler::appendToFile(const char *text){
    openFile();
    file.seekp(0, std::ios::end);
    file << text;
    closeFile();
    return true;
}

class CsvFileHandler : public fileHandler {
public:
    CsvFileHandler() {setFileName("log.csv");};
    ~CsvFileHandler();
    void readFromFile();
    bool appendJSON(const char* JSONmessage);

};

bool CsvFileHandler::appendJSON(const char* JSONmessage){
    QString AppendCSV; JSONtoQString(JSONmessage);
    return appendToFile(AppendCSV.toStdString().c_str());
}


#endif
