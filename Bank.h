#ifndef BANK_H
#define BANK_H

#include <iostream>
#include <string>
#include <fstream>
#include "Branch.h"

#define MAX_BRANCHES 5

class Bank {
private:
    std::string name;
    Branch *branches[MAX_BRANCHES];
    int front;
    int rear;
    int itemCount;

public:
    Bank(const std::string& bankName);
    ~Bank();

    bool isBranchQueueFull() const;
    bool isBranchQueueEmpty() const;
    void addBranch(int number);
    void removeBranch();
    void addAtmToBranch(int branchNumber, int atmNumber, const std::string& atmAddress);
    Branch* findBranch(int branchNumber) const;
    Branch* getBranch() const;
    Branch* getBranchByNumber(int branchNumber) const;
    std::string getName() const { return name; }
    void printBankInfo() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
};

Bank::Bank(const std::string& bankName) : name(bankName), front(0), rear(0), itemCount(0) {
    for (int i = 0; i < MAX_BRANCHES; ++i) {
        branches[i] = nullptr;
    }
}

Bank::~Bank() {
    for (int i = 0; i < MAX_BRANCHES; ++i) {
        if (branches[i] != nullptr) {
            delete branches[i];
            branches[i] = nullptr;
        }
    }
    delete[] *branches;
    *branches = nullptr;
}

void Bank::removeBranch() {
    if (isBranchQueueEmpty()) {
        std::cout << "Нет филиалов для удаления.\n";
        return;
    }
    delete branches[front];
    branches[front] = nullptr;
    front++;
    if (front == MAX_BRANCHES) front = 0;
    itemCount--;
    std::cout << "Филиал удален.\n";
}

void Bank::printBankInfo() const {
    std::cout << "Информация о банке " << name << ":\n";
    if (isBranchQueueEmpty()) {
        std::cout << "Нет филиалов для отображения.\n";
        return;
    }
    
    int orderChoice;
    std::cout << "Выберите порядок вывода (0 - прямой, 1 - обратный): ";
    std::cin >> orderChoice;

    bool reverseOrder = (orderChoice == 1);

    int i = front;
    int count = 0;
    do {
        if (branches[i] != nullptr) {
            std::cout << "Филиал Номер: " << branches[i]->getBranchID() << '\n';
            branches[i]->printAtms(reverseOrder);
            count++;
        }
        i = (i + 1) % MAX_BRANCHES;
    } while (count < itemCount);
}


bool Bank::isBranchQueueFull() const {
    return itemCount == MAX_BRANCHES;
}

bool Bank::isBranchQueueEmpty() const {
    return itemCount == 0;
}

void Bank::addBranch(int number) {
    if (isBranchQueueFull()) {
        std::cout << "Максимальное количество филиалов достигнуто.\n";
        return;
    }
    Branch* newBranch = new Branch(number);
    branches[rear] = newBranch;
    rear++;
    if (rear == MAX_BRANCHES) rear = 0;
    itemCount++;
    std::cout << "Филиал добавлен.\n";
}

Branch* Bank::getBranch() const {
    return branches[front];
}

Branch* Bank::getBranchByNumber(int branchNumber) const {
    for (int i = 0; i < MAX_BRANCHES; ++i) {
        if (branches[i] != nullptr && branches[i]->getBranchID() == branchNumber) {
            return branches[i];
        }
    }
    return nullptr;
}


void Bank::addAtmToBranch(int branchNumber, int atmNumber, const std::string& atmAddress) {
    Branch* branch = findBranch(branchNumber);
    if (branch) {
        if (branch->isAtmListEmpty()) {
            branch->addAtm(atmNumber, atmAddress, -1, true);
        } else {
            int referenceAtmNumber;
            std::cout << "Введите номер банкомата, относительно которого хотите добавить новый банкомат: ";
            std::cin >> referenceAtmNumber;
            char positionChoice;
            std::cout << "Добавить банкомат до (введите 'B') или после (введите 'A') указанного банкомата: ";
            std::cin >> positionChoice;
            bool addAfter = (positionChoice == 'A' || positionChoice == 'a');
            branch->addAtm(atmNumber, atmAddress, referenceAtmNumber, addAfter);
        }
    } else {
        std::cout << "Филиал с таким номером не найден.\n";
    }
}


Branch* Bank::findBranch(int branchNumber) const {
    for (int i = 0; i < itemCount; ++i) {
        if (branches[i]->getBranchID() == branchNumber) {
            return branches[i];
        }
    }
    return nullptr;
}


void Bank::saveToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Ошибка при открытии файла для записи.\n";
        return;
    }
    outFile << name << '\n';
    outFile << itemCount << '\n';
    for (int i = 0; i < itemCount; ++i) {
        branches[i]->save(outFile); 
    }
    outFile.close();
}

void Bank::loadFromFile(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Ошибка при открытии файла для чтения.\n";
        return;
    }
    std::getline(inFile, name);
    int branchCount;
    inFile >> branchCount;
    inFile.ignore();
    for (int i = 0; i < branchCount; ++i) {
        Branch* branch = Branch::loadFromFile(inFile);
        if (branch) {
            branches[rear] = branch;
            rear = (rear + 1) % MAX_BRANCHES;
            itemCount++;
        }
    }
    inFile.close();
}

#endif // BANK_H

