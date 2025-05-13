#include <../../libs/SQLAPI.h>
#include <iostream>

int main() {
    SAConnection con;

    try {
        // Replace with your DB info
        con.Connect("evotingsystem@127.0.0.1:3306", "root", "", SA_MySQL_Client);
        std::cout << "Connected successfully!" << std::endl;

        con.Disconnect();
    } catch (SAException &x) {
        std::cout << "Connection error: " << x.ErrText().GetMultiByteChars() << std::endl;
    }

    return 0;
}
