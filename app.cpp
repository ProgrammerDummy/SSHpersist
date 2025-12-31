#include "app.hpp"

int main() {

    mySession testsession(22, "127.0.0.1", "Matches");

    if(testsession.connect_and_auth() == 1) {
        std::cout << "successfully connected";
    }



}

void stuff(void) {

    unsigned int port = 22;
    const char* host = "127.0.0.1";
    std::string password = "Matches";
    
    int authentication_type_flag = 0; //build this into the custom class later

    /*

    these are test options which will be able to be changed according to which ssh server the user wants to connect to.
    */


    ssh::Session testsession;
    ssh_session rawCtestsession = testsession.getCSession();

    testsession.setOption(SSH_OPTIONS_HOST, host);
    testsession.setOption(SSH_OPTIONS_PORT, &port);


    try {
        testsession.connect();
    }
    catch(ssh::SshException &exception) {
        std::cerr << exception.getError() << "\n";
    }

    int connection_status = testsession.userauthNone();

    while(connection_status != SSH_AUTH_SUCCESS) {

        std::cout << "Enter password: ";
        
        std::getline(std::cin, password);

        connection_status = testsession.userauthPassword(password.c_str());

        authentication_type_flag = 1;

    }




    
    std::cout << "connection succeeded\n";

    std::cout << "connection status: " << ssh_is_connected(rawCtestsession) << std::endl;

    testsession.disconnect();

    std::cout << "connection status: " << ssh_is_connected(rawCtestsession) << std::endl;

    std::cout << "disconnected\n";
    





    if(ssh_is_connected(rawCtestsession) == 0) {

        ssh::Session testsession; 
        testsession.setOption(SSH_OPTIONS_HOST, host);
        testsession.setOption(SSH_OPTIONS_PORT, &port);

        try {
            testsession.connect();
        }
        catch(ssh::SshException &exception) {
            std::cerr << exception.getError() << "\n";
        }

        connection_status = testsession.userauthPassword(password.c_str()); //reconnect
        std::cout << "reconnected";
    }

}