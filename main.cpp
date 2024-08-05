#include "NaryTree.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <regex>
using namespace std;

//g++ -std=c++14 -o main.out main.cpp

void printOptions(){
    cout << "===============================" << endl;
    cout << "[1] Compare BFS & DFS Traversal" << endl;
    cout << "[2] Display Trivia" << endl;
    cout << "[3] Remove Artist" << endl;
    cout << "[4] Remove Album" << endl;
    cout << "[5] Remove Track" << endl;
    cout << "[6] Exit Program" << endl;
    cout << "===============================" << endl;
    cout << "Please Enter a Number: ";
}

bool verifyInt(const string& number, int &value){
    //Verify String is valid Input
    regex verify = regex("[0-9]*");
    if(regex_match(number, verify)){
        value = stoi(number);
    }
    return regex_match(number, verify);
}

int main(){
    // Create N-ary Tree Using Database in txt file
    NaryTree data;
    ifstream data_file("andreSpotifyData.txt");
    if(!data_file.is_open()){
        cout << "Error." << endl;
        return 0;
    }
    string current_line;
    string next;
    string artist;
    string album;
    string track;

    getline(data_file, current_line);
    while(getline(data_file, current_line)){
        istringstream inStream(current_line);
        vector<string> values;
        
        getline(inStream, next, ':');
        getline(inStream, next, '"');
        getline(inStream, artist, '"');
        values.push_back(artist);

        getline(inStream, next, ':');
        getline(inStream, next, '"');
        getline(inStream, album, '"');
        values.push_back(album);

        getline(inStream, next, ':');
        getline(inStream, next, '"');
        getline(inStream, track, '"');
        values.push_back(track);

        data.insert(values);
    }

    data_file.close();
    int input = 0;
    while(input != 6){
        string temp;
        printOptions();
        getline(cin, temp);
        if(!verifyInt(temp, input)){
            cout << "Invalid Input." << endl;
        }
        else if(input == 1){
            cout << "Please Enter Number of Nodes to Visit: ";
            int number = 0;
            getline(cin, temp);
            
            if(verifyInt(temp, number) && number > 0){
                data.printBFSTraversal(number);
                data.printDFSTraversal(number);
            }
            else{
                cout << "Invalid Number." << endl;
            }
        }
        else if(input == 2){
            data.printTrivia();
        }
        else if(input == 3){
            cout << "Enter Artist (not case sensitive):" << endl;
            getline(cin, temp);
            data.removeArtist(temp);
        }
        else if(input == 4){
            cout << "Enter Album (not case sensitive):" << endl;
            getline(cin, temp);
            data.removeAlbum(temp);
        }
        else if(input == 5){
            cout << "Enter Track (not case sensitive):" << endl;
            getline(cin, temp);
            data.removeTrack(temp);
        }
    }
    return 0;
}
