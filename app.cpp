#include "app.hpp"

int main() {

    mySession testsession(22, "127.0.0.1", "Matches");

    if(testsession.connect_and_auth() == 1) {
        std::cout << "successfully connected\n";
    }

    std::cout << testsession.disconnect() << "\n";

    std::cout << testsession.reconnect() << std::endl;



}


void oldcode() {
    
}
