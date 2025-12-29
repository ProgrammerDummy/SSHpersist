#include <iostream>
#include <libssh/libsshpp.hpp>


class mySession {
    private:
        ssh::Session my_ssh_session;

    public:
        int port;
        const char* host;
        const char* username;
        std::string password;
        int connection_status;
        int authentication_type; // this should be 0 if keys 1 if password
        
        mySession(int p, const char* h, std::string pw) {
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

            connection_status = testsession.userauthNone();

            while(connection_status != SSH_AUTH_SUCCESS) {

                std::cout << "Enter password: ";
        
                std::getline(std::cin, password);

                connection_status = testsession.userauthPassword(password.c_str());

                authentication_type_flag = 1;

            }

            return 1;

        }

        int connectionStatus() {
            ssh_session unwrapped_session = my_ssh_session.getCSession();

            return ssh_is_connected(unwrapped_session); //0 if disconnected, 1 if connected
        }

        int reconnect() {
            
        }

        int disconnect() {

        }

};