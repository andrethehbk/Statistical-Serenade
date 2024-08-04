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
    int trivia_values[4];
    TreeNode* trivia_nodes[4];
    
    void getBFSLevel(queue<TreeNode*>& list);
    TreeNode* searchBFSLevel(queue<TreeNode*>& list, string val);
    void getAndPrintBFSLevel(queue<TreeNode*>& list, int &nodes);
    void dfsHelper(TreeNode* node, int &nodes);
    void deleteHelper(TreeNode* node);
    void printNode(TreeNode* node, string pre, string sub);
    void recalcMax(queue<TreeNode*>& level, int start);
    void recalcMaxHelper(queue<TreeNode*>& list, int index);
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
};

NaryTree::NaryTree(){
    // Creates root of N-ary Tree
    root = new TreeNode("user");
    // Initializes empty triva arrays
    for(int i = 0; i < 4; i++){
        trivia_values[i] = 0;
        trivia_nodes[i] = nullptr;
    }
}

NaryTree::~NaryTree(){
    deleteHelper(root);
}

void NaryTree::insert(vector<string> values){
    /*
        This method inserts value nodes into N-ary tree. Levels are structured as User->Artist->Album->Track,
        if a node does not exist at the appropriate level, it is added as a child to a parent node.
    */
    //Invalid Input
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
    if(trivia_values[3] < curr->data){
        trivia_values[3] = curr->data;
        trivia_nodes[3] = curr;
    }
    return;
}

void NaryTree::removeArtist(string artist){
    // Removes all instances of artist from tree
    transform(artist.begin(), artist.end(), artist.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, artist);
    if(to_remove == nullptr){
        cout << artist << " not in dataset." << endl;
        return;
    }
    level.front()->data--;
    level.front()->children.erase(artist);
    deleteHelper(to_remove);
    cout << "Removed " << artist << " from dataset." << endl;

    recalcMax(temp, 0);
    return;
}

void NaryTree::removeAlbum(string album){
    // Removes all instances of album from tree
    transform(album.begin(), album.end(), album.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    getBFSLevel(level);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, album);
    if(to_remove == nullptr){
        cout << album << " not in dataset." << endl;
        return;
    }

    while(to_remove != nullptr){
        level.front()->data--;
        level.front()->children.erase(album);
        deleteHelper(to_remove);
        cout << "Removed " << album << " by " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, album);
    }

    recalcMax(temp, 1);
    return;
}

void NaryTree::removeTrack(string track){
    // Removes all instances of track from tree
    transform(track.begin(), track.end(), track.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    getBFSLevel(level);
    getBFSLevel(level);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, track);
    if(to_remove == nullptr){
        cout << track << " not in dataset." << endl;
        return;
    }

    while(to_remove != nullptr){
        level.front()->data--;
        level.front()->children.erase(track);
        deleteHelper(to_remove);
        cout << "Removed " << track << " from " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, track);
    }

    recalcMax(temp, 2);
    return;
}

TreeNode* NaryTree::searchBFSLevel(queue<TreeNode*>& list, string val){
    // Searches queue for child with name_ID equal to val
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
    // Progresses Queue to Next Level in Depth
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

void NaryTree::getAndPrintBFSLevel(queue<TreeNode*>& list, int &nodes){
    // Prints Current Level's Children and Progresses Queue to Next Level in Depth
    int size = list.size();
    string pre = "";
    string sub = "";
    for(int i = 0; i < size; i++){
        TreeNode* curr = list.front();
        for(auto iter = curr->children.begin(); iter != curr->children.end(); iter++){
            list.push(iter->second);
            if(nodes <= 0){
                return;
            }
            printNode(iter->second, pre, sub);
            nodes--;
            pre = ", ";
        }
        list.pop();
    }
    return;
}

void NaryTree::printBFSTraversal(int nodes){
    queue<TreeNode*> level;
    level.push(root);
    cout << "Print Breath First Search:" << endl;
    printNode(root, "", ", ");
    nodes--;
    
    getAndPrintBFSLevel(level, nodes);
    if(nodes == 0){
        cout << endl;
        return;
    }
    getAndPrintBFSLevel(level, nodes);
    if(nodes == 0){
        cout << endl;
        return;
    }
    getAndPrintBFSLevel(level, nodes);

    cout << endl;
    return;
}

void NaryTree::printDFSTraversal(int nodes){
    cout << "Print Preorder Traversal:" << endl;
    dfsHelper(root, nodes);
    cout << endl;
    return;
}

void NaryTree::printTrivia(){
    cout << "User Trivia:" << endl;
    if(trivia_nodes[0] == nullptr){
        cout << "No data." << endl;
        return;
    }

    cout << "Artist with Most Albums: " << trivia_nodes[1]->name_ID << " (" <<trivia_values[1] << " Albums)" << endl;
    cout << "Album with Most Tracks: " << trivia_nodes[2]->name_ID << " (" <<trivia_values[2] << " Tracks)" << endl;
    cout << "Track with Most Plays: " << trivia_nodes[3]->name_ID << " (" <<trivia_values[3] << " Plays)" << endl;
    return;
}

void NaryTree::dfsHelper(TreeNode* node, int &nodes){
    if(nodes <= 0){
        return;
    }
    printNode(node, "", ", ");
    nodes--;
    if(node->children.size() == 0){
        return;
    }
    for(auto iter = node->children.begin(); iter != node->children.end(); iter++){
        dfsHelper(iter->second, nodes);
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

void NaryTree::printNode(TreeNode* node, string pre, string sub){
    cout << pre << node->name_ID << sub;
    return;
}
void NaryTree::recalcMax(queue<TreeNode*> &list, int start){
    for(int i = start; i < 4; i++){
        trivia_values[i] = 0;
        trivia_nodes[i] = nullptr;
        recalcMaxHelper(list, i);
    }
    return;
}
void NaryTree::recalcMaxHelper(queue<TreeNode*> &list, int index){
    // Progresses Queue to Next Level in Depth
    int size = list.size();
    for(int i = 0; i < size; i++){
        TreeNode* curr = list.front();
        if(trivia_values[index] < curr->data){
            trivia_values[index] = curr->data;
            trivia_nodes[index] = curr;
        }
        for(auto iter = curr->children.begin(); iter != curr->children.end(); iter++){
            list.push(iter->second);
        }
        list.pop();
    }
    return;
}
