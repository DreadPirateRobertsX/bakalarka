#include <iostream>
#include "MyProcesses.h"

int main() {
    MyProcesses processes;
    processes.printProcesses(0);
    processes.printProcesses(1);

    processes.printConnections();

    return 0;
}
