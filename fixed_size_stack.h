#ifndef FIXED_SIZE_STACK_H
#define FIXED_SIZE_STACK_H

#include <vector>

template <class T> class FixedSizeStack {
public:
    FixedSizeStack(int size);
    ~FixedSizeStack();

    void clear();

    bool empty();
    bool full();

    void push(T* data);
    T* pop();

private:
    void stack_check();
    std::vector<T*> m_data;
    int m_fixed_size;
};

#endif //FIXED_SIZE_STACK_H
