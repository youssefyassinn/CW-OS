#include <iostream>
#include <thread>
#include <cstring>

#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void receive(int client) {
    char msg[1024];

    while (true) {
        memset(msg, 0, sizeof(msg));
        int n = recv(client, msg, sizeof(msg), 0);

        if (n <= 0) {
            cout << "\nClient left.\n";
            break;
        }

        cout << "\nClient: " << msg << endl;
        cout << "You: ";
        cout.flush();
    }
}

int main() {
    int server = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server, (sockaddr*)&addr, sizeof(addr));
    listen(server, 1);

    cout << "Waiting for client...\n";

    int client = accept(server, NULL, NULL);

    cout << "Client connected!\n";

    thread t(receive, client);

    string msg;
    while (true) {
        cout << "You: ";
        getline(cin, msg);

        if (msg == "exit") break;

        send(client, msg.c_str(), msg.size(), 0);
    }

    close(client);
    close(server);

    return 0;
}