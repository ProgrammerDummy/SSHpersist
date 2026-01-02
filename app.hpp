#include <iostream>
#include <string>
#include <memory>
#include <libssh/libsshpp.hpp>


class mySession {
    private:
        std::unique_ptr<ssh::Session> session_ptr;

    public:
        int port;
        std::string host;
        std::string username;
        std::string password;
        int connection_status;
        int authentication_type_flag; // this should be 0 if keys 1 if password
        int username_flag;
        
        mySession(int p, std::string h, std::string un, std::string pw) {
            port = p;
            host = h;
            username = un;
            password = pw;
            username_flag = 1;
            session_ptr = std::make_unique<ssh::Session>();
        }

        mySession(int p, std::string h, std::string pw) {
            port = p;
            host = h;
            password = pw;
            username_flag = 0;
            session_ptr = std::make_unique<ssh::Session>();
        }

        ~mySession() {

        }

        int connect_and_auth() {

            session_ptr->setOption(SSH_OPTIONS_HOST, (const char*)host.c_str());
            session_ptr->setOption(SSH_OPTIONS_PORT, &port);

            if(username_flag == 1) {
                session_ptr->setOption(SSH_OPTIONS_USER, (const char*)username.c_str());
            }

            try {
            session_ptr->connect();
            }
            catch(ssh::SshException &exception) {
                std::cerr << exception.getError() << "\n";
                return 0;
            }

            connection_status = session_ptr->userauthNone();

            while(connection_status != SSH_AUTH_SUCCESS) {

                std::cout << "Enter password: ";
        
                std::getline(std::cin, password);

                connection_status = session_ptr->userauthPassword(password.c_str());

                authentication_type_flag = 1;

            }

            return 1;

        }

        int connectionStatus() {
            ssh_session unwrapped_session = session_ptr->getCSession();

            return ssh_is_connected(unwrapped_session); //0 if disconnected, 1 if connected
        }

        int reconnect() {
            // 1) create a fresh session (where? how owned?)
            // 2) apply options + connect + auth
            // 3) replace the old session atomically (or safely)

            //no copy constructor
            //cant do ssh_free on underlying C ssh connection and then do ssh_new() --> not assignable


            //use pointers 

            return 1;


        }

        int disconnect() {
            session_ptr->disconnect();

            return 1;
        }

};