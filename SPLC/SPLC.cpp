
#include <iostream>
#include <fstream>
#include <string>

#include "Server.cpp"
#include "Client.cpp"

using namespace std;

// Парсинг локального адреса
string getLocalIP() {    
    std::system("ipconfig > out.txt");
    ifstream file("out.txt");
    string line, text = "", ip;
    while (getline(file, line)) { 
        text += line; 
    }

    int index = text.find("192.168.0.");
    
    for (int i = index; i < index + 15; i++) {
        if (text[i] == ' ') { 
            break; 
        }
        ip += text[i];
    }
    return ip;
}

int checkPort(string scount) {
    int count = 0;
    while (true) {        
        if (scount.length() == 4) {
            for (char i : scount) {
                if (i < '0' || i > '9') {
                    cout << "Invalid port number" << endl;
                    count = 0;
                    return -1;
                }
                else {
                    switch (i) {
                    case '0': count += 0; break;
                    case '1': count += 1; break;
                    case '2': count += 2; break;
                    case '3': count += 3; break;
                    case '4': count += 4; break;
                    case '5': count += 5; break;
                    case '6': count += 6; break;
                    case '7': count += 7; break;
                    case '8': count += 8; break;
                    case '9': count += 9; break;
                    }
                    count *= 10;
                }
            }
            count /= 10;            
            if (count >= 2000 && count <= 8000) {
                return count;
            } else {
                cout << "Invalid port number" << endl;
                return -1;
            }
        } else {
            cout << "Incorrect input! Please try again!\n";
        }
    }
}

// Точка входа
int main() {
    string com, my_name, server_ip, my_local_ip, sport;
    int port, count = 0;

    my_local_ip = getLocalIP();
    cout << "===========================================\n";
    cout << "=     Smart Private Local Chat [SPLC]     =\n";
    cout << "===========================================\n";
    cout << " You local IP: " << my_local_ip << "\n";
    cout << "===========================================\n\n";
    cout << " Input you name: ";
    getline(cin, my_name);
    while (true) {
        cout << "\n";
        cout << "===========================================\n";
        cout << "= [S] Create server                       =\n";
        cout << "===========================================\n";
        cout << "= [C] Connect to server                   =\n";
        cout << "===========================================\n";
        cout << "Input command: ";
        getline(cin, com);
        if (com == "S" || com == "s") {
            while (true) {
                cout << "Input port number [2000 - 8000]: ";
                cin >> sport;
                port = checkPort(sport);
                if (port > 0) { break; }
                else { cout << "Incorrect input! Please try again!\n"; }
            }
            cout << "Wait for client...\n";
            Server server(my_name, my_local_ip, port); // Создание сервера, как объекта
            server.start(); // Запуск сервера
        }
        else if (com == "C" || com == "c") {
            while (true) {
                while (true) {
                    cout << "Input server local IP address: ";
                    getline(cin, server_ip);
                    for (char i : server_ip) { if (i == '.') { count += 1; } }
                    if (count == 3) { break; }
                    else { cout << "Incorrect input! Please try again!\n"; }
                }
                while (true) {
                    cout << "Input port number [2000 - 8000]: ";
                    cin >> sport;
                    port = checkPort(sport);
                    if (port > 0) break; 
                    else cout << "Incorrect input! Please try again!\n";
                }
                Client client(my_name, server_ip, port); // Создание клиента, как объекта
                int cl = client.start(); // Выполнение присоединения к серверу
                client.~Client();
                if (cl == 0)
                    break;
            }
        } else { cout << "Incorrect input! Please try again!\n"; }
    }
    return 0;
}


