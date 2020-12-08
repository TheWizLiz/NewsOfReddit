#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
class tree {
public:
    class entry {
    public:
        int date, upvotes, downvotes;
        string title;
        entry(int dateIn, int upvotesIn, int downvotesIn, string titleIn) {
            date = dateIn;
            upvotes = upvotesIn;
            downvotes = downvotesIn;
            title = titleIn;
        }
    };
    class node {
    public:
        vector<entry> entries;
        node* right = nullptr;
        node* left = nullptr;
        int date;
        node(entry inital) {
            date = inital.date;
            entries.push_back(inital);
        }
        void add(entry input) {
            entries.push_back(input);
        }
    };
    node* root;
    tree() {

    }
    void insert(node* &current, entry data) {
            if (current == nullptr) {
                current = new node(data);
            }
            if (current->date == data.date)
                current->add(data);
            else if(current->date > data.date) {
                if (current->left != nullptr)
                    insert(current->left, data);
                else
                    current->left = new node(data);
            }
            else {
                if (current->right != nullptr)
                    insert(current->right, data);
                else
                    current->right = new node(data);
            }
        }
    vector<entry> find(node* current, int date) {
        if (current == nullptr)
            return vector<entry>();
        else if (current->date == date)
            return current->entries;
        else if (current->date > date) {
            return find(current->left, date);
        }
        else {
            return find(current->right, date);
        }
    }

    int FindLikedDay(int factor) {

        int tempLikes;
        int highestLikes = 0;
        int date;

        for (int i = 0; i < 3225; i++) {

            tempLikes = 0;

            vector<entry> posts = find(root, i);

            for (unsigned long j = 0; j < posts.size(); j++) {

                tempLikes += posts[j].upvotes;

            }

            if (tempLikes*factor > highestLikes*factor) {
                highestLikes = tempLikes;
                date = i;
            }
        }

        return date;

    }
};
