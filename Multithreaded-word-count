#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <map>
#include <mutex>

using namespace std;

map<string, int> counter;
mutex m;

void worker(vector<string> words, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        m.lock();
        counter[words[i]]++;
        m.unlock();
    }
}

int main()
{
    ifstream file("input.txt");
    vector<string> words;
    string word;

    while (file >> word)
        words.push_back(word);

    int n = 3;
    vector<thread> t;

    int size = words.size() / n;

    for (int i = 0; i < n; i++)
    {
        int start = i * size;
        int end;

        if (i == n - 1)
            end = words.size();
        else
            end = start + size;

        t.push_back(thread(worker, words, start, end));
    }

    for (int i = 0; i < n; i++)
        t[i].join();

    cout << "Final Count:\n";

    for (pair<string, int> p : counter)
        cout << p.first << " : " << p.second << endl;

    return 0;
}
