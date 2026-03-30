#include <iostream>
#include <filesystem>
#include <vector>

using namespace std;
using namespace std::filesystem;

int main() {
    string path;
    size_t binWidth;

    cout << "Enter directory path: ";
    cin >> path;

    cout << "Enter bin width (e.g., 1024): ";
    cin >> binWidth;

    vector<size_t> fileSizes;

    // Traverse directory
    for (recursive_directory_iterator it(path), end; it != end; ++it) {
        if (is_regular_file(*it)) {
            size_t size = file_size(*it);
            fileSizes.push_back(size);
        }
    }

    // Find max file size
    size_t maxSize = 0;
    for (size_t i = 0; i < fileSizes.size(); i++) {
        if (fileSizes[i] > maxSize) {
            maxSize = fileSizes[i];
        }
    }

    // Create histogram
    size_t numBins = (maxSize / binWidth) + 1;
    vector<int> histogram(numBins, 0);

    for (size_t i = 0; i < fileSizes.size(); i++) {
        size_t binIndex = fileSizes[i] / binWidth;
        histogram[binIndex]++;
    }

    // Print histogram
    cout << "\nFile Size Histogram:\n";

    for (size_t i = 0; i < histogram.size(); i++) {
        size_t lower = i * binWidth;
        size_t upper = lower + binWidth - 1;

        cout << "[" << lower << " - " << upper << "] : ";

        for (int j = 0; j < histogram[i]; j++) {
            cout << "*";
        }

        cout << " (" << histogram[i] << " files)\n";
    }

    return 0;
}
