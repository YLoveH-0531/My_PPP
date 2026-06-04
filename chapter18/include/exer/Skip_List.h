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
    public:
        Skip_list() : head(new node{0, 0 ,nullptr}), hight(0){}
        Skip_list(const Skip_list& lhs);                // sholow copy
        Skip_list& operator=(const Skip_list& lhs);     // sholow copy
        ~Skip_list();
};

#endif //SKIP_LIST_H