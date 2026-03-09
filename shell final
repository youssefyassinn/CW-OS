#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

// This function runs either a built-in command or an external program
void executeCommand(vector<string> arguments, string inputFile, 
                   string outputFile, bool appendMode, bool background) {
    
    string command = arguments[0];
    
    // ----- cd command -----
    // If no argument, just print current directory
    if (command == "cd") {
        if (arguments.size() == 1) {
            char currentDir[1024];
            if (getcwd(currentDir, 1024)) 
                cout << currentDir << endl;
        } 
        // Otherwise try to change directory
        else if (chdir(arguments[1].c_str()) == 0) {
            char currentDir[1024];
            if (getcwd(currentDir, 1024)) 
                setenv("PWD", currentDir, 1);  // update PWD
        } 
        else {
            cerr << "cd: directory not found\n";
        }
        return;  // stop here (built-in finished)
    }
    
    // ----- dir command -----
    // Lists files in a directory (or current directory if none given)
    if (command == "dir") {
        string path = (arguments.size() > 1) ? arguments[1] : ".";
        DIR* directory = opendir(path.c_str());
        
        if (directory) {
            struct dirent* entry;
            while ((entry = readdir(directory))) {
                cout << entry->d_name << endl;
            }
            closedir(directory);
        }
        return;
    }
    
    // Prints all environment variables
    if (command == "environ") {
        extern char** environ;
        for (char** env = environ; *env; env++) {
            cout << *env << endl;
        }
        return;
    }
     
    // Sets or updates an environment variable
    if (command == "set") {
        if (arguments.size() >= 3) {
            string value = arguments[2];
            
            // In case value has spaces
            for (size_t i = 3; i < arguments.size(); i++) {
                value += " " + arguments[i];
            }
            
            setenv(arguments[1].c_str(), value.c_str(), 1);
        }
        return;
    }
    
    // Just prints whatever comes after echo
    if (command == "echo") {
        for (size_t i = 1; i < arguments.size(); i++) {
            if (i > 1) cout << " ";
            cout << arguments[i];
        }
        cout << endl;
        return;
    }
    
    // Shows basic information about this shell
    if (command == "help") {
        cout << "Commands: cd, dir, environ, set, echo, help, pause, quit\n"
             << "Features: < > >> &\n";
        return;
    }

    // Waits until user presses Enter
    if (command == "pause") {
        cout << "Press Enter..." << flush;
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    // If it's not built-in, we try to run it using fork + exec
    pid_t processId = fork();
    
    if (processId == 0) {
        // Child process
        
        // If there is input redirection (<)
        if (!inputFile.empty()) {
            int fileDescriptor = open(inputFile.c_str(), O_RDONLY);
            if (fileDescriptor != -1) {
                dup2(fileDescriptor, STDIN_FILENO);
                close(fileDescriptor);
            }
        }
        
        // If there is output redirection (> or >>)
        if (!outputFile.empty()) {
            int flags = O_WRONLY | O_CREAT | 
                        (appendMode ? O_APPEND : O_TRUNC);
            
            int fileDescriptor = open(outputFile.c_str(), flags, 0644);
            if (fileDescriptor != -1) {
                dup2(fileDescriptor, STDOUT_FILENO);
                close(fileDescriptor);
            }
        }
        
        // Prepare arguments for execvp
        char** execArgs = new char*[arguments.size() + 1];
        for (size_t i = 0; i < arguments.size(); i++) {
            execArgs[i] = (char*)arguments[i].c_str();
        }
        execArgs[arguments.size()] = NULL;
        
        // Replace child process with requested program
        execvp(execArgs[0], execArgs);
        
        // If exec fails, this runs
        cerr << "Error: command not found\n";
        exit(1);
        
    } 
    else if (processId > 0) {
        // Parent process
        
        // If background execution (&), don't wait
        if (background) {
            cout << "[" << processId << "]\n";
        } 
        else {
            waitpid(processId, NULL, 0);
        }
    }
}

int main(int argc, char* argv[]) {
    
    // Set initial PWD when shell starts
    char currentDir[1024];
    if (getcwd(currentDir, 1024)) {
        setenv("PWD", currentDir, 1);
    }
    
    // If a file name is provided, use batch mode
    ifstream batchFile;
    if (argc == 2) {
        batchFile.open(argv[1]);
        if (!batchFile) {
            cerr << "Cannot open file\n";
            return 1;
        }
    }
    
    // Choose where input comes from (keyboard or file)
    istream* inputStream;
    if (argc == 2) {
        inputStream = &batchFile;
    } else {
        inputStream = &cin;
    }
    
    string line;
    
    while (true) {
        
        // Show prompt only in interactive mode
        if (argc == 1) {
            if (getcwd(currentDir, 1024)) {
                cout << currentDir << "> " << flush;
            }
        }
        
        // Read a full line
        if (!getline(*inputStream, line)) break;
        
        // Ignore empty lines or comments
        if (line.empty() || line[0] == '#') continue;
        
        // Break line into words
        istringstream parser(line);
        vector<string> arguments;
        string word, inputFile, outputFile;
        bool appendMode = false, background = false;
        
        while (parser >> word) {
            if (word == "<") {
                parser >> inputFile;
            } 
            else if (word == ">") {
                parser >> outputFile;
                appendMode = false;
            } 
            else if (word == ">>") {
                parser >> outputFile;
                appendMode = true;
            } 
            else if (word == "&") {
                background = true;
            } 
            else {
                arguments.push_back(word);
            }
        }
        
        if (arguments.empty()) continue;
        if (arguments[0] == "quit") break;
        
        executeCommand(arguments, inputFile, outputFile, appendMode, background);
    }
    
    return 0;
}
