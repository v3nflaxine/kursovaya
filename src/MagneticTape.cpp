#include "MagneticTape.h"

// --- Приватные методы ---

std::vector<std::string> MagneticTape::splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
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

MagneticTape::~MagneticTape() { clearAll(); }

void MagneticTape::clearAll() {
    TapeNode* current = head;
    while (current != nullptr) {
        TapeNode* temp = current;
        current = current->next;
        delete temp;
    }
    head = tail = nullptr;
    count = 0;
}

bool MagneticTape::isEmpty() const { return head == nullptr; }
int MagneticTape::getCount() const { return count; }

void MagneticTape::addToTail(std::string name, int size, std::string time) {
    TapeNode* newNode = new TapeNode(name, size, time);
    if (!head) {
        head = tail = newNode;
    }
    else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    count++;
}

bool MagneticTape::insertAt(int index, std::string name, int size, std::string time) {
    if (index < 1 || index > count + 1) return false;
    if (index == count + 1) {
        addToTail(name, size, time);
        return true;
    }

    TapeNode* newNode = new TapeNode(name, size, time);
    if (index == 1) {
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
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

    if (toDelete == head && toDelete == tail) {
        head = tail = nullptr;
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
        std::cout << errors[3] << std::endl;
        return;
    }
    std::cout << "№\tРазмер(Кб)\tВремя\t\tИмя файла\n" << std::string(52, '-') << std::endl;
    TapeNode* current = head;
    for (int i = 1; current; i++, current = current->next) {
        std::cout << i << "\t" << current->sizeKb << "\t\t" << current->createTime << "\t\t" << current->filename << std::endl;
    }
}

bool MagneticTape::exportToFile(const std::string& filename) {
    std::ofstream file(filename + ".txt");
    if (!file.is_open()) return false;
    for (TapeNode* curr = head; curr; curr = curr->next) {
        file << curr->filename << ";" << curr->sizeKb << ";" << curr->createTime << "\n";
    }
    return true;
}

bool MagneticTape::importFromFile(const std::string& filename) {
    std::ifstream file(filename + ".txt");
    if (!file.is_open()) {
        std::cout << errors[4] << std::endl;
        return false;
    }
    clearAll();
    std::string line;
    while (std::getline(file, line)) {
        auto parts = splitString(line, ';');
        if (parts.size() >= 3) {
            try {
                addToTail(parts[0], std::stoi(parts[1]), parts[2]);
            }
            catch (...) { continue; }
        }
    }
    return true;
}

void MagneticTape::search(int searchType, const std::string& value) {
    if (isEmpty()) return;
    TapeNode* curr = head;
    bool found = false;
    for (int i = 1; curr; i++, curr = curr->next) {
        bool match = false;
        if (searchType == 1 && curr->filename == value) match = true;
        else if (searchType == 2 && std::to_string(curr->sizeKb) == value) match = true;
        else if (searchType == 3 && curr->createTime == value) match = true;

        if (match) {
            std::cout << "Найдено [" << i << "]: " << curr->filename << " (" << curr->sizeKb << " Kb)\n";
            found = true;
        }
    }
    if (!found) std::cout << errors[5] << std::endl;
}

void MagneticTape::sort(int fieldType) {
    if (!head || !head->next) return;
    bool swapped;
    do {
        swapped = false;
        for (TapeNode* curr = head; curr->next; curr = curr->next) {
            bool need = false;
            if (fieldType == 1 && curr->filename > curr->next->filename) need = true;
            else if (fieldType == 2 && curr->sizeKb > curr->next->sizeKb) need = true;
            else if (fieldType == 3 && curr->createTime > curr->next->createTime) need = true;

            if (need) {
                std::swap(curr->filename, curr->next->filename);
                std::swap(curr->sizeKb, curr->next->sizeKb);
                std::swap(curr->createTime, curr->next->createTime);
                swapped = true;
            }
        }
    } while (swapped);
}
