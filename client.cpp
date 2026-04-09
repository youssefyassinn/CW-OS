#include <iostream>
#include <thread>
#include <cstring>

#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void receive(int server) {
    char msg[1024];

    while (true) {
        memset(msg, 0, sizeof(msg));
        int n = recv(server, msg, sizeof(msg), 0);

        if (n <= 0) {
            cout << "\nServer closed.\n";
            break;
        }

        cout << "\nServer: " << msg << endl;
        cout << "You: ";
        cout.flush();
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    connect(sock, (sockaddr*)&addr, sizeof(addr));

    cout << "Connected!\n";

    thread t(receive, sock);

    string msg;
    while (true) {
        cout << "You: ";
        getline(cin, msg);

        if (msg == "exit") break;

        send(sock, msg.c_str(), msg.size(), 0);
    }

    close(sock);

    return 0;
}