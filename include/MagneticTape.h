#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "Constants.h" 

class MagneticTape {
private:
    struct TapeNode {
        std::string filename;
        int sizeKb;
        std::string createTime;
        TapeNode* next = nullptr;
        TapeNode* prev = nullptr;

        TapeNode(std::string name, int size, std::string time)
            : filename(name), sizeKb(size), createTime(time) {}
    };

    TapeNode* head;
    TapeNode* tail;
    int count;

    // Внутренние утилиты для работы со списком
    std::vector<std::string> splitString(const std::string& str, char delimiter);
    TapeNode* getNodeAtIndex(int index);

public:
    MagneticTape();
    ~MagneticTape();

    void clearAll();
    bool isEmpty() const;
    int getCount() const;

    void addToTail(std::string name, int size, std::string time);
    bool insertAt(int index, std::string name, int size, std::string time);
    bool deleteAt(int index);
    void display() const;

    bool exportToFile(const std::string& filename);
    bool importFromFile(const std::string& filename);

    void search(int searchType, const std::string& value);
    void sort(int fieldType);
};
