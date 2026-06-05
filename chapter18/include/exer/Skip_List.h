#ifndef SKIP_LIST_H
#define SKIP_LIST_H
#define MAX_LEVEL 5
#include <random>
#include <utility>
struct node{
    int data;
    int height;
    node** succ;
};

class Skip_list
{
    private:
        std::mt19937 rng;                              // random number generator
        node* head;
        int height;
        void destory();                                // free all the node.
        std::pair<bool, node*> find(int value, node** update);          // find the node.
        int createLevel();                             // create level
    public:
        Skip_list();
        Skip_list(const Skip_list& lhs) = delete;                // forbidden 
        Skip_list& operator=(const Skip_list& lhs) = delete;     // forbidden
        Skip_list(Skip_list&& lhs) noexcept;
        Skip_list& operator=(Skip_list&& lhs) noexcept;
        ~Skip_list();
        node* insert(int value);
        node* at(int value);
        node* erase(int value);
        void print();
};

#endif //SKIP_LIST_H