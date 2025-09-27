//TaskManger.h
#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include "Task.h"
#include <vector>
#include <string>
#include <memory>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class TaskManager {
public:
    TaskManager();
    ~TaskManager();

   
    void addTask(const std::string& title,const std::string& description, int priority, const std::string& dueDate);
    void deleteTask(int id);
    void reorderTaskIDsAfterDelete();
    void updateTask(int id, const std::string& title,const std::string& description, int priority, const std::string& dueDate);
    void listTasks(int sortOption = 0) const; // 0-按ID, 1-按优先级, 2-按截止日期
    

    void updateTaskStatus(int id, const std::string& status) ;
    void listTasksByStatus(const std::string& status)  const;
    void showStatusOptions()  const;
    bool isValidStatus(const std::string& status) const;

private:
    std::unique_ptr<sql::Connection> connection;
    void establishConnection(); // 建立数据库连接
    void initializeDatabase();
    
};


#endif // TASKMANAGER_H
