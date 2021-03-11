#include <filesystem>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include <vector>
#include "MyProcesses.h"
#include <string>

void MyProcesses::getProcesses(){
    std::string path = "/proc";
    std::vector<std::string> process;
    for (const auto & entry : std::filesystem::directory_iterator(path))
        if(isNumber(entry.path()))
        {
            process.push_back(m_PID);
            m_PID = "";
            m_processes.push_back(process);
            process.clear();
        }
}

bool MyProcesses::isNumber(std::string str) {
    size_t sizeOfStr;
    char a;
    sizeOfStr = str.size()-1;
    a = str[sizeOfStr]; //a on last position

    while(a != '/')
    {
        if(a > 47 && a < 58)
        {
            m_PID = m_PID+a;
            a = str[--sizeOfStr];
        }
        else{
            m_PID = "";
            return false;
        }
    }
    std::reverse(m_PID.begin(),m_PID.end());
    return true;
}

void MyProcesses::printProcesses() {
    std::cout << std::setw(6) << "PID" << std::setw(13)<< "PPID" << std::setw(20) <<"STATE" << std::setw(18) << "UID" << std::setw(26)<< "wchan" << std::setw(40)<< "Name" << "\n";
    for (auto & process : m_processes) {
        std::cout << std::setw(6) << process[0] << std::setw(13) << process[1] << std::setw(26) << process[2] << std::setw(13) << process[3]<< std::setw(35) << process[4] << std::setw(40) << process[5] <<"\n";
    }
}
//test
void MyProcesses::loadProcData() {
    std::string path = "/proc";
    std::ifstream file;
    std::string line;
    std::string fullPath;
    for(auto & process : m_processes)
    {
        fullPath = path+"/"+process[0]+"/status";
        loadLineToProcess(process,fullPath,7);

        fullPath = path+"/"+process[0]+"/status";
        loadLineToProcess(process,fullPath,3);

        fullPath = path+"/"+process[0]+"/loginuid";
        loadLineToProcess(process,fullPath);

        fullPath = path+"/"+process[0]+"/wchan";
        loadLineToProcess(process,fullPath);

        fullPath = path+"/"+process[0]+"/comm";
        loadLineToProcess(process,fullPath);
    }
}

void MyProcesses::loadLineToProcess(std::vector<std::string> &process, const std::string &fullPath, size_t lineNum) {
    std::string line;
    std::ifstream file;

    file.open(fullPath);
    for (size_t i = 0; i < lineNum; ++i) {
        getline(file, line);
    }
    process.push_back(line);
    file.close();
}

void MyProcesses::loadLineToProcess(std::vector<std::string>& process,const std::string& fullPath) {
    std::string line;
    std::ifstream file;

    file.open(fullPath);
    getline(file, line);
    process.push_back(line);
    file.close();
}
