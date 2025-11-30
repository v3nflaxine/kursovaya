#include "MagneticTape.h"

// --- Приватные методы ---

int MagneticTape::strToInt(string data) {
    int newData = 0;
    for (int i = 0; i < data.size(); i++) {
        if (data[i] >= '0' && data[i] <= '9') {
            newData = newData * 10 + (data[i] - '0');
        }
    }
    return newData;
}

bool MagneticTape::isValidInput(const string& input, bool isNumber) {
    if (input.empty()) return false;
    if (input == "exit") return true;

    if (isNumber) {
        for (char c : input) {
            if (c < '0' || c > '9') return false;
        }
        return true;
    }
    return true;
}

vector<string> MagneticTape::splitString(const string& str, char delimiter) {
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

MagneticTape::TapeNode* MagneticTape::getNodeAtIndex(int index) {
    if (index < 1 || index > count) return nullptr;

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

// --- Публичные методы ---

MagneticTape::MagneticTape() : head(nullptr), tail(nullptr), count(0) {}

MagneticTape::~MagneticTape() {
    clearAll();
}

void MagneticTape::clearAll() {
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

bool MagneticTape::isEmpty() const {
    return head == nullptr;
}

int MagneticTape::getCount() const {
    return count;
}

void MagneticTape::addToTail(string name, int size, string time) {
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

bool MagneticTape::insertAt(int index, string name, int size, string time) {
    if (index < 1 || index > count + 1) return false;

    if (index == count + 1) {
        addToTail(name, size, time);
        return true;
    }

    TapeNode* newNode = new TapeNode(name, size, time);

    if (index == 1) {
        newNode->next = head;
        if (head != nullptr) {
            head->prev = newNode;
        }
        head = newNode;
        if (tail == nullptr) tail = newNode;
    }
    else {
        TapeNode* current = getNodeAtIndex(index);
        TapeNode* previous = current->prev;

        previous->next = newNode;
        newNode->prev = previous;
        newNode->next = current;
        current->prev = newNode;
    }
    count++;
    return true;
}

bool MagneticTape::deleteAt(int index) {
    if (index < 1 || index > count) return false;

    TapeNode* toDelete = getNodeAtIndex(index);

    if (head == tail) {
        head = nullptr;
        tail = nullptr;
    }
    else if (toDelete == head) {
        head = head->next;
        head->prev = nullptr;
    }
    else if (toDelete == tail) {
        tail = tail->prev;
        tail->next = nullptr;
    }
    else {
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
    }

    delete toDelete;
    count--;
    return true;
}

void MagneticTape::display() const {
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

bool MagneticTape::exportToFile(const string& filename) {
    string fullName = filename + ".txt";
    ofstream file(fullName);

    if (!file.is_open()) {
        cout << "Ошибка создания файла!" << endl;
        return false;
    }

    TapeNode* current = head;
    while (current != nullptr) {
        file << current->filename << ";"
            << current->sizeKb << ";"
            << current->createTime << endl;
        current = current->next;
    }
    file.close();
    return true;
}

bool MagneticTape::importFromFile(const string& filename) {
    string fullName = filename + ".txt";
    ifstream file(fullName);
    if (!file.is_open()) {
        cout << errors[4] << endl;
        return false;
    }

    clearAll();

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

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

void MagneticTape::search(int searchType, string value) {
    if (isEmpty()) {
        cout << errors[3] << endl;
        return;
    }

    TapeNode* current = head;
    bool found = false;
    int index = 1;

    while (current != nullptr) {
        bool isMatch = false;
        if (searchType == 1) {
            if (current->filename == value) isMatch = true;
        }
        else if (searchType == 2) {
            if (current->sizeKb == strToInt(value)) isMatch = true;
        }
        else if (searchType == 3) {
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

void MagneticTape::sort(int fieldType) {
    if (head == nullptr || head->next == nullptr) return;

    bool swapped;
    do {
        swapped = false;
        TapeNode* current = head;

        while (current->next != nullptr) {
            bool needSwap = false;

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
                swap(current->filename, current->next->filename);
                swap(current->sizeKb, current->next->sizeKb);
                swap(current->createTime, current->next->createTime);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
}

bool MagneticTape::publicIsValidInput(const string& input, bool isNumber) {
    return isValidInput(input, isNumber);
}

int MagneticTape::publicStrToInt(const string& data) {
    return strToInt(data);
}
