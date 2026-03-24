#include <iostream>
#include <string>
#include <Windows.h>
#include "MagneticTape.h"

using namespace std; // Допустимо только в .cpp

bool isValidNumber(const string& s) {
    if (s.empty()) return false;
    for (char c : s) if (!isdigit(c)) return false;
    return true;
}

void pressAnyKey() {
    cout << "\nНажмите Enter для продолжения...";
    cin.ignore(cin.rdbuf()->in_avail());
    cin.get();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    MagneticTape tape;
    string input;

    while (true) {
        system("cls");
        cout << programmInfo << endl;
        for (const auto& item : choses) cout << item << endl;
        cout << "\nВыберите пункт: ";
        cin >> input;

        if (!isValidNumber(input)) {
            cout << errors[1] << endl;
            pressAnyKey();
            continue;
        }

        int choice = stoi(input);
        if (choice == 0) break;

        switch (choice) {
        case 1: { // Добавление
            string n, s, t;
            cout << "Имя: "; cin >> n;
            cout << "Размер (Кб): "; cin >> s;
            cout << "Время: "; cin >> t;
            if (isValidNumber(s)) {
                tape.addToTail(n, stoi(s), t);
                cout << "Добавлено.";
            }
            else cout << errors[1];
            break;
        }
        case 2: { // Вставка
            string idx, n, s, t;
            cout << "Позиция (1-" << tape.getCount() + 1 << "): "; cin >> idx;
            if (!isValidNumber(idx)) { cout << errors[1]; break; }
            cout << "Имя: "; cin >> n;
            cout << "Размер: "; cin >> s;
            cout << "Время: "; cin >> t;
            if (isValidNumber(s)) {
                if (!tape.insertAt(stoi(idx), n, stoi(s), t)) cout << errors[2];
                else cout << "Вставлено.";
            }
            else cout << errors[1];
            break;
        }
        case 3: tape.display(); break;
        case 4: { // Удаление
            if (tape.isEmpty()) { cout << errors[3]; break; }
            tape.display();
            cout << "Номер для удаления: "; cin >> input;
            if (isValidNumber(input)) {
                if (!tape.deleteAt(stoi(input))) cout << errors[2];
            }
            else cout << errors[1];
            break;
        }
        case 5: { // Экспорт
            cout << "Имя файла: "; cin >> input;
            if (tape.exportToFile(input)) cout << "Сохранено.";
            break;
        }
        case 6: { // Импорт
            cout << "Имя файла: "; cin >> input;
            if (tape.importFromFile(input)) cout << "Загружено.";
            break;
        }
        case 7: { // Поиск
            cout << "1. Имя, 2. Размер, 3. Время: "; cin >> input;
            if (input < "1" || input > "3") { cout << errors[1]; break; }
            string val; cout << "Значение: "; cin >> val;
            tape.search(stoi(input), val);
            break;
        }
        case 8: { // Сортировка
            cout << "1. Имя, 2. Размер, 3. Время: "; cin >> input;
            if (input >= "1" && input <= "3") {
                tape.sort(stoi(input));
                cout << "Отсортировано.";
            }
            else cout << errors[1];
            break;
        }
        default: cout << "Неверный пункт!";
        }
        pressAnyKey();
    }
    return 0;
}
