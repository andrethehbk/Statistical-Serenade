#include "NaryTree.h"
#include <filesystem> // used for debugging

using namespace std;

//g++ -std=c++14 -o main.out main.cpp

int main() {
    setlocale(LC_ALL, "");
    NaryTree data;
    string filename = R"(C:\Users\awcam\CLionProjects\StatisticalSerenade\andreSpotifyData.json)";
    data.loadFromFile(filename);
    data.mainloop();
    return 0;
}
