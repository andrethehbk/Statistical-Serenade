#include "NaryTree.h"
#include "nlohmann/json.hpp"

using json = nlohmann::json;

NaryTree::NaryTree() {
    // creates root of N-ary Tree
    root = new TreeNode("andrethehbk");
    // initializes empty trivia arrays
    for (int i = 0; i < 4; i++) {
        trivia_values[i] = 0;
        trivia_nodes[i] = nullptr;
    }
}

NaryTree::~NaryTree() {
    deleteHelper(root);
}

void NaryTree::loadFromFile(const string& filename) {
    ifstream file(filename);
    // check the file is open and read in all data
    if (!file.is_open()) {
        cout << "Error opening file." << endl;
        exit(1); // if the file fails to open, exit with error code
    }

    string current_line;

    // ignore the first line
    if (!getline(file,current_line)) {
        cout << "Error reading file." << endl;
        exit(1);
    }

    unsigned lineCount = 0; // for debugging purposes

    // read all lines of json file
    while (getline(file, current_line)) {
        try {
            json j = json::parse(current_line);

            string artist = j["artistName"].get<string>();
            string album = j["albumName"].get<string>();
            string track = j["trackName"].get<string>();

            vector<string> values = { artist, album, track };
            NaryTree::insert(values);
            lineCount++;
            // update artistAlbums and artistCount, separate from the n-ary tree
            artistAlbums[artist].insert(album);
            artistCount[artist]++;
        } catch (json::parse_error& e) {
            cout << "JSON parse error: " << e.what() << " at line: " << current_line << endl;
        } catch (exception& e) {
            cout << "Error processing line: " << e.what() << " at line: " << current_line << endl;
        }
    }

    file.close();
    // debugging statement
    cout << "Total lines processed: " << lineCount << endl;
}

void NaryTree::printOptions() {
    cout << "===============================" << endl;
    cout << "[1] Compare BFS & DFS Traversal" << endl;
    cout << "[2] Display Trivia" << endl;
    cout << "[3] Remove Artist" << endl;
    cout << "[4] Remove Album" << endl;
    cout << "[5] Remove Track" << endl;
    cout << "[6] Display Top 25 Most Listened Artists and Their Albums" << endl;
    cout << "[7] Exit Program" << endl;
    cout << "===============================" << endl;
    cout << "Please Enter a Number: ";
}

bool NaryTree::verifyInt(const string& number, int& value) {
    // verify string is valid input
    regex verify = regex("[0-9]*");
    if (regex_match(number, verify)) {
        value = stoi(number);
    }
    return regex_match(number, verify);
}

void NaryTree::insert(vector<string> values) {
    /*
        This method inserts value nodes into N-ary tree. Levels are structured as User->Artist->Album->Track->DatesListened,
        if a node does not exist at the appropriate level, it is added as a child to a parent node.
    */
    // invalid input error handling
    if (values.size() != 3) {
        cout << "Error in data" << endl;
        return;
    }
    TreeNode* curr = root;
    for (int i = 0; i < values.size(); i++) {
        string val = values[i];
        transform(val.begin(), val.end(), val.begin(), ::toupper);
        if (curr->children.find(val) == curr->children.end()) {
            curr->children[val] = new TreeNode(val);
            curr->data++;
            if (trivia_values[i] < curr->data) {
                trivia_values[i] = curr->data;
                trivia_nodes[i] = curr;
            }
        }
        curr = curr->children[val];
    }
    curr->data++;
    if (trivia_values[3] < curr->data) {
        trivia_values[3] = curr->data;
        trivia_nodes[3] = curr;
    }
}

void NaryTree::removeArtist(string artist) {
    // removes all instances of artist from tree
    transform(artist.begin(), artist.end(), artist.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, artist);
    if (to_remove == nullptr) {
        cout << artist << " not in dataset." << endl;
        return;
    }
    level.front()->data--;
    level.front()->children.erase(artist);
    deleteHelper(to_remove);
    cout << "Removed " << artist << " from dataset." << endl;

    recalcMax(temp, 0);
}

void NaryTree::removeAlbum(string album) {
    // removes all instances of album from tree
    transform(album.begin(), album.end(), album.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    getBFSLevel(level);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, album);
    if (to_remove == nullptr) {
        cout << album << " not in dataset." << endl;
        return;
    }

    while (to_remove != nullptr) {
        level.front()->data--;
        level.front()->children.erase(album);
        deleteHelper(to_remove);
        cout << "Removed " << album << " by " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, album);
    }

    recalcMax(temp, 1);
}

void NaryTree::removeTrack(string track) {
    // removes all instances of track from tree
    transform(track.begin(), track.end(), track.begin(), ::toupper);
    queue<TreeNode*> level;
    queue<TreeNode*> temp;
    level.push(root);
    getBFSLevel(level);
    getBFSLevel(level);
    temp = level;

    TreeNode* to_remove = searchBFSLevel(level, track);
    if (to_remove == nullptr) {
        cout << track << " not in dataset." << endl;
        return;
    }

    while (to_remove != nullptr) {
        level.front()->data--;
        level.front()->children.erase(track);
        deleteHelper(to_remove);
        cout << "Removed " << track << " from " << level.front()->name_ID << endl;
        level.pop();
        to_remove = searchBFSLevel(level, track);
    }

    recalcMax(temp, 2);
}

TreeNode* NaryTree::searchBFSLevel(queue<TreeNode*>& list, const string& val) {
    // searches queue for child with name_ID equal to val
    int size = list.size();
    for (int i = 0; i < size; i++) {
        TreeNode* curr = list.front();
        for (auto&  iter : curr->children) {
            if (iter.second->name_ID == val) {
                return iter.second;
            }
        }
        list.pop();
    }
    return nullptr;
}

void NaryTree::getBFSLevel(queue<TreeNode*>& list) {
    // progresses queue to next level in depth
    int size = list.size();
    for (int i = 0; i < size; i++) {
        TreeNode* curr = list.front();
        for (auto&  iter : curr->children) {
            list.push(iter.second);
        }
        list.pop();
    }
}

void NaryTree::getAndPrintBFSLevel(queue<TreeNode*>& list, int& nodes) {
    // prints current level's children and progresses queue to next level in depth
    int size = list.size();
    string pre;
    string sub;
    for (int i = 0; i < size; i++) {
        TreeNode* curr = list.front();
        for (auto&  iter : curr->children) {
            list.push(iter.second);
            if (nodes <= 0) {
                return;
            }
            printNode(iter.second, pre, sub);
            nodes--;
            pre = ", ";
        }
        list.pop();
    }
}

void NaryTree::printBFSTraversal(int nodes) {
    queue<TreeNode*> level;
    level.push(root);
    cout << "Print Breath First Search:" << endl;
    printNode(root, "", ", ");
    nodes--;

    getAndPrintBFSLevel(level, nodes);
    if (nodes == 0) {
        cout << endl;
        return;
    }
    getAndPrintBFSLevel(level, nodes);
    if (nodes == 0) {
        cout << endl;
        return;
    }
    getAndPrintBFSLevel(level, nodes);

    cout << endl;
}

void NaryTree::printDFSTraversal(int nodes) {
    cout << "Print Preorder Traversal:" << endl;
    dfsHelper(root, nodes);
    cout << endl;
}

void NaryTree::compareAlgorithms(string& temp) {
    cout << "Please Enter Number of Nodes to Visit: ";
    int number = 0;
    getline(cin, temp);

    if (NaryTree::verifyInt(temp, number) && number > 0) {
        if (number > 500) {
            cout << "Are you sure you want to print this many nodes? (Y/N)" << endl;
            getline(cin, temp);
            if (temp == "Y" || temp == "y") {
                NaryTree::printBFSTraversal(number);
                NaryTree::printDFSTraversal(number);
            } else if (temp == "N" || temp == "n") {
                compareAlgorithms(temp);
            }
        } else {
            NaryTree::printBFSTraversal(number);
            NaryTree::printDFSTraversal(number);
        }
    }
    else {
        cout << "Invalid Number." << endl;
    }
}

void NaryTree::dfsHelper(TreeNode* node, int& nodes) {
    if (nodes <= 0) {
        return;
    }
    printNode(node, "", ", ");
    nodes--;
    if (node->children.empty()) {
        return;
    }
    for (auto&  iter : node->children) {
        dfsHelper(iter.second, nodes);
    }
}

void NaryTree::deleteHelper(TreeNode* node) {
    if (node->children.empty()) {
        delete node;
        return;
    }
    for (auto& iter: node->children) {
        deleteHelper(iter.second);
    }
    delete node;
}

void NaryTree::printNode(TreeNode* node, const string& pre, const string& sub) {
    cout << pre << node->name_ID <<  sub;
}

void NaryTree::recalcMax(queue<TreeNode *>& level, int start) {
    for (int i = start; i < 4; i++) {
        trivia_values[i] = 0;
        trivia_nodes[i] = nullptr;
        recalcMaxHelper(level, i);
    }
}

void NaryTree::recalcMaxHelper(queue<TreeNode *>& list, int index) {
    // progresses queue to next level in depth
    int size = list.size();
    for (int i = 0; i < size; i++) {
        TreeNode* curr = list.front();
        if (trivia_values[index] < curr->data) {
            trivia_values[index] = curr->data;
            trivia_nodes[index] = curr;
        }
        for (auto&  iter : curr->children) {
            list.push(iter.second);
        }
        list.pop();
    }
}

void NaryTree::printTrivia() {
    cout << "User Trivia:" << endl;
    if (trivia_nodes[0] == nullptr) {
        cout << "No data." << endl;
        return;
    }

    cout << "Artist with Most Albums: " << trivia_nodes[1]->name_ID << " (" << trivia_values[1] << " Albums)" << endl;
    cout << "Album with Most Tracks: " << trivia_nodes[2]->name_ID << " (" << trivia_values[2] << " Tracks)" << endl;
    cout << "Track with Most Plays: " << trivia_nodes[3]->name_ID << " (" << trivia_values[3] << " Plays)" << endl;
}

void NaryTree::displayTop25ArtistsAndAlbums() {
    // create a vector of pairs and sort it based on play count
    vector<pair<string, int>> sortedArtists(artistCount.begin(), artistCount.end());
    sort(sortedArtists.begin(), sortedArtists.end(), [](const auto &a, const auto &b) {
        return b.second < a.second; // sort in ascending order
    });

    if (sortedArtists.empty()) {
        cout << "No artists found in the dataset." << endl;
        return;
    }
    // display the T25 artists + albums
    cout << "Top 25 Most Listened Artists and Their Albums:" << endl;
    for (int i = 0; i < 25 && i < sortedArtists.size(); i++) {
        const string &artist = sortedArtists[i].first;
        cout << i + 1 << ". " << artist << " (" << sortedArtists[i].second << " plays):" << endl;

        if (artistAlbums.find(artist) != artistAlbums.end()) {
            // convert the set of albums to a vector for sorting
            vector<string> albums(artistAlbums[artist].begin(), artistAlbums[artist].end());
            // sort the albums alphabetically
            sort(albums.begin(), albums.end());
            // print the sorted albums
            for (const string &album: albums) {
                cout << "    " << album << endl;
            }
        }
    }
}

void NaryTree::mainloop() {
    int input = 0;
    while (input != 7) {
        string temp;
        NaryTree::printOptions();
        getline(cin, temp);
        if (!NaryTree::verifyInt(temp, input)) {
            cout << "Invalid Input." << endl;
        }
        else if (input == 1) {
            NaryTree::compareAlgorithms(temp);
        }
        else if (input == 2) {
            NaryTree::printTrivia();
        }
        else if (input == 3) {
            cout << "Enter Artist (not case sensitive):" << endl;
            getline(cin, temp);
            NaryTree::removeArtist(temp);
        }
        else if (input == 4) {
            cout << "Enter Album (not case sensitive):" << endl;
            getline(cin, temp);
            NaryTree::removeAlbum(temp);
        }
        else if (input == 5) {
            cout << "Enter Track (not case sensitive):" << endl;
            getline(cin, temp);
            NaryTree::removeTrack(temp);
        }
        else if (input == 6) {
            displayTop25ArtistsAndAlbums();
        }
    }
}
