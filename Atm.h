#ifndef ATM_H
#define ATM_H

#include <iostream>
#include <string>
#include <fstream>

class Atm {
private:
    int number;
    std::string address;
    Atm *next, *prev;

public:
    Atm(int num, const std::string& addr);
    ~Atm();

    int getNumber() const { return number; }
    std::string getAddress() const { return address; }

    Atm* getNext() const { return next; }
    Atm* getPrev() const { return prev; }

    void setNext(Atm* nextAtm) { next = nextAtm; }
    void setPrev(Atm* prevAtm) { prev = prevAtm; }

    void save(std::ofstream& outFile) const;
    static Atm* load(std::ifstream& inFile);
};

Atm::Atm(int num, const std::string& addr) : number(num), address(addr), next(nullptr), prev(nullptr) {}

Atm::~Atm() {
    next = nullptr;
    prev = nullptr;
}

void Atm::save(std::ofstream& outFile) const {
    outFile << number << '\n' << address << '\n';
}

Atm* Atm::load(std::ifstream& inFile) {
    int num;
    std::string addr;
    if (inFile >> num && std::getline(inFile, addr) && std::getline(inFile, addr)) {
        return new Atm(num, addr);
    }
    return nullptr;
}

#endif // ATM_H
