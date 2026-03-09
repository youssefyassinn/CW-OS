#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc < 2)
        return 1;

    int maxF = 10;

    if (argc >= 3) {
        stringstream ss(argv[2]);
        ss >> maxF;
    }
    // open file
    ifstream file(argv[1]);
    if (!file)
        return 1;

    vector<int> pages;
    int x;

    while (file >> x)
        pages.push_back(x);

    file.close();

    vector<double> faultRate(maxF);
    // try different frame sizes
    for (int numFrames = 1; numFrames <= maxF; numFrames++) {

        vector<int> mem(numFrames, -1); // memory frames
        vector<int> counter(numFrames, 0); // aging counters

        int faults = 0;

        for (int i = 0; i < pages.size(); i++) {

            int curr = pages[i];
            bool found = false;

            // aging step
            for (int j = 0; j < numFrames; j++)
                counter[j] >>= 1;

            // check if page exists
            for (int j = 0; j < numFrames; j++) {
                if (mem[j] == curr) {
                    counter[j] |= 128;
                    found = true;
                    break;
                }
            }

            // page fault
            if (!found) {
                faults++;

                int replaceIndex = -1;

                // find empty frame
                for (int j = 0; j < numFrames; j++) {
                    if (mem[j] == -1) {
                        replaceIndex = j;
                        break;
                    }
                }

                // if no empty frame, replace smallest counter
                if (replaceIndex == -1) {
                    replaceIndex = 0;
                    for (int j = 1; j < numFrames; j++) {
                        if (counter[j] < counter[replaceIndex])
                            replaceIndex = j;
                    }
                }
                // replace page and reset its counter
                mem[replaceIndex] = curr;
                counter[replaceIndex] = 128;
            }
        }
        // calculate faults per 1000 references
        faultRate[numFrames - 1] = (double)faults / pages.size() * 1000.0;
        cout << numFrames << " " << faultRate[numFrames - 1] << endl;
    }

    return 0;
}
