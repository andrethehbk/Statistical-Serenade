#ifndef NARYTREE_H
#define NARYTREE_H

#include <iostream>
#include <string>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <utility>
#include <regex>
#include <fstream>
#include <vector>
#include <sstream>
#include <set>
#include <unordered_set>
#include <locale>
#include <codecvt>

using namespace std;

struct TreeNode {
    string name_ID;
    int data;
    unordered_map<string, TreeNode*> children;
    explicit TreeNode(string val) {
        name_ID = std::move(val);
        data = 0;
    }
};

class NaryTree {
private:
    TreeNode* root;
    int trivia_values[4]{};
    TreeNode* trivia_nodes[4]{};
    map<string, set<string>> artistAlbums; // to store artists and their albums
    std::map<std::string, int> artistCount; // to store artist play count

    static void getBFSLevel(queue<TreeNode*>& list);
    static TreeNode* searchBFSLevel(queue<TreeNode*>& list, const string& val);
    static void getAndPrintBFSLevel(queue<TreeNode*>& list, int &nodes);
    void dfsHelper(TreeNode* node, int &nodes);
    void deleteHelper(TreeNode* node);
    static void printNode(TreeNode* node, const string& pre, const string& sub);
    void recalcMax(queue<TreeNode*>& level, int start);
    void recalcMaxHelper(queue<TreeNode*>& list, int index);
    static bool verifyInt(const string &number, int &value);
    static void printOptions();

public:
    NaryTree();
    ~NaryTree();
    void insert(vector<string> values);
    void removeArtist(string artist);
    void removeAlbum(string album);
    void removeTrack(string track);
    void printBFSTraversal(int nodes);
    void printDFSTraversal(int nodes);
    void printTrivia();
    void loadFromFile(const string& filename);
    void mainloop();
    void displayTop25ArtistsAndAlbums();
    void calcTrackPlaycounts(TreeNode* node, std::map<std::string, int>& trackCount);

    void processBatch(const vector<std::wstring> &lines);
};

#endif // NARYTREE_H
