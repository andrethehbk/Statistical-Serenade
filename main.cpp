#include "NaryTree.h"
#include <filesystem> // used for debugging

using namespace std;

//g++ -std=c++14 -o main.out main.cpp

int main() {
    NaryTree data;
    string filename = R"(..\andreSpotifyData.json)";
    data.loadFromFile(filename);
    data.mainloop();
    return 0;
}
