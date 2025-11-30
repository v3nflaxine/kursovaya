#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <algorithm> 

using namespace std;

// Массив сообщений об ошибках
const string errors[6] = {
    "Нельзя выполнить операцию с пустым списком",
    "Введено неверное значение",
    "Индекс выходит за пределы списка",
    "Список пуст",
    "Файл не найден или поврежден",
    "Элемент не найден"
};

const string programmInfo = "Программа моделирования магнитной ленты (Двусвязный список).\nВведите пункт меню и нажмите Enter для продолжения\n";

// Пункты меню
const string choses[9] = {
    "1. Добавить запись в конец ленты",
    "2. Вставить запись в произвольное место",
    "3. Вывести содержимое ленты",
    "4. Удалить запись по позиции",
    "5. Экспорт ленты в файл",
    "6. Импорт ленты из файла",
    "7. Поиск записи",
    "8. Сортировка ленты",
    "0. Выход"
};

class MagneticTape {
private:
    // Класс узла двусвязного списка
    class TapeNode {
    public:
        string filename;    // Название файла
        int sizeKb;         // Размер в Кб
        string createTime;  // Время создания (строка)

        TapeNode* next;     // Указатель на следующий
        TapeNode* prev;     // Указатель на предыдущий

        TapeNode(string name, int size, string time)
            : filename(name), sizeKb(size), createTime(time), next(nullptr), prev(nullptr) {}
    };

    TapeNode* head; // Начало ленты
    TapeNode* tail; // Конец ленты
    int count;      // Количество записей

    // Преобразование строки в число (твоя реализация)
    int strToInt(string data) {
        int newData = 0;
        for (int i = 0; i < data.size(); i++) {
            if (data[i] >= '0' && data[i] <= '9') {
                newData = newData * 10 + (data[i] - '0');
            }
        }
        return newData;
    }

    // Проверка ввода (твоя стилистика)
    bool isValidInput(const string& input, bool isNumber = false) {
        if (input.empty()) return false;
        if (input == "exit") return true;

        if (isNumber) {
            for (char c : input) {
                if (c < '0' || c > '9') return false;
            }
            return true;
        }
        // Для текста разрешаем больше символов, но проверяем на пустоту
        return true;
    }

    // Вспомогательная функция для разделения строки (для парсинга файла)
    vector<string> splitString(const string& str, char delimiter) {
        vector<string> tokens;
        string token;
        for (char c : str) {
            if (c == delimiter) {
                if (!token.empty()) {
                    tokens.push_back(token);
                    token.clear();
                }
            }
            else {
                token += c;
            }
        }
        if (!token.empty()) tokens.push_back(token);
        return tokens;
    }

    // Получение узла по индексу (служебный метод)
    TapeNode* getNodeAtIndex(int index) {
        if (index < 1 || index > count) return nullptr;

        // Оптимизация поиска: идем с начала или с конца
        TapeNode* current;
        if (index <= count / 2) {
            current = head;
            for (int i = 1; i < index; i++) current = current->next;
        }
        else {
            current = tail;
            for (int i = count; i > index; i--) current = current->prev;
        }
        return current;
    }

public:
    MagneticTape() : head(nullptr), tail(nullptr), count(0) {}

    ~MagneticTape() {
        clearAll();
    }

    void clearAll() {
        TapeNode* current = head;
        while (current != nullptr) {
            TapeNode* temp = current;
            current = current->next;
            delete temp;
        }
        head = nullptr;
        tail = nullptr;
        count = 0;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    int getCount() const {
        return count;
    }

    // Добавление в конец (стандартная операция)
    void addToTail(string name, int size, string time) {
        TapeNode* newNode = new TapeNode(name, size, time);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        count++;
    }

    // Вставка в произвольное место (по заданию)
    bool insertAt(int index, string name, int size, string time) {
        if (index < 1 || index > count + 1) return false;

        // Если вставка в конец или список пуст
        if (index == count + 1) {
            addToTail(name, size, time);
            return true;
        }

        TapeNode* newNode = new TapeNode(name, size, time);

        // Вставка в начало
        if (index == 1) {
            newNode->next = head;
            if (head != nullptr) {
                head->prev = newNode;
            }
            head = newNode;
            if (tail == nullptr) tail = newNode;
        }
        else {
            // Вставка в середину
            TapeNode* current = getNodeAtIndex(index); // Это узел, который сместится вправо
            TapeNode* previous = current->prev;

            previous->next = newNode;
            newNode->prev = previous;
            newNode->next = current;
            current->prev = newNode;
        }
        count++;
        return true;
    }

    // Удаление из произвольного места
    bool deleteAt(int index) {
        if (index < 1 || index > count) return false;

        TapeNode* toDelete = getNodeAtIndex(index);

        // Если удаляем единственный элемент
        if (head == tail) {
            head = nullptr;
            tail = nullptr;
        }
        // Если удаляем первый
        else if (toDelete == head) {
            head = head->next;
            head->prev = nullptr;
        }
        // Если удаляем последний
        else if (toDelete == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        // Удаление из середины
        else {
            toDelete->prev->next = toDelete->next;
            toDelete->next->prev = toDelete->prev;
        }

        delete toDelete;
        count--;
        return true;
    }

    // Вывод списка
    void display() const {
        if (isEmpty()) {
            cout << errors[3] << endl;
            return;
        }

        cout << "№\tРазмер(Кб)\tВремя\t\tИмя файла" << endl;
        cout << "----------------------------------------------------" << endl;

        TapeNode* current = head;
        int i = 1;
        while (current != nullptr) {
            cout << i << "\t"
                << current->sizeKb << "\t\t"
                << current->createTime << "\t\t"
                << current->filename << endl;
            current = current->next;
            i++;
        }
        cout << "----------------------------------------------------" << endl;
    }

    // Экспорт в файл
    bool exportToFile(const string& filename) {
        string fullName = filename + ".txt";
        ofstream file(fullName);

        if (!file.is_open()) {
            cout << "Ошибка создания файла!" << endl;
            return false;
        }

        TapeNode* current = head;
        while (current != nullptr) {
            // Формат: Имя;Размер;Время
            file << current->filename << ";"
                << current->sizeKb << ";"
                << current->createTime << endl;
            current = current->next;
        }
        file.close();
        return true;
    }

    // Импорт из файла
    bool importFromFile(const string& filename) {
        string fullName = filename + ".txt";
        ifstream file(fullName);
        if (!file.is_open()) {
            cout << errors[4] << endl;
            return false;
        }

        clearAll(); // Очищаем перед загрузкой

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            // Используем точку с запятой как разделитель
            vector<string> parts = splitString(line, ';');
            if (parts.size() >= 3) {
                string name = parts[0];
                int size = strToInt(parts[1]);
                string time = parts[2];
                addToTail(name, size, time);
            }
        }
        file.close();
        return true;
    }

    // Поиск по указанному полю
    void search(int searchType, string value) {
        if (isEmpty()) {
            cout << errors[3] << endl;
            return;
        }

        TapeNode* current = head;
        bool found = false;
        int index = 1;

        while (current != nullptr) {
            bool isMatch = false;
            if (searchType == 1) { // По имени
                if (current->filename == value) isMatch = true;
            }
            else if (searchType == 2) { // По размеру
                // Конвертируем значение поиска в int
                if (current->sizeKb == strToInt(value)) isMatch = true;
            }
            else if (searchType == 3) { // По времени
                if (current->createTime == value) isMatch = true;
            }

            if (isMatch) {
                cout << "Найдено на позиции " << index << ": "
                    << current->filename << " (" << current->sizeKb << " Kb, " << current->createTime << ")" << endl;
                found = true;
            }
            current = current->next;
            index++;
        }

        if (!found) cout << errors[5] << endl;
    }

    // Сортировка (пузырек, меняем данные местами)
    void sort(int fieldType) {
        if (head == nullptr || head->next == nullptr) return;

        bool swapped;
        do {
            swapped = false;
            TapeNode* current = head;

            while (current->next != nullptr) {
                bool needSwap = false;

                // 1 - имя, 2 - размер, 3 - время
                if (fieldType == 1) {
                    if (current->filename > current->next->filename) needSwap = true;
                }
                else if (fieldType == 2) {
                    if (current->sizeKb > current->next->sizeKb) needSwap = true;
                }
                else if (fieldType == 3) {
                    if (current->createTime > current->next->createTime) needSwap = true;
                }

                if (needSwap) {
                    // Меняем данные местами
                    swap(current->filename, current->next->filename);
                    swap(current->sizeKb, current->next->sizeKb);
                    swap(current->createTime, current->next->createTime);
                    swapped = true;
                }
                current = current->next;
            }
        } while (swapped);
    }

    bool publicIsValidInput(const string& input, bool isNumber = false) {
        return isValidInput(input, isNumber);
    }
    int publicStrToInt(const string& data) {
        return strToInt(data);
    }
};

void pressAnyKey() {
    cout << "\nНажмите любую клавишу для продолжения...";
    cin.ignore();
    cin.get();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "ru");

    MagneticTape tape;
    int choice = 0;

    while (true) {
        system("cls");
        cout << programmInfo << endl;

        for (int i = 0; i < 9; i++) {
            cout << choses[i] << endl;
        }

        cout << "\nВыберите пункт меню: ";
        string input;
        cin >> input;

        if (!tape.publicIsValidInput(input, true)) {
            cout << errors[1] << endl;
            pressAnyKey();
            continue;
        }

        if (input == "exit") break;
        choice = tape.publicStrToInt(input);

        switch (choice) {
        case 1: { // Добавить в конец
            system("cls");
            string name, sizeStr, time;

            cout << "Введите имя файла: ";
            cin >> name;
            cout << "Введите размер (Кб): ";
            cin >> sizeStr;
            cout << "Введите время создания: ";
            cin >> time;

            if (!tape.publicIsValidInput(sizeStr, true)) {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            tape.addToTail(name, tape.publicStrToInt(sizeStr), time);
            cout << "Запись добавлена." << endl;
            pressAnyKey();
            break;
        }
        case 2: { // Вставка по индексу
            system("cls");
            if (tape.isEmpty()) {
                cout << "Список пуст. Добавляем первый элемент." << endl;
            }
            else {
                tape.display();
            }

            string idxStr, name, sizeStr, time;
            cout << "\nВведите позицию для вставки (1-" << tape.getCount() + 1 << "): ";
            cin >> idxStr;

            if (!tape.publicIsValidInput(idxStr, true)) {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            cout << "Введите имя файла: ";
            cin >> name;
            cout << "Введите размер (Кб): ";
            cin >> sizeStr;
            cout << "Введите время создания: ";
            cin >> time;

            if (!tape.publicIsValidInput(sizeStr, true)) {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            if (tape.insertAt(tape.publicStrToInt(idxStr), name, tape.publicStrToInt(sizeStr), time)) {
                cout << "Запись успешно вставлена." << endl;
            }
            else {
                cout << errors[2] << endl;
            }
            pressAnyKey();
            break;
        }
        case 3: { // Вывод
            system("cls");
            tape.display();
            pressAnyKey();
            break;
        }
        case 4: { // Удаление
            system("cls");
            if (tape.isEmpty()) {
                cout << errors[3] << endl;
                pressAnyKey();
                break;
            }
            tape.display();
            cout << "\nВведите номер записи для удаления: ";
            string idxStr;
            cin >> idxStr;

            if (!tape.publicIsValidInput(idxStr, true)) {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            if (tape.deleteAt(tape.publicStrToInt(idxStr))) {
                cout << "Запись удалена." << endl;
            }
            else {
                cout << errors[2] << endl;
            }
            pressAnyKey();
            break;
        }
        case 5: { // Экспорт
            system("cls");
            if (tape.isEmpty()) {
                cout << errors[3] << endl;
                pressAnyKey();
                break;
            }
            cout << "Введите имя файла для сохранения (без .txt): ";
            string fname;
            cin >> fname;
            if (tape.exportToFile(fname)) {
                cout << "Успешно сохранено." << endl;
            }
            pressAnyKey();
            break;
        }
        case 6: { // Импорт
            system("cls");
            cout << "Введите имя файла для загрузки (без .txt): ";
            string fname;
            cin >> fname;
            if (tape.importFromFile(fname)) {
                cout << "Данные загружены." << endl;
            }
            pressAnyKey();
            break;
        }
        case 7: { // Поиск
            system("cls");
            if (tape.isEmpty()) {
                cout << errors[3] << endl;
                pressAnyKey();
                break;
            }
            cout << "Искать по:\n1. Имени\n2. Размеру\n3. Времени\nВаш выбор: ";
            string typeStr;
            cin >> typeStr;

            if (typeStr != "1" && typeStr != "2" && typeStr != "3") {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            cout << "Введите значение для поиска: ";
            string val;
            cin >> val;

            tape.search(tape.publicStrToInt(typeStr), val);
            pressAnyKey();
            break;
        }
        case 8: { // Сортировка
            system("cls");
            if (tape.isEmpty()) {
                cout << errors[3] << endl;
                pressAnyKey();
                break;
            }
            cout << "Сортировать по:\n1. Имени\n2. Размеру\n3. Времени\nВаш выбор: ";
            string typeStr;
            cin >> typeStr;
            if (typeStr != "1" && typeStr != "2" && typeStr != "3") {
                cout << errors[1] << endl;
                pressAnyKey();
                break;
            }

            tape.sort(tape.publicStrToInt(typeStr));
            cout << "Список отсортирован." << endl;
            pressAnyKey();
            break;
        }
        case 0: {
            return 0;
        }
        default: {
            cout << "Неверный пункт меню!" << endl;
            pressAnyKey();
            break;
        }
        }
    }
    return 0;
}