#ifndef BRANCH_H
#define BRANCH_H

#include <iostream>
#include <string>
#include "Atm.h"

class Branch {
private:
    int branchID;
    Atm *firstAtm;

public:
    Branch(int id);
    ~Branch();

    int getBranchID() const { return branchID; }
    void addAtm(int number, const std::string& address, int referenceAtmNumber, bool addAfter);
    void removeAtm(int number);
    Atm* findAtm(int number) const;
    void printAtms(bool reverseOrder) const;
    bool isAtmListEmpty() const;
    void save(std::ofstream& outFile) const;
    static Branch* loadFromFile(std::ifstream& inFile);
};

Branch::Branch(int id) : branchID(id), firstAtm(nullptr) {}

Branch::~Branch() {
    while (firstAtm != nullptr) {
        Atm* temp = firstAtm;
        firstAtm = firstAtm->getNext();
        delete temp;
    }
}

bool Branch::isAtmListEmpty() const {
    return firstAtm == nullptr;
}

void Branch::addAtm(int number, const std::string& address, int referenceAtmNumber, bool addAfter) {
    if (findAtm(number) != nullptr) {
        std::cout << "Банкомат с таким номером уже существует.\n";
        return;
    }
    Atm* newAtm = new Atm(number, address);
    if (isAtmListEmpty()) {
        firstAtm = newAtm;
    } else {
        Atm* referenceAtm = findAtm(referenceAtmNumber);
        if (!referenceAtm) {
            std::cout << "Банкомат для ссылки не найден. Добавление в конец списка.\n";
            Atm* current = firstAtm;
            while (current->getNext() != nullptr) {
                current = current->getNext();
            }
            current->setNext(newAtm);
            newAtm->setPrev(current);
            return;
        }
        
        if (addAfter) {
            newAtm->setNext(referenceAtm->getNext());
            newAtm->setPrev(referenceAtm);
            if (referenceAtm->getNext() != nullptr) {
                referenceAtm->getNext()->setPrev(newAtm);
            }
            referenceAtm->setNext(newAtm);
        } else {
            newAtm->setNext(referenceAtm);
            newAtm->setPrev(referenceAtm->getPrev());
            if (referenceAtm->getPrev() != nullptr) {
                referenceAtm->getPrev()->setNext(newAtm);
            } else {
                firstAtm = newAtm;
            }
            referenceAtm->setPrev(newAtm);
        }
    }
    std::cout << "Банкомат добавлен.\n";
}

void Branch::removeAtm(int number) {
    Atm* atmToDelete = findAtm(number);
    if (atmToDelete == nullptr) {
        std::cout << "Банкомат с таким номером не найден.\n";
        return;
    }
    if (atmToDelete == firstAtm) {
        firstAtm = atmToDelete->getNext();
    }
    if (atmToDelete->getPrev() != nullptr) {
        atmToDelete->getPrev()->setNext(atmToDelete->getNext());
    }
    if (atmToDelete->getNext() != nullptr) {
        atmToDelete->getNext()->setPrev(atmToDelete->getPrev());
    }
    delete atmToDelete;
    std::cout << "Банкомат удален.\n";
}

Atm* Branch::findAtm(int number) const {
    Atm* current = firstAtm;
    while (current != nullptr) {
        if (current->getNumber() == number) {
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void Branch::printAtms(bool reverseOrder) const {
    if (firstAtm == nullptr) {
        std::cout << "Нет банкоматов для отображения.\n";
        return;
    }

    std::cout << "Банкоматы:\n";
    if (!reverseOrder) {
        Atm* current = firstAtm;
        while (current != nullptr) {
            std::cout << "  Банкомат Номер: " << current->getNumber() << ", Адрес: " << current->getAddress() << '\n';
            current = current->getNext();
        }
    } else {
        Atm* current = firstAtm;
        while (current->getNext() != nullptr) {
            current = current->getNext();
        }
        while (current != nullptr) {
            std::cout << "  Банкомат Номер: " << current->getNumber() << ", Адрес: " << current->getAddress() << '\n';
            current = current->getPrev();
        }
    }
}

void Branch::save(std::ofstream& outFile) const {
    outFile << branchID << '\n';
    Atm* current = firstAtm;
    while (current != nullptr) {
        current->save(outFile);
        current = current->getNext();
    }
    outFile << "-1\n";
}

Branch* Branch::loadFromFile(std::ifstream& inFile) {
    int id;
    if (inFile >> id) {
        inFile.ignore();
        Branch* branch = new Branch(id);
        
        while (true) {
            int atmNumber;
            std::string atmAddress;
            inFile >> atmNumber;
            inFile.ignore();
            if (atmNumber == -1) break;
            std::getline(inFile, atmAddress);
            branch->addAtm(atmNumber, atmAddress, -1, true);
        }
        
        return branch;
    }
    return nullptr;
}

#endif // BRANCH_H
