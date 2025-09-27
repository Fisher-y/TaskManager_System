//TaskManager.cpp
#include "TaskManager.h"
#include "Logger.h"
#include <iostream>
#include <stdexcept>
#include "TableFormatter.h"

TaskManager::TaskManager() {
    establishConnection();
    initializeDatabase();
    Logger::getInstance().log("数据库连接已建立。");
}

TaskManager::~TaskManager() {
    if (connection) {
        connection->close();
        Logger::getInstance().log("数据库连接已关闭。");
    }
}

void TaskManager::establishConnection() {
    try {
        sql::Driver* driver = get_driver_instance();
        // 修改为你的数据库连接信息
        connection.reset(driver->connect("tcp://127.0.0.1:3306", "taskuser", "12345"));
        connection->setSchema("task_manager"); // 使用我们创建的数据库
        Logger::getInstance().log("MySQL数据库连接成功建立");
    } catch (sql::SQLException& e) {
        std::cerr << "MySQL数据库连接失败: " << e.what() << std::endl;
        Logger::getInstance().log("MySQL数据库连接失败: " + std::string(e.what()));
        throw std::runtime_error("数据库连接失败");
    }
}

void TaskManager::initializeDatabase() {
    try {
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        
        // 创建数据库（如果不存在）
        stmt->execute("CREATE DATABASE IF NOT EXISTS task_manager");
        connection->setSchema("task_manager");
        
        // 创建任务表
        stmt->execute(
            "CREATE TABLE IF NOT EXISTS tasks ("
            "task_id INT AUTO_INCREMENT PRIMARY KEY, "
            "title VARCHAR(255) NOT NULL DEFAULT 'Task', "
            "description TEXT, "
            "status ENUM('pending', 'in_progress', 'completed') DEFAULT 'pending', "
            "priority INT DEFAULT 2 COMMENT '1-高, 2-中, 3-低', "
            "due_date DATE, "
            "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, "
            "updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"
            ")"
        );
        
        Logger::getInstance().log("数据库初始化完成");
    } catch (sql::SQLException& e) {
        std::cerr << "数据库初始化失败: " << e.what() << std::endl;
        Logger::getInstance().log("数据库初始化失败: " + std::string(e.what()));
    }
}

void TaskManager::addTask(const std::string& title,const std::string& description, int priority, const std::string& dueDate) {
     try {
        std::unique_ptr<sql::PreparedStatement> prepStmt(connection->prepareStatement(
            "INSERT INTO tasks (title, description, priority, due_date) VALUES (?, ?, ?, ?)"));
        
        prepStmt->setString(1, title);
        prepStmt->setString(2, description);
        prepStmt->setInt(3, priority);
        prepStmt->setString(4, dueDate);
        prepStmt->executeUpdate();
        
      
        Logger::getInstance().log("添加任务: " + title);
        
    } catch (sql::SQLException& e) {
        std::cerr << "添加任务失败: " << e.what() << std::endl;
        Logger::getInstance().log("添加任务失败: " + std::string(e.what()));
    }
}


void TaskManager::deleteTask(int id) {
    try {
    std::unique_ptr<sql::PreparedStatement> prepStmt(
            connection->prepareStatement("DELETE FROM tasks WHERE task_id = ?")
        );
    prepStmt->setInt(1, id);
    int affectedRows = prepStmt->executeUpdate();

     if (affectedRows > 0) {
            Logger::getInstance().log("删除任务成功，ID: " + std::to_string(id));
            std::cout << "任务删除成功。" << std::endl;
            // 删除后自动重整ID
            reorderTaskIDsAfterDelete();
        } else {
            std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
        }
        
    }catch (sql::SQLException& e) {
        std::cerr << "删除任务失败: " << e.what() << std::endl;
        Logger::getInstance().log("删除任务失败: " + std::string(e.what()));
    }
}
void TaskManager::reorderTaskIDsAfterDelete() {
    try {
        // 获取当前所有ID并按顺序重新编号
        std::string reorderQuery = 
            "SET @new_id = 0; "
            "UPDATE tasks SET task_id = (@new_id := @new_id + 1) ORDER BY task_id; "
            "ALTER TABLE tasks AUTO_INCREMENT = (SELECT MAX(task_id) + 1 FROM tasks);";
        
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        stmt->execute("SET @new_id = 0");
        stmt->execute("UPDATE tasks SET task_id = (@new_id := @new_id + 1) ORDER BY task_id");
        
        // 重置自增计数器
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT MAX(task_id) FROM tasks"));
        if (res->next()) {
            int maxId = res->getInt(1);
            stmt->execute("ALTER TABLE tasks AUTO_INCREMENT = " + std::to_string(maxId + 1));
        }
        
    } catch (sql::SQLException& e) {
        std::cerr << "ID重整失败: " << e.what() << std::endl;
    }
}


void TaskManager::updateTask(int id, const std::string& title,const std::string& description, int priority, const std::string& dueDate) {
    try {
        
        std::unique_ptr<sql::PreparedStatement> prepStmt(
            connection->prepareStatement(
                "UPDATE tasks SET title = ?, description = ?, priority = ?, due_date = ?, updated_at=CURRENT_TIMESTAMP WHERE task_id = ?"
            )
        );
        
        prepStmt->setString(1, title);
        prepStmt->setString(2, description);
        prepStmt->setInt(3, priority);
        prepStmt->setString(4, dueDate);
        prepStmt->setInt(5, id);

        int affectedRows = prepStmt->executeUpdate();
        
        if (affectedRows > 0) {
            Logger::getInstance().log("更新任务成功，ID: " + std::to_string(id));
            std::cout << "任务更新成功。" << std::endl;
        } else {
            std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
        }
        
    } catch (sql::SQLException& e) {
        std::cerr << "更新任务失败: " << e.what() << std::endl;
        Logger::getInstance().log("更新任务失败: " + std::string(e.what()));
    }
}

// 在TaskManager类中添加状态相关的辅助方法
void TaskManager::showStatusOptions() const {
    std::cout << "\n可用状态选项:" << std::endl;
    std::cout << "1. pending - 待处理" << std::endl;
    std::cout << "2. in_progress - 进行中" << std::endl;
    std::cout << "3. completed - 已完成" << std::endl;
}

bool TaskManager::isValidStatus(const std::string& status) const {
    return (status == "pending" || status == "in_progress" || status == "completed");
}

void TaskManager::updateTaskStatus(int id, const std::string& status) {
      if (!isValidStatus(status)) {
        std::cout << "无效状态值。可用状态: pending, in_progress, completed" << std::endl;
        return;
    }
    try {
        // 首先检查任务是否存在
        std::unique_ptr<sql::PreparedStatement> checkStmt(
            connection->prepareStatement("SELECT task_id FROM tasks WHERE task_id = ?")
        );
        checkStmt->setInt(1, id);
        std::unique_ptr<sql::ResultSet> res(checkStmt->executeQuery());
        
        if (!res->next()) {
            std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
            return;
        }
        std::unique_ptr<sql::PreparedStatement> pstmt(connection->prepareStatement(
            "UPDATE tasks SET status = ? WHERE task_id = ?"));
        
        pstmt->setString(1, status);
        pstmt->setInt(2, id);
        
        int affectedRows = pstmt->executeUpdate();
        
        if (affectedRows > 0) {
           // 获取更新后的任务信息用于日志
            std::unique_ptr<sql::PreparedStatement> getStmt(
                connection->prepareStatement("SELECT title FROM tasks WHERE task_id = ?")
            );
            getStmt->setInt(1, id);
            std::unique_ptr<sql::ResultSet> taskRes(getStmt->executeQuery());
            
            std::string taskTitle = "Unknown";
            if (taskRes->next()) {
                taskTitle = taskRes->getString("title");
            }
            
            Logger::getInstance().log("更新任务状态 ID: " + std::to_string(id) + 
                                    " 标题: " + taskTitle + " 状态: " + status);
            std::cout << "任务状态更新成功！" << std::endl;
            
            // 显示状态变更信息
            std::cout << "任务 '" << taskTitle << "' 的状态已更新为: ";
            if (status == "pending") std::cout << "待处理";
            else if (status == "in_progress") std::cout << "进行中";
            else if (status == "completed") std::cout << "已完成";
            std::cout << std::endl;
        } else {
            std::cout << "未找到ID为 " << id << " 的任务。" << std::endl;
        }
    } catch (sql::SQLException& e) {
        std::cerr << "更新任务状态失败: " << e.what() << std::endl;
        Logger::getInstance().log("更新任务状态失败: " + std::string(e.what()));
    }
}

// 添加按状态筛选任务的方法
void TaskManager::listTasksByStatus(const std::string& status) const {
    try {
        std::string query = "SELECT task_id, title, description, priority, due_date, status FROM tasks WHERE status = ? ORDER BY task_id";
        
        std::unique_ptr<sql::PreparedStatement> stmt(connection->prepareStatement(query));
        stmt->setString(1, status);
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery());
        
        std::cout << "状态为 '";
        if (status == "pending") std::cout << "待处理";
        else if (status == "in_progress") std::cout << "进行中";
        else if (status == "completed") std::cout << "已完成";
        std::cout << "' 的任务列表:" << std::endl;
        
        TableFormatter::printHeader();
        
        bool hasTasks = false;
        while (res->next()) {
            hasTasks = true;
            std::cout << TableFormatter::formatTask(
                res->getInt("task_id"),
                res->getString("title"),
                res->getInt("priority"),
                res->getString("due_date"),
                res->getString("status"),
                res->getString("description")
            ) << std::endl;
        }
        
        if (!hasTasks) {
            std::cout << "没有找到相应状态的任务。" << std::endl;
        }
        
    } catch (sql::SQLException& e) {
        std::cerr << "按状态查询任务失败: " << e.what() << std::endl;
        Logger::getInstance().log("按状态查询任务失败: " + std::string(e.what()));
    }
}

void TaskManager::listTasks(int sortOption) const {
     // 直接从数据库实时查询，确保数据最新
    try {
      
        std::string query = "SELECT task_id, title, description, priority, due_date, status FROM tasks";
        switch (sortOption) {
            case 1: query += " ORDER BY priority"; break;
            case 2: query += " ORDER BY due_date"; break;
            default: query += " ORDER BY task_id"; 
        }
        
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery(query));
        
        std::cout << "任务列表:" << std::endl;
       
         TableFormatter::printHeader();  // 使用统一的表头输出
        
        while (res->next()) {
            std::cout << TableFormatter::formatTask(
                res->getInt("task_id"),
                res->getString("title"),
                res->getInt("priority"),
                res->getString("due_date"),
                res->getString("status"),
                res->getString("description")
            ) << std::endl;
        }
        
    } catch (sql::SQLException& e) {
        std::cerr << "查询任务失败: " << e.what() << std::endl;
         Logger::getInstance().log("查询任务失败: " + std::string(e.what()));
    
    }
}


