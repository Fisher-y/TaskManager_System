//Task.h
#ifndef TASK_H
#define TASK_H


#include <string>
#include <sstream>
#include <iomanip>
#include "TableFormatter.h"

struct Task {
    int id;
    std::string title;        // 新增：任务标题
    std::string description;
    int priority; // 1-高, 2-中, 3-低
    std::string dueDate; // 格式: YYYY-MM-DD
    std::string status;      // 新增：任务状态

    std::string toString() const {
         return TableFormatter::formatTask(id, title, priority, dueDate, status, description);
    
    }
};


#endif // TASK_H
