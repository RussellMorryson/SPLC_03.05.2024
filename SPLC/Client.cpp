
#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

class Client {
private:
    string name;
    string server_ip;
    int port;
    string s_name;

public:
    Client(string name, string ip, int port) {
        this->name = name;
        this->server_ip = ip;
        this->port = port;
    }

    string getData(tcp::socket& socket) {        
        boost::asio::streambuf buf;
        read_until(socket, buf, "\n");
        std::string data = buffer_cast<const char*>(buf.data());
        return data;
    }

    void sendData(tcp::socket& socket, const string& message) {
        boost::asio::write(socket, boost::asio::buffer(message + "\n"));
    }

    int start() {        
        io_service io_service;
        tcp::socket client_socket(io_service);
        client_socket.connect(tcp::endpoint(address::from_string(this->server_ip), this->port));
    
        boost::system::error_code error;
        boost::asio::write(client_socket, boost::asio::buffer(this->name + "\n"), error);
        if (error) {
            cout << "Connection is failed: " << error.message() << endl;
            return -1;
        }
            
        this->s_name = getData(client_socket);
        this->s_name.pop_back();
        cout << this->s_name << " joined!" << endl;
    
        string reply, response;
        int count = 0;
        while (true) {
            cout << this->name << ": ";
            getline(cin, reply);
            sendData(client_socket, reply);
            if (reply == "exit")
                break;
            
            cout << "..." << endl;
            response = getData(client_socket);
            response.pop_back();
            if (response == "exit") {
                cout << "The connection is broken!" << endl;
                break;
            }
            cout << this->s_name << ": " << response << endl;
    
            response.clear();
            reply.clear();            
        }
        return 0;
    }    
    ~Client() {}
};