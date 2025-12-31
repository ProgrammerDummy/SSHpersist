#include <iostream>
#include <string>
#include <memory>
#include <libssh/libsshpp.hpp>


class mySession {
    private:

        ssh::Session my_ssh_session; //can this be copied/assigned/moved?

    public:
        int port;
        char* host;
        char* username;
        std::string password;
        int connection_status;
        int authentication_type_flag; // this should be 0 if keys 1 if password
        
        mySession(int p, char* h, std::string pw) {
            port = p;
            host = h;
            password = pw;
        }

        int connect_and_auth() {
            my_ssh_session.setOption(SSH_OPTIONS_HOST, host);
            my_ssh_session.setOption(SSH_OPTIONS_PORT, &port);
            my_ssh_session.setOption(SSH_OPTIONS_USER, username);

            try {
            my_ssh_session.connect();
            }
            catch(ssh::SshException &exception) {
                std::cerr << exception.getError() << "\n";
                return 0;
            }

            connection_status = my_ssh_session.userauthNone();

            while(connection_status != SSH_AUTH_SUCCESS) {

                std::cout << "Enter password: ";
        
                std::getline(std::cin, password);

                connection_status = my_ssh_session.userauthPassword(password.c_str());

                authentication_type_flag = 1;

            }

            return 1;

        }

        int connectionStatus() {
            ssh_session unwrapped_session = my_ssh_session.getCSession();

            return ssh_is_connected(unwrapped_session); //0 if disconnected, 1 if connected
        }

        int reconnect() {
            // 1) create a fresh session (where? how owned?)
            // 2) apply options + connect + auth
            // 3) replace the old session atomically (or safely)

            //no copy constructor
            //cant do ssh_free on underlying C ssh connection and then do ssh_new() --> not assignable


            //use pointers 


            ssh::Session dummy_ssh_session;







            my_ssh_session.setOption(SSH_OPTIONS_HOST, host);
            my_ssh_session.setOption(SSH_OPTIONS_PORT, &port);
            my_ssh_session.setOption(SSH_OPTIONS_USER, username);

            try {
            my_ssh_session.connect();
            }
            catch(ssh::SshException &exception) {
                std::cerr << exception.getError() << "\n";
                return 0;
            }

            connection_status = my_ssh_session.userauthNone();

            while(connection_status != SSH_AUTH_SUCCESS) {

                connection_status = my_ssh_session.userauthPassword(password.c_str());

                authentication_type_flag = 1;

            }

            return 1;



        }

        int disconnect() {
            my_ssh_session.disconnect();

            return 1;
        }

};