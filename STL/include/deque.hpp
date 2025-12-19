#pragma once
#include<iostream>
#include <stdexcept>


template <class T>
class deque
{
private:

    size_t m_size = 0;
    size_t m_capacity=0;
    T* m_data = nullptr; 

public:

    //构造
    deque () = default;
    deque (const size_t n, const T& val);
    deque (const deque<T> other);

    //析构
    ~deque ();
    
    //赋值
    deque<T> operator =(const deque & other);
    deque<T> assign (T* begin_ptr, T* end_ptr);

    //尾插和尾删
    void push_back( const T&val);
    void pop_back();

    //头插和头删
    void push_front( const T&val);
    void pop_front();

    //大小
    bool empty() const { return m_size == 0;};
    size_t size () const {return m_size;};
    void resize (size_t size );
    void resize (size_t num; size_t elem);

    //插入和删除
    void insert( T* insert_begin_ptr , const T& val);
    T* erase(T* erase_ptr );
    T* erase(T* erase_begin_ptr, T* erase_end_ptr);
    void clear();

    //数据存取
    const T& operator[](size_t index)const;
    const T& at(size_t index) const;
    T& operator [](size_t index);
    T& at(size_t index);

    const T & front() const;
    const T & back() const;
    T& front();
    T& back();

    //迭代器
    T* begin() {return m_data; }
    const T* begin() const {return m_data;}
    T* end() { return m_data+m_size; }
    const T* end() const { return m_data + m_size; }


};