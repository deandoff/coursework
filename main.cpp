#include <iostream>
#include <string>
#include "Bank.h"

void printMenu(const Bank& bank) {
    std::cout << "============================\n";
    std::cout << "Меню банка: " << bank.getName() << "\n";
    std::cout << "============================\n";
    std::cout << "1 - Добавить филиал\n";
    std::cout << "2 - Удалить филиал\n";
    std::cout << "3 - Добавить банкомат\n";
    std::cout << "4 - Удалить банкомат\n";
    std::cout << "5 - Найти банкомат\n";
    std::cout << "6 - Показать информацию о банке\n";
    std::cout << "7 - Сохранить банк в файл\n";
    std::cout << "8 - Загрузить банк из файла\n";
    std::cout << "0 - Завершить работу программы\n";
    std::cout << "============================\n";
    std::cout << "Выберите опцию: ";
}


template <typename T>
void ProtectedInput(T* input) {
    while (!(std::cin >> *input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Некорректный ввод. Попробуйте еще раз: ";
    }
}

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main() {
    clearConsole();
    std::string bankName;
    std::cout << "Добро пожаловать в систему управления банком.\n";
    std::cout << "Введите название банка: ";
    std::getline(std::cin, bankName);

    Bank bank(bankName);

    int choice = -1;
    while (choice != 0) {
        printMenu(bank);
        ProtectedInput(&choice);

        switch (choice) {
            case 1: {
                if (!bank.isBranchQueueFull()) {
                    clearConsole();
                    int branchID;
                    std::cout << "Введите номер филиала: ";
                    ProtectedInput(&branchID);
                    bank.addBranch(branchID);
                } else {
                    std::cout << "Очередь филиалов переполнена\n";
                }
                break;
            }
            case 2: {
                if (!bank.isBranchQueueEmpty()) {
                    clearConsole();
                    bank.removeBranch();
                } else {
                    std::cout << "Очередь филиалов пуста. Нечего удалять\n";
                }
                break;
            }
            case 3: {
                if (!bank.isBranchQueueEmpty()) {
                    clearConsole();
                    int branchID, atmNumber;
                    std::string atmAddress;
                    std::cout << "Введите номер филиала: ";
                    ProtectedInput(&branchID);
                    if (!bank.findBranch(branchID)) {
                        std::cout << "Филиал не найден.\n";
                        break;
                    }
                    std::cout << "Введите номер банкомата: ";
                    ProtectedInput(&atmNumber);
                    if (bank.getBranchByNumber(branchID)->findAtm(atmNumber)) {
                        std::cout << "Банкомат с таким номером уже есть в филиале.\n";
                        break; 
                    }
                    std::cout << "Введите адрес банкомата: ";
                    std::cin.ignore();
                    std::getline(std::cin, atmAddress);
                    bank.addAtmToBranch(branchID, atmNumber, atmAddress);
                } else {
                    std::cout << "Филиалы не найдены. Добавьте филиал с помощью команды 1\n";
                }
                break;
            }
            case 4: {
                if (!bank.isBranchQueueEmpty()) {
                    clearConsole();
                    int branchID, atmNumber;
                    std::cout << "Введите номер филиала: ";
                    ProtectedInput(&branchID);
                    Branch* branch = bank.getBranchByNumber(branchID);
                    if (branch) {
                        std::cout << "Введите номер банкомата: ";
                        ProtectedInput(&atmNumber);
                        branch->removeAtm(atmNumber);
                    } else {
                        std::cout << "Филиал не найден.\n";
                    }
                } else {
                    std::cout << "Филиалы не найдены. Добавьте филиал с помощью команды 1\n";
                }
                break;
            }
            case 5: {
                if (!bank.isBranchQueueEmpty()) {
                    clearConsole();
                    int branchID, atmNumber;
                    std::cout << "Введите номер филиала: ";
                    ProtectedInput(&branchID);
                    Branch* branch = bank.findBranch(branchID);
                    if (branch) {
                        std::cout << "Введите номер банкомата: ";
                        ProtectedInput(&atmNumber);
                        Atm* atm = branch->findAtm(atmNumber);
                        if (atm) {
                            std::cout << "Банкомат Номер: " << atm->getNumber() << ", Адрес: " << atm->getAddress() << '\n';
                        } else {
                            std::cout << "Банкомат с номером " << atmNumber << " не найден.\n";
                        }
                    } else {
                        std::cout << "Филиал не найден.\n";
                    }
                } else {
                    std::cout << "Филиалы не найдены. Добавьте филиал с помощью команды 1\n";
                }
                break;
            }
            case 6: {
                clearConsole();
                bank.printBankInfo();
                break;
            }
            case 7: {
                clearConsole();
                std::string filename;
                std::cout << "Введите имя файла для сохранения: ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                bank.saveToFile(filename);
                std::cout << "Данные банка сохранены в файл.\n";
                break;
            }
            case 8: {
                while (!bank.isBranchQueueEmpty())
                {
                    bank.removeBranch();
                }
                clearConsole();
                std::string filename;
                std::cout << "Введите имя файла для загрузки: ";
                std::cin.ignore();
                std::getline(std::cin, filename);
                bank.loadFromFile(filename);
                clearConsole();
                std::cout << "Данные банка загружены из файла.\n";
                clearConsole();
                break;
            }
            case 0: {
                std::cout << "Завершение работы программы.\n";
                break;
            }
            default: {
                std::cout << "Неверная опция. Попробуйте снова.\n";
                break;
            }
        }
    }

    return 0;
}
