#include "DataBase.h"
#include <iostream>

int main() {
    MYSQL* conn = mysql_init(nullptr);
    if (conn == nullptr) {
        std::cerr << "mysql_init failed!" << std::endl;
        return -1;
    }
    std::cout << "MySQL initialized successfully!" << std::endl;
    return 0;
}
