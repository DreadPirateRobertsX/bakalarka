//
// Created by root on 11. 3. 2021.
//

#ifndef FORENSX_MYPROCESSES_H
#define FORENSX_MYPROCESSES_H
#include <vector>

class  MyProcesses{

public:
    void getProcesses();
    void getNetworkConn();
    void init();
    MyProcesses()
    {
        init();
    }

    void printProcesses(int index);
    void printConnections();
private:
    static void exportLog();
    static void fileCopy(const std::string& srcPath, const std::string& destPtah);
    static void formatUdpTcpTable(std::vector<std::vector<std::string> >& matrix, const std::string& type);
    static void loadFileToVector(const std::string& fullPath,std::vector<std::vector<std::string> >& matrix);
    bool isNumber(std::string str);
    void loadProcData();
    static void loadLineToProcess(std::vector<std::string>& process,const std::string& fullPath, size_t lineNum);
    static void loadLineToProcess(std::vector<std::string> &process, const std::string &fullPath);

    std::vector<std::vector <std::string>> m_processes;
    std::vector<std::vector <std::string>> m_networkConnections;
    struct m_processesStore{
        std::vector<std::vector<std::vector <std::string>>> processes;
        std::vector<std::string> hash_values;
    };
    struct m_processesStore m_MP;
    std::string m_PID;
};

#endif //FORENSX_MYPROCESSES_H
