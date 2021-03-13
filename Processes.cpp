#include <filesystem>
#include <iostream>
#include <algorithm>
#include <unistd.h>
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
    loadProcData();
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

void MyProcesses::printProcesses(int index) {
    std::cout << std::setw(6) << "PID" << std::setw(13)<< "PPID" << std::setw(20) <<"STATE" << std::setw(18) << "UID" << std::setw(26)<< "wchan" << std::setw(40)<< "Name" << "\n";
    for (auto & process : m_MP.processes[index]) {
        std::cout << std::setw(6) << process[0] << std::setw(13) << process[1] << std::setw(26) << process[2] << std::setw(13) << process[3]<< std::setw(35) << process[4] << std::setw(40) << process[5] <<"\n";
    }
    std::cout << "\n\n\n\n\n";
}

void MyProcesses::printConnections() {
    std::cout << std::setw(9) << "Protocol" << std::setw(9) << "sl"<< std::setw(22) << "local addr" << std::setw(22) << "remote adr"<< std::setw(5) << "st" << std::setw(7) << "UID\n\n";
    for (auto & m_networkConnection : m_networkConnections) {
        std::cout << std::setw(9)<< m_networkConnection[0];
        std::cout << std::setw(9)<< m_networkConnection[1];
        std::cout << std::setw(22)<< m_networkConnection[2];
        std::cout << std::setw(22)<< m_networkConnection[3];
        std::cout << std::setw(5)<< m_networkConnection[4];
        std::cout << std::setw(7)<< m_networkConnection[8];
        std::cout << "\n";
    }
}

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
void MyProcesses::loadFileToVector(const std::string& fullPath,std::vector<std::vector<std::string>>& matrix) {

    std::string line;
    std::ifstream infile(fullPath);
    std::vector<std::string> row;
    int counter = 0;

    while (std::getline(infile, line)) { //bude treba refactoring
        std::string word;
        std::istringstream tmp(line);
        while (counter < line.size()) {
            getline( tmp, word, ' ');
            if(!word.empty())
            {
                row.push_back(word);
                counter += word.size();
            } else counter++;
        }
        counter = 0;
        matrix.push_back(row);
        row.clear();
    }
}

void MyProcesses::getNetworkConn() {
    std::string path = "/proc/net";
    std::string fullPath;
    std::vector<std::vector<std::string> > matrix;

    fullPath = path + "/tcp";
    loadFileToVector(fullPath,matrix);
    formatUdpTcpTable(matrix,"TCP:");
    m_networkConnections = matrix;
    matrix.clear();

    fullPath = path + "/udp";
    loadFileToVector(fullPath,matrix);
    formatUdpTcpTable(matrix, "UDP:");
    for (auto & i : matrix) {
        m_networkConnections.push_back(i);
    }
    matrix.clear();

}

void MyProcesses::formatUdpTcpTable(std::vector<std::vector<std::string> >& matrix, const std::string& type) {  //nieco zle pre UDP
    std::string result;
    std::string hexa;
    matrix.erase(matrix.begin());
    std::string tmp;
    for (auto & j : matrix) {
        for (int k = 1; k < 3; ++k) {
            for (int i = 0; i < 13; i += 2) {
                if (i == 8) {
                    i--;
                    continue;
                }
                if(i == 9)
                    hexa = j[k].substr(i,4);
                else
                    hexa = j[k].substr(i,2);

                result = std::to_string(std::stol(hexa, nullptr, 16));
                if (i < 8) {
                    tmp.insert(0, result);
                    if (i < 6)
                        tmp.insert(0, ".");
                } else {
                    if (i == 9)
                    {
                        tmp.append(":");
                        i+=2;
                    }

                    tmp.append(result);
                }
            }
            j[k] = tmp;
            tmp = "";
        }
        j.insert(j.begin(), type);
    }
}

void MyProcesses::fileCopy(const std::string& srcPath, const std::string& destPtah) {
    std::ifstream  src(srcPath, std::ios::binary);
    std::ofstream  dst(destPtah,   std::ios::binary);

    dst << src.rdbuf();
}

void MyProcesses::exportLog() {
    fileCopy("/var/log/syslog","/home/dreadpirateroberts/Desktop/forensX-volume/syslog");
    fileCopy("/var/log/auth.log","/home/dreadpirateroberts/Desktop/forensX-volume/auth.log");
    fileCopy("/var/log/boot.log","/home/dreadpirateroberts/Desktop/forensX-volume/boot.log");
    fileCopy("/var/log/kern.log","/home/dreadpirateroberts/Desktop/forensX-volume/kern.log");
    fileCopy("/var/log/faillog","/home/dreadpirateroberts/Desktop/forensX-volume/faillog");
}

void MyProcesses::init() {
    int a;
    getProcesses();
    m_MP.processes.push_back(m_processes);
    m_processes.clear();

    getNetworkConn();
    exportLog();

    std::cout << "sem: ";
    std::cin >> a;

    getProcesses();
    m_MP.processes.push_back(m_processes);
    m_processes.clear();
}
