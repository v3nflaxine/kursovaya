#include <iostream>
#include <Windows.h>
#include "MagneticTape.h" 
#include "Constants.h"    

using namespace std;

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
