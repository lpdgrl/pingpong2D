#include "logger.hpp"

void Logger::Log(Render* render, float x, float y, float scale, glm::vec3 color) {
    bool first = true;
    float offset = 10.f;

    for (const auto& [key, value] : log_array_) {
        if (first) {
            render->DrawText(std::string(key + ": " + value), x, y, scale, color);
            first = false;
            continue;
        }
        y -= offset;
        render->DrawText(std::string(key + ": " + value), x, y, scale, color);
    }
    
}

std::vector<std::pair<std::string, std::string>>& Logger::GetLogArray() {
    return log_array_;
}

void Logger::InitLogArray(std::vector<std::string>& vec) {
    for (const auto& name_log_param : vec) {
        log_array_.push_back(std::make_pair(name_log_param, ""));
    }
 }
 
void Logger::UpdateDataLog(std::vector<std::string>& data_to_log) {
    auto it_data_to_log = data_to_log.begin();
    
    for (auto& [key, value] : log_array_) {
        if (it_data_to_log != data_to_log.end()) {
            value = *it_data_to_log;
            ++it_data_to_log;
        }   
    }
 }