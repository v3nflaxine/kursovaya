
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include "Constants.h" 

using namespace std;

class MagneticTape {
private:
    // Класс узла двусвязного списка
    class TapeNode {
    public:
        string filename;
        int sizeKb;
        string createTime;
        TapeNode* next;
        TapeNode* prev;

        TapeNode(string name, int size, string time)
            : filename(name), sizeKb(size), createTime(time), next(nullptr), prev(nullptr) {}
    };

    TapeNode* head;
    TapeNode* tail;
    int count;
    int strToInt(string data);
    bool isValidInput(const string& input, bool isNumber = false);
    vector<string> splitString(const string& str, char delimiter);
    TapeNode* getNodeAtIndex(int index);

public:
    MagneticTape();     // Конструктор
    ~MagneticTape();    // Деструктор

    void clearAll();
    bool isEmpty() const;
    int getCount() const;

    // Основные операции
    void addToTail(string name, int size, string time);
    bool insertAt(int index, string name, int size, string time);
    bool deleteAt(int index);
    void display() const;

    // Работа с файлами
    bool exportToFile(const string& filename);
    bool importFromFile(const string& filename);

    // Поиск и сортировка
    void search(int searchType, string value);
    void sort(int fieldType);

    // Публичные прокси-методы
    bool publicIsValidInput(const string& input, bool isNumber = false);
    int publicStrToInt(const string& data);
};

