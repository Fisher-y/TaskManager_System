# C++任务管理系统
一个基于C++开发的命令行任务管理系统，采用现代C++设计模式，支持任务的增删改查、状态管理和持久化存储。
## 项目简介
本项目是一个功能完整的任务管理工具，帮助用户高效组织和管理日常任务。系统采用命令行交互方式，支持任务添加、删除、更新、查询和状态跟踪，所有数据通过MySQL数据库持久化存储。
## 核心特性
- 任务全生命周期管理：支持任务的创建、修改、删除和查询

- 智能状态跟踪：任务状态流转（待处理、进行中、已完成）

- 多维度排序：按ID、优先级、截止日期等多种方式排序查看

- 持久化存储：基于MySQL数据库，确保数据安全

- 操作日志记录：完整记录所有操作便于审计

- 命令行界面：简洁易用的交互方式

## 技术架构
### 系统设计模式
- 命令模式 (Command Pattern)：使用CRTP（奇异递归模板模式）实现高效命令分发

- 单例模式 (Singleton Pattern)：日志系统确保全局唯一实例

- 面向对象设计：模块化类设计，便于扩展和维护

### 技术栈
- 编程语言：C++11/14标准

- 数据库：MySQL + MySQL Connector/C++

- 构建工具：CMake 3.10+

- 设计模式：命令模式、单例模式、CRTP

### 项目结构
```bash
TaskManager/
├── main.cpp              # 主程序入口
├── Command.h             # 命令接口和具体命令实现
├── Task.h               # 任务数据结构和格式化
├── TaskManager.h        # 任务管理类声明
├── TaskManager.cpp      # 任务管理类实现
├── Logger.h             # 日志系统声明
├── Logger.cpp           # 日志系统实现
├── TableFormatter.h     # 表格格式化工具
└── CMakeLists.txt       # 项目构建配置
```
## 安装指南
### 前置要求
- C++编译器：支持C++11的编译器（GCC 5.0+或Clang 3.8+）

- MySQL服务器：MySQL 5.7+ 并创建数据库

- CMake：版本3.10或更高

### MySQL Connector/C++：用于数据库连接

### 环境配置步骤
1、安装MySQL数据库
```bash
# Ubuntu/Debian
sudo apt-get install mysql-server libmysqlcppconn-dev


# CentOS/RHEL
sudo yum install mysql-server mysql-devel

```

2、创建数据库和用户

```SQL
CREATE DATABASE task_manager;
CREATE USER 'taskuser'@'localhost' IDENTIFIED BY '12345';
GRANT ALL PRIVILEGES ON task_manager.* TO 'taskuser'@'localhost';
FLUSH PRIVILEGES;
```

3、构建项目
```bash
mkdir build && cd build
cmake ..
make
```

4、运行程序

```Bash
./LogSystem
```

## 使用方法
### 命令概览
系统支持以下命令：
-  add  - 添加新任务
-  del  ete  - 删除任务
-  lis  t  - 列出任务
-  upd  ate  - 更新任务详情
-  sta  tus  - 更新任务状态
-  exi  t  - 退出程序
-  help  - 查看帮助

详细命令语法
添加任务
```bash
add <标题>,<描述>,<优先级>,<截止日期>
# 示例：add "完成报告","编写项目总结文档",2,"2025-10-01"
```
- 优先级：1（高）、2（中）、3（低）
- 日期格式：YYYY-MM-DD

### 更新任务状态
```bash
status <任务ID>,<状态>
# 示例：status 1,in_progress
```
可用状态： pen  ding （待处理）、 in_  progress （进行中）、 com  pleted （已完成）
### 列出任务
```bash
list [排序选项]
# 示例：list 1 （按优先级排序）
```
排序选项：0（按ID）、1（按优先级）、2（按截止日期）
### 更新任务信息
```bash
update <ID>,<标题>,<描述>,<优先级>,<截止日期>
# 示例：update 1,"新标题","新描述",1,"2025-10-02"
```

### 删除任务
```bash
delete <任务ID>
# 示例：delete 1
```

### 数据库设计
系统使用以下数据库表结构存储任务信息：
```SQL
CREATE TABLE tasks (
    task_id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(255) NOT NULL DEFAULT 'Task',
    description TEXT,
    status ENUM('pending', 'in_progress', 'completed') DEFAULT 'pending',
    priority INT DEFAULT 2 COMMENT '1-高, 2-中, 3-低',
    due_date DATE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
);
```
## 设计亮点
1. 命令模式实现
采用CRTP（奇异递归模板模式）实现命令架构，兼具静态多态的效率和动态多态的灵活性。每个命令独立封装，符合开闭原则，新增命令无需修改现有代码。
2. 资源管理
使用智能指针自动管理数据库连接资源

RAII技术确保资源安全释放

异常安全设计保证操作原子性

3. 状态管理
完整的状态流转机制确保任务生命周期可控：
pending（待处理） → in_progress（进行中） → completed（已完成）

Plain Text

4. 日志系统
线程安全的单例日志系统

异步写入避免I/O阻塞

完整操作审计追踪

## 扩展建议
基于当前架构，可以考虑以下扩展方向：
1、用户认证系统：支持多用户访问控制
2、任务分类标签：为任务添加分类和标签功能
3、提醒通知：任务截止日期提醒
4、数据统计：任务完成率、工作效率统计
5、Web界面：提供图形化操作界面
6、数据备份：定期备份和恢复功能

## 故障排除
### 常见问题
1、数据库连接失败

- 检查MySQL服务是否启动： sys  temctl status mysql 

- 验证数据库用户权限

- 确认连接参数正确性

2、编译错误

- 确认MySQL Connector/C++已正确安装

- 检查CMake版本是否符合要求

- 验证编译器支持C++11标准

3、运行时错误

- 检查数据库表是否创建

- 验证任务参数格式是否正确


