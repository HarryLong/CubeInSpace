#include <fixed_size_stack.h>
#include "gl_core/glheader.h"

template <class T> FixedSizeStack<T>::FixedSizeStack(int size) : m_fixed_size(size)
{

}

template <class T> FixedSizeStack<T>::~FixedSizeStack()
{
    clear();
}

template <class T> void FixedSizeStack<T>::clear()
{
    for(T * elem : m_data)
        delete elem;
    m_data.clear();
}

template <class T> bool FixedSizeStack<T>::empty()
{
    return m_data.empty();
}

template <class T> bool FixedSizeStack<T>::full()
{
    return m_data.size() == m_fixed_size;
}

template <class T> void FixedSizeStack<T>::push(T* data)
{
    m_data.push_back(data);
    stack_check();
}

template <class T> T* FixedSizeStack<T>::pop()
{
    if(!empty())
    {
        T * elem = m_data.back();
        m_data.pop_back();
        return elem;
    }

    return nullptr;
}

template <class T> void FixedSizeStack<T>::stack_check()
{
    while(m_data.size() > m_fixed_size)
    {
        T * elem = m_data.front();
        m_data.erase(m_data.begin());
        delete elem;
    }
}

template class FixedSizeStack<GLubyte>;
template class FixedSizeStack<GLbyte>;
template class FixedSizeStack<GLfloat>;
template class FixedSizeStack<GLuint>;
template class FixedSizeStack<GLint>;
