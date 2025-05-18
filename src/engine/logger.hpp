#pragma once

#include <vector>
#include "render.hpp"

class Logger {

public:
    Logger(Logger&) = delete;

    void operator=(const Logger&) = delete;

    inline static Logger* GetInstance() { 
        if (logger_ == nullptr) {
            logger_ = new Logger();
        }

        return logger_;
    }

    static void Log(Render* render, float x, float y, float scale, glm::vec3 color);
    static void InitLogArray(std::vector<std::string>& vec);
    static std::vector<std::pair<std::string, std::string>>& GetLogArray();  
    static void UpdateDataLog(std::vector<std::string>& data_to_log);
    

private:
    Logger() {}
    inline static Logger* logger_;
    inline static std::vector<std::pair<std::string, std::string>> log_array_;
};