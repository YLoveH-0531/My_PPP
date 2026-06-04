#ifndef SKIP_LIST_H
#define SKIP_LIST_H

struct node{
    int data;
    int hight;
    node** succ;
};

class Skip_list
{
    private:
        node* head;
        int hight;
        void destory();                                 // free all the node.
        node* find(node* p);                            // find the node.
    public:
        Skip_list() : head(new node{0, 0 ,nullptr}), hight(0){}
        Skip_list(const Skip_list& lhs);                // Shallow copy
        Skip_list& operator=(const Skip_list& lhs);     // Shallow copy
        Skip_list(Skip_list&& lhs) noexcept;
        Skip_list& operator=(Skip_list&& lhs) noexcept;
        ~Skip_list();
};

#endif //SKIP_LIST_H