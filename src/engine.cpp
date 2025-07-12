#include "engine.hpp"
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <cstring>

Engine::Engine() = default;

Engine::~Engine() {
    stop();
}

bool Engine::launch(const std::string& enginePath) {
    SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) }; //какие то атрибуты безопасности
    saAttr.bInheritHandle = TRUE; //разрешаем наследование дескрипторов
    saAttr.lpSecurityDescriptor = NULL; //дефолт настройки безопасности

    HANDLE hChildStdoutRd, hChildStdoutWr; //создаем канал для чтения вывода
    if (!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)) {
        return false;
    }

    HANDLE hChildStdinRd, hChildStdinWr; //создаем канал для ввода
    if (!CreatePipe(&hChildStdinRd, &hChildStdinWr, &saAttr, 0)) {
        CloseHandle(hChildStdoutRd);
        CloseHandle(hChildStdoutWr);
        return false;
    }
    //конфигурируем
    STARTUPINFO si = { sizeof(STARTUPINFO) }; //размер
    si.dwFlags = STARTF_USESTDHANDLES; //используем кастомные дескрипторы
    si.hStdInput = hChildStdinRd; //для чтения команд
    si.hStdOutput = hChildStdoutWr; //для вывода команд
    si.hStdError = hChildStdoutWr; //если ошибка, то тоже в вывод идем

    PROCESS_INFORMATION pi; //структура для хранения информации о процессе
    
    //запускаем процесс
    if (!CreateProcess(NULL, const_cast<LPSTR>(enginePath.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)){ //тут условий жесть сколько
        CloseHandle(hChildStdinRd); //Если че то сломалось, все отключаем и false кидаем
        CloseHandle(hChildStdinWr); 
        CloseHandle(hChildStdoutRd); 
        CloseHandle(hChildStdoutWr); 
        return false;
    }

    hProcess = pi.hProcess; //дескриптор процесса движка
    hInputWrite = hChildStdinWr; //дескриптор записи
    hOutputRead = hChildStdoutRd; //дескриптор чтения

    //закрываем лишнее
    CloseHandle(pi.hThread);
    CloseHandle(hChildStdinRd);
    CloseHandle(hChildStdoutWr);

    return true;
}

void Engine::sendCommand(const std::string& command) {
    if (!hInputWrite) return;
    
    DWORD bytesWritten;
    std::string cmd = command + "\n"; //добавление символа новой строки
    WriteFile( static_cast<HANDLE>(hInputWrite), cmd.c_str(), cmd.size(), &bytesWritten, NULL); //запись команды в канал
}

std::string Engine::readLine() {
    if (!hOutputRead) return "";
    
    char buffer[256]; 
    DWORD bytesRead;
    std::string result; //строка
    
    while (true) { //построчно читаем, пока не найдем \n
        char ch; //символ
        if (!ReadFile(static_cast<HANDLE>(hOutputRead), &ch, 1, &bytesRead, NULL) || bytesRead == 0) {
            break;
        }
        
        if (ch == '\n') break;
        result += ch;
    }
    
    return result;
}

void Engine::stop() {
    if (hInputWrite) {
        sendCommand("quit"); //пишем команду выход
        Sleep(100); 
    }
    
    if (hInputWrite) { //закрываем дескрипторы
        CloseHandle(static_cast<HANDLE>(hInputWrite));
        hInputWrite = nullptr;
    }
    if (hOutputRead) {
        CloseHandle(static_cast<HANDLE>(hOutputRead));
        hOutputRead = nullptr;
    }
    
    if (hProcess) {
        TerminateProcess(static_cast<HANDLE>(hProcess), 0);
        CloseHandle(static_cast<HANDLE>(hProcess));
        hProcess = nullptr;
    }
}