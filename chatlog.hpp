#ifndef CHATLOG_HPP
#define CHATLOG_HPP
#include "Json.h"
#include <fstream>
enum FileType {
    CSV = 1,
    TXT = 2,
    JSON = 3,
    XML = 4
};
class fileHandler {
public:
    fileHandler(){fileName = "Chat.log"; fileopen = false;}
    virtual ~fileHandler(){}  // Ensure the destructor is virtual
    bool init(){
        std::cout << __func__ << " filename: " << fileName << std::endl;
        if (!fileGood()){
            createFile();
            return true;
        }
        return false;
    }
    virtual void readFromFile() {}  // Provide default empty implementation
    virtual bool appendJSON(const char* JSONmessage) { return false; }  // Provide default implementation
        void clearFile(){file.open(fileName, std::ios::out | std::ios::trunc); file.close();}
        void setFileName(const char* name){fileName = name; std::cout << __func__ << " filename: " << fileName << std::endl;}
        const char* getFileName(){return fileName;}
        bool appendToFile(const char *text);
    protected:


    private:
        const char* fileName;
        bool fileopen;
        std::fstream file;
        bool fileGood(){return file.good();}
        void createFile(){std::cout << __func__ << " filename: " << fileName << std::endl; file.open(fileName, std::ios::out); file.close();}
        bool openFile();
        void closeFile(){if(file.is_open()) file.close(); fileopen = false;}
};

bool fileHandler::openFile(){
    std::cout << __func__ << " filename: " << fileName << std::endl;
    if (!fileopen)
    {
        if(fileName != nullptr){
            if (fileName[0] != '\0'){
                file.open(fileName, std::ios::in | std::ios::out | std::ios::app);  // Open the file in append mode
                if (!file.good()) {
                    file.clear();  // Clear any error flags
                    file.open(fileName, std::ios::out);  // Create the file if it doesn't exist
                }
                fileopen = true;
            }
        }
    }
    return fileopen;
}


bool fileHandler::appendToFile(const char *text){
    if(openFile()){
        std::cout << __func__ << ": adding to file: " << text << std::endl;
        file.seekp(0, std::ios::end);
        file << text;
        closeFile();
        return true;
    }
    return false;
}

class CsvFileHandler : public fileHandler {
public:
    CsvFileHandler() {setFileName("log.csv");}
    virtual ~CsvFileHandler() {}  // Ensure the destructor is virtual
    void readFromFile() override {}  // Correctly override the base class method
    bool appendJSON(const char* JSONmessage) override;  // Correctly override the base class method
};

bool CsvFileHandler::appendJSON(const char* JSONmessage){
    bool status;
    std::cout << __func__ <<"parsing JSON" << std::endl;

    QString AppendCSV = JSONtoQString(JSONmessage);  // Assuming JSONtoQString converts JSON to CSV string
    
    std::cout << __func__ <<"appending to file" << std::endl;
    
    status = appendToFile(AppendCSV.toStdString().c_str());
    status = appendToFile(",\n");
    return status;
}

class TxtFileHandler : public fileHandler {
public:
    TxtFileHandler() {setFileName("log.txt");}
    virtual ~TxtFileHandler() {}  // Ensure the destructor is virtual
    void readFromFile() override {}  // Correctly override the base class method
    bool appendJSON(const char* JSONmessage) override;  // Correctly override the base class method
};

bool TxtFileHandler::appendJSON(const char* JSONmessage){
    std::cout << __func__ <<"parsing JSON" << std::endl;
    QString AppendCSV = JSONtoTXT(JSONmessage);  // Assuming JSONtoQString converts JSON to CSV string
    AppendCSV.append("\n");
    std::cout << __func__ <<"appending to file" << std::endl;
    return appendToFile(AppendCSV.toStdString().c_str());
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
            // case JSON:
            //     file = new JsonFileHandler();
            //     break;
            // case XML:
            //     file = new XmlFileHandler();
            //     break;
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
    bool appendJSON(const char* JSONmessage){
        return file->appendJSON(JSONmessage);
    }
    fileHandler* getFileHandler() { return file; }
    
private:
    enum FileType fileType;
    fileHandler* file;
};

#endif

