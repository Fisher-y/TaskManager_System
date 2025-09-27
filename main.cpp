//main.cpp
#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include "TaskManager.h"
#include "Command.h"


int main() {
    TaskManager taskManager;


    // 创建命令对象
    AddCommand addCmd(taskManager);
    DeleteCommand delCmd(taskManager);
    ListCommand listCmd(taskManager);
    UpdateCommand updateCmd(taskManager);
    UpdateStatusCommand statusCmd(taskManager); 

    // 命令映射
    std::unordered_map<std::string, std::unique_ptr<CommandBase>> commands;
    commands["add"] = std::make_unique<AddCommand>(taskManager);
    commands["delete"] = std::make_unique<DeleteCommand>(taskManager);
    commands["list"] = std::make_unique<ListCommand>(taskManager);
    commands["update"] = std::make_unique<UpdateCommand>(taskManager);
    commands["status"] = std::make_unique<UpdateStatusCommand>(taskManager); // 注册状态命令
    
    std::cout << "欢迎使用任务管理系统！" << std::endl;
    std::cout << "可用命令: add, delete, list, update, status, exit" << std::endl;
    std::cout << "使用 'status <ID>,<状态>' 来更新任务状态" << std::endl;
    std::cout << "可用状态: pending(待处理), in_progress(进行中), completed(已完成)" << std::endl;


    std::string input;
    while (true) {
        std::cout << "\\n> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;


        // 分离命令和参数
        size_t spacePos = input.find(' ');
        std::string cmd = input.substr(0, spacePos);
        std::string args;
        if (spacePos != std::string::npos) {
            args = input.substr(spacePos + 1);
        }


        if (cmd == "exit") {
            std::cout << "退出程序。" << std::endl;
            break;
        }
        
        if (cmd == "help") {
            std::cout << "可用命令:" << std::endl;
            std::cout << "add <标题>,<描述>,<优先级>,<截止日期> - 添加新任务" << std::endl;
            std::cout << "delete <ID> - 删除任务" << std::endl;
            std::cout << "list [排序选项] - 列出任务(0=按ID,1=按优先级,2=按截止日期)" << std::endl;
            std::cout << "update <ID>,<标题>,<描述>,<优先级>,<截止日期> - 更新任务" << std::endl;
            std::cout << "status <ID>,<状态> - 更新任务状态" << std::endl;
            std::cout << "exit - 退出程序" << std::endl;
            continue;
        }

        auto it = commands.find(cmd);
        if (it != commands.end()) {
            it->second->execute(args);
        } else {
            std::cout << "未知命令：" << cmd << std::endl;
        }
    }


    return 0;
}
