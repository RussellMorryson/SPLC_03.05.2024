
#include <boost/asio.hpp>
#include <iostream>

using namespace std;
using namespace boost;
using namespace boost::asio;
using namespace boost::asio::ip;

class Server {
private:
    string name;
    string ip;
    int port;
    string c_name;

public:
    Server(string name, string ip, int port) {
        this->name = name;
        this->ip = ip;
        this->port = port;
    }

    string getData(tcp::socket& socket) {
        boost::asio::streambuf buf;
        read_until(socket, buf, "\n");
        string data = buffer_cast<const char*>(buf.data());
        return data;
    }

    void sendData(tcp::socket& socket, const string& message) {
        boost::asio::write(socket, buffer(message + "\n"));
    }

    void start() {
        
        io_service io_service;
        tcp::acceptor acceptor_server(io_service, tcp::endpoint(address::from_string(this->ip), this->port));
        tcp::socket server_socket(io_service);
        acceptor_server.accept(server_socket);

        cout << "..." << endl;
        this->c_name = getData(server_socket);
        this->c_name.pop_back();

        string response, reply;
        
        reply = this->c_name + " joined!";
        cout << reply << endl;
        sendData(server_socket, this->name);

        while (true) {
            cout << "..." << endl;
            response = getData(server_socket);
            response.pop_back();

            if (response == "exit") {
                cout << this->c_name << " leave chat!" << endl;
                break;
            }
            cout << this->c_name << ": " << response << endl;

            cout << this->name << ": ";
            getline(cin, reply);
            sendData(server_socket, reply);

            if (reply == "exit")
                break;
        }
    }
    
    ~Server() {}
};

