//Command.h
#ifndef COMMAND_H
#define COMMAND_H


#include <string>


class CommandBase{
public:
    virtual void execute(const std::string& args) = 0;
};


// CRTP 基类模板
template <typename Derived>
class Command :public CommandBase{
public:
    void execute(const std::string& args) {
        static_cast<Derived*>(this)->executeImpl(args);
    }
};


// 具体命令类示例
#include "TaskManager.h"
#include "Logger.h"
#include <iostream>


// 添加任务命令
class AddCommand : public Command<AddCommand> {
public:
    AddCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args) {
        // 简单的参数解析：标题，描述,优先级,截止日期
        size_t pos1 = args.find(',');
        size_t pos2 = args.find(',', pos1 + 1);
        size_t pos3 = args.find(',', pos2 + 1);

        if (pos1 == std::string::npos || pos2 == std::string::npos) {
            std::cout << "参数格式错误。请使用: add <标题>,<描述>,<优先级>,<截止日期>" << std::endl;
            return;
        }
        std::string title = args.substr(0, pos1);
        std::string description = args.substr(pos1 + 1, pos2 - pos1 - 1);
        int priority = std::stoi(args.substr(pos2 + 1, pos3 - pos2 - 1));
        std::string dueDate = args.substr(pos3 + 1);
        taskManager.addTask(title,description, priority, dueDate);
        std::cout << "任务添加成功。" << std::endl;
    }
private:
    TaskManager& taskManager;
};


// 删除任务命令
class DeleteCommand : public Command<DeleteCommand> {
public:
    DeleteCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args) {
        try{
            size_t pos;
            int id = std::stoi(args, &pos);
            if(pos != args.length()){
                std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
                return;
            }
            taskManager.deleteTask(id);
            
        }catch(const std::invalid_argument& e){
            std::cout << "参数格式错误。请使用: delete <ID>" << std::endl;
            return;
        }catch(const std::out_of_range& e){
            std::cout << "ID超出范围。请使用有效的任务ID。" << std::endl;
            return;
        }


    }
private:
    TaskManager& taskManager;
};


// 列出任务命令
class ListCommand : public Command<ListCommand> {
public:
    ListCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args) {
        int sortOption = 0;
        if (!args.empty()) {
            sortOption = std::stoi(args);
        }
        taskManager.listTasks(sortOption);
    }
private:
    TaskManager& taskManager;
};


// 更新任务命令
class UpdateCommand : public Command<UpdateCommand> {
public:
    UpdateCommand(TaskManager& manager) : taskManager(manager) {}
    void executeImpl(const std::string& args) {
        // 参数格式: ID,描述,优先级,截止日期
        size_t pos1 = args.find(',');
        size_t pos2 = args.find(',', pos1 + 1);
        size_t pos3 = args.find(',', pos2 + 1);
        size_t pos4 = args.find(',', pos3 + 1);

        if (pos1 == std::string::npos || pos2 == std::string::npos || pos3 == std::string::npos|| pos4 == std::string::npos) {
            std::cout << "参数格式错误。请使用: update <ID>,<标题>,<描述>,<优先级>,<截止日期>" << std::endl;
            return;
        }
        int id = std::stoi(args.substr(0, pos1));
        std::string title= args.substr(pos1 + 1, pos2 - pos1 - 1);
        std::string description = args.substr(pos2 + 1, pos3 - pos2 - 1);
        int priority = std::stoi(args.substr(pos3 + 1, pos4 - pos3 - 1));
        std::string dueDate = args.substr(pos4 + 1);

        taskManager.updateTask(id,title, description, priority, dueDate);
        
    }
private:
    TaskManager& taskManager;
};

// 更新任务状态命令
class UpdateStatusCommand : public Command<UpdateStatusCommand> {
public:
    UpdateStatusCommand(TaskManager& manager) : taskManager(manager) {}
    
    void executeImpl(const std::string& args) {
        // 参数格式: ID,状态
        size_t pos = args.find(',');
        if (pos == std::string::npos) {
            std::cout << "参数格式错误。请使用: status <ID>,<状态>" << std::endl;
            std::cout << "可用状态: pending, in_progress, completed" << std::endl;
            return;
        }
        
        try {
            int id = std::stoi(args.substr(0, pos));
            std::string status = args.substr(pos + 1);
            
            // 验证状态值
            if (status != "pending" && status != "in_progress" && status != "completed") {
                std::cout << "无效状态。可用状态: pending, in_progress, completed" << std::endl;
                return;
            }
            
            taskManager.updateTaskStatus(id, status);
            
        } catch(const std::invalid_argument& e) {
            std::cout << "参数格式错误。请使用: status <ID>,<状态>" << std::endl;
        } catch(const std::out_of_range& e) {
            std::cout << "ID超出范围。请使用有效的任务ID。" << std::endl;
        }
    }
    
private:
    TaskManager& taskManager;
};

#endif // COMMAND_H
