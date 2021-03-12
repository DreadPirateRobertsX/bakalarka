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
    MyProcesses()
    {
        getProcesses();
        loadProcData();
        getNetworkConn();
    }

    void printProcesses();
private:
    void formatUdpTcpTable(std::vector<std::vector<std::string> >& matrix);
    void loadFileToVector(std::string fullPath,std::vector<std::vector<std::string> >& matrix);
    bool isNumber(std::string str);
    void loadProcData();
    static void loadLineToProcess(std::vector<std::string>& process,const std::string& fullPath, size_t lineNum);
    static void loadLineToProcess(std::vector<std::string> &process, const std::string &fullPath);

    std::vector<std::vector <std::string>> m_processes;
    std::vector<std::vector <std::string>> m_networkConnections;
    std::string m_PID;
};

#endif //FORENSX_MYPROCESSES_H
