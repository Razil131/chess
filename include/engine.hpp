#pragma once

#include <string>

class Engine {
public:
    Engine();
    ~Engine();
    
    bool launch(const std::string& enginePath);
    void sendCommand(const std::string& command);
    std::string readLine();
    void stop();

private:
    void* hProcess = nullptr;      // HANDLE процесса движка
    void* hInputWrite = nullptr;   // HANDLE для записи в stdin движка
    void* hOutputRead = nullptr;   // HANDLE для чтения из stdout движка
};