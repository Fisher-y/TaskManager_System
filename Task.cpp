#include "Task.h"
#include <sstream>
#include <iostream>

// 序列化任务到字符串（用于文件存储）
std::string Task::serialize() const {
    std::ostringstream oss;
    oss << id << "," << description << "," << priority << "," << dueDate;
    return oss.str();
}

// 从字符串反序列化（静态方法，用于从文件加载）
Task Task::deserialize(const std::string& taskData) {
    std::istringstream iss(taskData);
    Task task;
    char delimiter; // 用于读取逗号
    
    // 读取ID
    iss >> task.id;
    if (iss.fail()) {
        std::cerr << "反序列化错误: 无法读取任务ID" << std::endl;
        return Task(); // 返回默认任务
    }
    iss >> delimiter; // 读取逗号
    
    // 读取描述
    std::getline(iss, task.description, ',');
    
    // 读取优先级
    iss >> task.priority;
    if (iss.fail()) {
        std::cerr << "反序列化错误: 无法读取任务优先级" << std::endl;
        return Task(); // 返回默认任务
    }
    iss >> delimiter; // 读取逗号
    
    // 读取截止日期
    iss >> task.dueDate;
    if (iss.fail()) {
        std::cerr << "反序列化错误: 无法读取任务截止日期" << std::endl;
        return Task(); // 返回默认任务
    }
    
    return task;
}
