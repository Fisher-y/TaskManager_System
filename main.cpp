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


    // 命令映射
    std::unordered_map<std::string, std::unique_ptr<CommandBase>> commands;
    commands["add"] = std::make_unique<AddCommand>(taskManager);
    commands["delete"] = std::make_unique<DeleteCommand>(taskManager);
    commands["list"] = std::make_unique<ListCommand>(taskManager);
    commands["update"] = std::make_unique<UpdateCommand>(taskManager);


    std::cout << "欢迎使用任务管理系统！" << std::endl;
    std::cout << "可用命令: add, delete, list, update, exit" << std::endl;


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


        auto it = commands.find(cmd);
        if (it != commands.end()) {
            it->second->execute(args);
        } else {
            std::cout << "未知命令：" << cmd << std::endl;
        }
    }


    return 0;
}
