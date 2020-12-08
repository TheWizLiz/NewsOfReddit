#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
#include <array>
using namespace std;

class Post {
private:
    int upvote;
    int downvote;
    string title;

public:

    Post() {
        upvote = 0;
        downvote = 0;
        title = "";
    }
    Post(int upvote, int downvote, string title) {
        this->upvote = upvote;
        this->downvote = downvote;
        this->title = title;
    }

    string getTitle() {
        return this->title;
    }

    int getUpvotes() {
        return this->upvote;
    }
};

class HashNode {
public:
    int date;
    vector<Post*> posts;

    HashNode(int date, int uv, int dv, string title) {
        this->date = date;
        Post* post = new Post(uv, dv, title);
        posts.push_back(post);
    }

    void addPost(int uv, int dv, string title) {
        Post* post = new Post(uv, dv, title);
        posts.push_back(post);
        /*
        sort(posts.begin(), posts.end(),
            [](Post* post1, Post* post2) {
                int i = 0;
                while ((post1->getTitle())[i] == (post2->getTitle())[i]) {
                    i++;
                }
                return (post1->getTitle())[i] < (post2->getTitle())[i];
            });
            */
    }

};

class hashTable {
private:
    array<HashNode*, 3225> nodes;
public:
    hashTable() {
        for (unsigned long i = 0; i < nodes.size(); i++) {
            nodes[i] = nullptr;
        }
    }

    int hashFunction(int date) {
        int hashValue = date % 3225;
        return hashValue;
    }

    void InsertNode(int date, int uv, int dv, string title) {
        int hashValue = hashFunction(date);
        if (nodes[hashValue] != nullptr) {
            nodes[hashValue]->addPost(uv, dv, title);
        }
        else {
            HashNode* node = new HashNode(date, uv, dv, title);
            nodes[hashValue] = node;
        }
    }

    vector<Post*> Search(int date) {
            int hashValue = hashFunction(date);
            vector<Post*> empty;
            if (nodes[hashValue] == nullptr) {
                return empty;
            }
            else {
                return nodes[hashValue]->posts;
            }
        }

    int FindLikedDay(int factor) {

        int tempLikes;
        int highestLikes = 0;
        int date = 0;

        for (int i = 0; i < 3225; i++) {

            if (nodes[i] == nullptr) {
                //cout << i << endl;
                continue;
            }

            tempLikes = 0;

            for (unsigned long j = 0; j < nodes[i]->posts.size(); j++) {

                tempLikes += nodes[i]->posts[j]->getUpvotes();

            }

            if (tempLikes*factor > highestLikes*factor) {
                highestLikes = tempLikes;
                date = i;
            }
        }

        return date;

    }

};
