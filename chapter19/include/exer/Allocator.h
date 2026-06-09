#ifndef ALLOCATOR_H
#define ALLOCATOR_H

template <typename T>
class Allocator
{
    public:
        Allocator();
        ~Allocator();
        void construct(T* p, const T& val);
        void destory(T* p);
    private:
        void* p;

};

#endif // ALLOCATOR_H