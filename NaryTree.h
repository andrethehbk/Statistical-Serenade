#include <string>
#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iostream>
using namespace std;

struct TreeNode {
    string name_ID;
    int data;
    unordered_map<string, TreeNode*> children;
    TreeNode(string val){
        name_ID = val;
        data = 0;
    }
};

class NaryTree {
private:
    TreeNode* root;
    int trivia_values[3];
    TreeNode* trivia_nodes[3];
    
    void getBFSLevel(queue<TreeNode*>& list);
    TreeNode* searchBFSLevel(queue<TreeNode*>& list, string val);
    void getAndPrintBFSLevel(queue<TreeNode*>& list);
    void dfsHelper(TreeNode* node);
    void deleteHelper(TreeNode* node);
    void printNode(TreeNode* node);
public:
    NaryTree();
    ~NaryTree();
    void insert(vector<string> values);
    void removeArtist(string artist);
    void removeAlbum(string album);
    void removeTrack(string track);
    void printBFSTraversal();
    void printDFSTraversal();
    void printTrivia();
};

NaryTree::NaryTree(){
    root = new TreeNode("user");
    for(int i = 0; i < 3; i++){
        trivia_values[i] = 0;
        trivia_nodes[i] = nullptr;
    }
}

NaryTree::~NaryTree(){
    deleteHelper(root);
}

void NaryTree::insert(vector<string> values){
    if(values.size() != 3){
        cout << "Error in data" << endl;
        return;
    }
    TreeNode* curr = root;
    for(int i = 0; i < values.size(); i++){
        string val = values[i];
        transform(val.begin(), val.end(), val.begin(), ::toupper);
        if(curr->children.find(val) == curr->children.end()){
            curr->children[val] = new TreeNode(val);
            curr->data++;
            if(trivia_values[i] < curr->data){
                trivia_values[i] = curr->data;
                trivia_nodes[i] = curr;
            }
            
        }
        curr = curr->children[val];
    }
    curr->data++;
    return;
}

void NaryTree::removeArtist(string artist){
    transform(artist.begin(), artist.end(), artist.begin(), ::toupper);
    queue<TreeNode*> level;
    level.push(root);
    TreeNode* to_remove = searchBFSLevel(level, artist);
    if(to_remove == nullptr){
        cout << artist << " not in dataset." << endl;
        return;
    }
    level.front()->data--;
    level.front()->children.erase(artist);
    deleteHelper(to_remove);
    cout << "Removed " << artist << " from dataset." << endl;
    return;
}

void NaryTree::removeAlbum(string album){
    transform(album.begin(), album.end(), album.begin(), ::toupper);
    queue<TreeNode*> level;
    level.push(root);
    getBFSLevel(level);
    TreeNode* to_remove = searchBFSLevel(level, album);
    if(to_remove == nullptr){
        cout << album << " not in dataset." << endl;
        return;
    }
    while(to_remove != nullptr){
        level.front()->data--;
        level.front()->children.erase(album);
        deleteHelper(to_remove);
        cout << "Removed " << album << "by " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, album);
    }
    return;
}

void NaryTree::removeTrack(string track){
    transform(track.begin(), track.end(), track.begin(), ::toupper);
    queue<TreeNode*> level;
    level.push(root);
    getBFSLevel(level);
    getBFSLevel(level);
    TreeNode* to_remove = searchBFSLevel(level, track);
    if(to_remove == nullptr){
        cout << track << " not in dataset." << endl;
        return;
    }
    while(to_remove != nullptr){
        level.front()->data--;
        level.front()->children.erase(track);
        deleteHelper(to_remove);
        cout << "Removed " << track << "from " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, track);
    }
    return;
}

TreeNode* NaryTree::searchBFSLevel(queue<TreeNode*>& list, string val){
    int size = list.size();
    for(int i = 0; i < size; i++){
        TreeNode* curr = list.front();
        for(auto iter = curr->children.begin(); iter != curr->children.end(); iter++){
            if(iter->second->name_ID == val){
                return iter->second;
            }
        }
        list.pop();
    }
    return nullptr;
}

void NaryTree::getBFSLevel(queue<TreeNode*>& list){
    int size = list.size();
    for(int i = 0; i < size; i++){
        TreeNode* curr = list.front();
        for(auto iter = curr->children.begin(); iter != curr->children.end(); iter++){
            list.push(iter->second);
        }
        list.pop();
    }
    return;
}

void NaryTree::getAndPrintBFSLevel(queue<TreeNode*>& list){
    int size = list.size();
    for(int i = 0; i < size; i++){
        TreeNode* curr = list.front();
        for(auto iter = curr->children.begin(); iter != curr->children.end(); iter++){
            list.push(iter->second);
            printNode(iter->second);
        }
        list.pop();
    }
    return;
}

void NaryTree::printBFSTraversal(){
    queue<TreeNode*> level;
    level.push(root);

    cout << "Print Artists:" << endl;
    getAndPrintBFSLevel(level);
    cout << endl;
    cout << level.size() << " Artists" << endl;

    cout << "Print Albums:" << endl;
    getAndPrintBFSLevel(level);
    cout << endl;
    cout << level.size() << " Albums" << endl;

    cout << "Print Tracks:" << endl;
    getAndPrintBFSLevel(level);
    cout << endl;
    cout << level.size() << " Tracks" << endl;
    return;
}

void NaryTree::printDFSTraversal(){
    cout << "Print Preorder Traversal:" << endl;
    dfsHelper(root);
    cout << endl;
    return;
}

void NaryTree::printTrivia(){
    cout << "User Trivia:" << endl;
    if(trivia_nodes[0] == nullptr){
        cout << "No data." << endl;
        return;
    }
    cout << "Artist with Most Albums: " << trivia_nodes[0]->name_ID << " (" <<trivia_values[0] << " Albums)" << endl;
    cout << "Album with Most Tracks: " << trivia_nodes[1]->name_ID << " (" <<trivia_values[1] << " Tracks)" << endl;
    cout << "Track with Most Plays: " << trivia_nodes[2]->name_ID << " (" <<trivia_values[2] << " Plays)" << endl;
    return;
}

void NaryTree::dfsHelper(TreeNode* node){
    printNode(node);
    if(node->children.size() == 0){
        return;
    }
    for(auto iter = node->children.begin(); iter != node->children.end(); iter++){
        dfsHelper(iter->second);
    }
    return;
}

void NaryTree::deleteHelper(TreeNode* node){
    if(node->children.size() == 0){
        delete node;
        return;
    }
    for(auto iter = node->children.begin(); iter != node->children.end(); iter++){
        deleteHelper(iter->second);
    }
    delete node;
    return;
}

void NaryTree::printNode(TreeNode* node){
    cout << node->name_ID << ", ";
    return;
}
