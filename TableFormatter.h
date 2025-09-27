// TableFormatter.h
#ifndef TABLEFORMATTER_H
#define TABLEFORMATTER_H

#include <iostream>
#include <iomanip>
#include <string>

class TableFormatter {
public:
    // 列宽常量定义
    static const int ID_WIDTH = 4;
    static const int TITLE_WIDTH = 15;
    static const int PRIORITY_WIDTH = 8;
    static const int DUEDATE_WIDTH = 12;
    static const int STATUS_WIDTH = 15;
    static const int DESCRIPTION_WIDTH = 30;
    
    // 计算中英文字符串的显示宽度
    static int getChineseWidth(const std::string& str) {
        int width = 0;
        
    for (size_t i = 0; i < str.length();) {
        unsigned char c = static_cast<unsigned char>(str[i]);
        int charWidth = 1; // 默认字符显示宽度
        int bytes = 1;     // 当前字符占用的字节数

        // 判断UTF-8字符的字节序列和显示宽度
        if (c < 0x80) { // ASCII字符
            charWidth = 1;
            bytes = 1;
        } else if ((c & 0xE0) == 0xC0) { // 2字节字符
            charWidth = 1;
            bytes = 2;
        } else if ((c & 0xF0) == 0xE0) { // 3字节字符（包括常用汉字）
            charWidth = 2; // 汉字通常显示宽度为2
            bytes = 3;
        } else if ((c & 0xF8) == 0xF0) { // 4字节字符（如emoji）
            charWidth = 2; // 多数emoji显示宽度为2
            bytes = 4;
        } else {
            // 非法UTF-8起始字节，按单字节处理
            charWidth = 1;
            bytes = 1;
        }

        // 确保不越界
        if (i + bytes > str.length()) {
            bytes = 1; // 如果字节不完整，按单字节处理
        }

        width += charWidth;
        i += bytes;
    }
        return width;
    }
    

    // 字符串截断函数
     static std::string truncateString(const std::string& str, int maxDisplayWidth) {
        int currentWidth = 0;
        std::string result;
        size_t i = 0;
        
        while (i < str.length()) {
            char c = str[i];
            int charWidth = ((c & 0x80) != 0) ? 2 : 1;
            
            if (currentWidth + charWidth > maxDisplayWidth - 3) {
                if (currentWidth + 3 <= maxDisplayWidth) {
                    // 检查是否需要避免截断中文字符
                    if (i < str.length() && (str[i] & 0x80) != 0) {
                        // 如果下一个字符是中文字符，提前结束
                        result += "...";
                    } else {
                        result += "...";
                    }
                }
                break;
            }
            
            result += c;
            currentWidth += charWidth;
            i++;
            
            // 如果是中文字符，跳过下一个字节（UTF-8连续字节）
            if ((c & 0x80) != 0) {
                while (i < str.length() && (str[i] & 0xC0) == 0x80) {
                    result += str[i];
                    i++;
                }
            }
        }
        return result;
    }
     // 填充字符串到指定显示宽度
    static std::string padToWidth(const std::string& str, int targetWidth) {
        int currentWidth = getChineseWidth(str);
        if (currentWidth >= targetWidth) {
            return truncateString(str, targetWidth);
        }
        return str + std::string(targetWidth - currentWidth, ' ');
    }
    // 输出表头
    static void printHeader() {
        std::cout << std::left 
                  << padToWidth("ID", ID_WIDTH) << " | "
                  << padToWidth("标题", TITLE_WIDTH) << " | "
                  << padToWidth("优先级", PRIORITY_WIDTH) << " | "
                  << padToWidth("截止日期", DUEDATE_WIDTH) << " | "
                  << padToWidth("状态", STATUS_WIDTH) << " | "
                  << padToWidth("描述", DESCRIPTION_WIDTH) 
                  << std::endl;
        
        // 计算分隔线长度
        int totalWidth = ID_WIDTH + TITLE_WIDTH + PRIORITY_WIDTH + 
                        DUEDATE_WIDTH + STATUS_WIDTH + DESCRIPTION_WIDTH +10;
        std::cout << std::string(totalWidth, '-') << std::endl;
    }
    
    // 输出任务行（用于Task结构体）
    static std::string formatTask(int id, const std::string& title, int priority, 
                                 const std::string& dueDate, const std::string& status, 
                                 const std::string& description) {
        std::ostringstream oss;
        oss << std::left
            << padToWidth(std::to_string(id), ID_WIDTH) << " | "
            << padToWidth(truncateString(title, TITLE_WIDTH - 2), TITLE_WIDTH) << " | "
            << padToWidth(std::to_string(priority), PRIORITY_WIDTH) << " | "
            << padToWidth(dueDate, DUEDATE_WIDTH) << " | "
            << padToWidth(status, STATUS_WIDTH) << " | "
            << padToWidth(truncateString(description, DESCRIPTION_WIDTH - 2), DESCRIPTION_WIDTH);
            // << std::setw(ID_WIDTH) << id
            // << std::setw(TITLE_WIDTH) << title
            // << std::setw(PRIORITY_WIDTH) << priority
            // << std::setw(DUEDATE_WIDTH) << dueDate
            // << std::setw(STATUS_WIDTH) << status
            // << std::setw(DESCRIPTION_WIDTH) << truncateString(description, DESCRIPTION_WIDTH - 2);
        return oss.str();
    }
    // 专门为数据库查询结果设计的格式化函数
    static void formatDatabaseRow(int id, const std::string& title, int priority,
                                const std::string& dueDate, const std::string& status,
                                const std::string& description) {
        std::cout << std::left
                  << padToWidth(std::to_string(id), ID_WIDTH) << " | "
                  << padToWidth(truncateString(title, TITLE_WIDTH - 2), TITLE_WIDTH) << " | "
                  << padToWidth(std::to_string(priority), PRIORITY_WIDTH) << " | "
                  << padToWidth(dueDate, DUEDATE_WIDTH) << " | "
                  << padToWidth(status, STATUS_WIDTH) << " | "
                  << padToWidth(truncateString(description, DESCRIPTION_WIDTH - 2), DESCRIPTION_WIDTH)<< " | "
                  << std::endl;
    }
};

#endif // TABLEFORMATTER_H
