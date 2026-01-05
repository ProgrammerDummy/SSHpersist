#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <libssh/libsshpp.hpp>


class mySession {
    private:
        std::unique_ptr<ssh::Session> session_ptr;
        std::unique_ptr<ssh::Channel> channel_ptr;

    public:
        int port;
        std::string host;
        std::string username;
        std::string password;
        int authentication_type_flag; // this should be 0 if keys 1 if password
        int username_flag;
        int reconnect_flag;

        //maybe i can use bitwise operations to make a control integer that stores connection status and all flags?
        
        mySession(int p, std::string h, std::string un, std::string pw) {
            port = p;
            host = h;
            username = un;
            password = pw;
            username_flag = 1;
            session_ptr = std::make_unique<ssh::Session>();
            reconnect_flag = 0;
        }

        mySession(int p, std::string h, std::string pw) {
            port = p;
            host = h;
            password = pw;
            username_flag = 0;
            session_ptr = std::make_unique<ssh::Session>();
            reconnect_flag = 0;
        }

        ~mySession() {

        }

        int connect_and_auth() {

            session_ptr->setOption(SSH_OPTIONS_HOST, (const char*)host.c_str());
            session_ptr->setOption(SSH_OPTIONS_PORT, &port);

            int auth_status;

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

            if(reconnect_flag == 1) {

                goto reconnectpoint;
            }

            if(authentication_type_flag != 1) {
                auth_status = session_ptr->userauthNone();
            }

            while(auth_status != SSH_AUTH_SUCCESS) {

                std::cout << "Enter password: ";

                std::getline(std::cin, password);

                auth_status = session_ptr->userauthPassword(password.c_str());

                authentication_type_flag = 1;

            }

            return 1;


            reconnectpoint: //if this is for reconnection, then we should already have the password value which was stored for initial authentication

                //all we have to do is just check what kind of authentication was used

                if(authentication_type_flag == 1) {
                    while(auth_status != SSH_AUTH_SUCCESS) {

                        auth_status = session_ptr->userauthPassword(password.c_str());

                    }
                }

                return 1;

        }

        int connection_status_check() {
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

            if(connection_status_check() == 1) {
                return 1;
            }

            reset_channel(); 

            std::unique_ptr<ssh::Session> dummy_ptr = std::make_unique<ssh::Session>();

            std::swap(dummy_ptr, session_ptr);

            reconnect_flag = 1;

            return connect_and_auth();

        }

        int disconnect() {

            if(connection_status_check() == 0) {
                return 1;
            }

            session_ptr->disconnect();

            return 1;
        }

        
        void open_channel_and_shell() {

            channel_ptr = std::make_unique<ssh::Channel>(*session_ptr);

            channel_ptr->openSession();

            channel_ptr->requestPty();

            channel_ptr->changePtySize(10, 10);

            channel_ptr->requestShell();

            int bytes_read;
            int bytes_written;
            int user_bytes_read;
            std::vector<char> inbuf(4096);
            std::string outbuf;
            
            while(channel_ptr->isOpen() && !channel_ptr->isEof()) {
                bytes_read = channel_ptr->read(inbuf.data(), inbuf.size(), false, -1); //was ambiguous function call

                if(bytes_read < 0) {
                    return;
                }

                if(bytes_read > 0) {
                    std::cout.write(inbuf.data(), bytes_read);
                    std::cout.flush();
                }

                std::getline(std::cin, outbuf);

                channel_ptr->write(outbuf.data(), outbuf.size());
                channel_ptr->write("\n", 1);
            }

        }

        void close_channel() {

            if(channel_ptr->isClosed() == false) {
                channel_ptr->close();
            }
        }
            
        void reset_channel() {

        }
};