#pragma once
#include<iostream>
#include <stdexcept>

template <class T>
class vector
{
private:

    T * m_data = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;

public:

    //构造
    vector ()=default;
    explicit vector (size_t n);  //n elements default-initialized
    explicit vector (size_t n, const T& val);  //n elements initialized to val
    

    //拷贝构造
    vector (const vector &other);

    //析构函数
    ~vector();
    //赋值
    vector<T>& operator=(const vector & other);
    
    //尾插和尾删
    void push_back(const T&val);
    void pop_back();

    //容量和大小
    bool empty() const { return m_size==0;};
    size_t capacity() const { return m_capacity; };
    size_t size() const { return m_size; };
    void resize(size_t size);

    //插入和删除
    void insert(T * insert_begin_ptr , const T&val );
    T* erase(T * erase_ptr );
    T* erase(T* erase_begin_ptr, T* erase_end_ptr);
    void clear();

    //数据存取
    const T & operator[](size_t index)const;
    const T & at(size_t index) const;
    T & operator [](size_t index);
    T& at(size_t index);

    const T & front() const;
    const T & back() const;
    T& front();
    T& back();

    //互换容器
    void swap(vector & v);

    //预留空间
    void reserve(size_t size);

    // 迭代器
    T* begin() { return m_data; }
    const T* begin() const { return m_data; }
    T* end() { return m_data + m_size; }
    const T* end() const { return m_data + m_size; }

};


//构造 - n elements default-initialized
template <typename T>
vector<T>::vector (size_t n):
 m_data(static_cast<T*>(::operator new(n * sizeof(T)))), 
 m_size(n), 
 m_capacity(n) 
{
    for (size_t i = 0; i < n; ++i) {
        new (m_data + i) T(); 
    }
}

//有参构造
template <typename T>
vector<T>::vector (size_t n,const T& val ):
 m_data(static_cast<T*>(::operator new(n * sizeof(T)))), 
 m_size(n), 
 m_capacity(2*n) 
{
    for (size_t i = 0; i < n; ++i) {
        new (m_data + i) T(val); 
    }
}

//拷贝构造
template<typename T>
vector<T>::vector (const vector & other)
: m_data(static_cast<T*>(::operator new(other.m_size* sizeof(T)))),
 m_size(other.m_size),
 m_capacity(other.m_capacity)
{
    for(size_t i = 0; i < other.m_size; ++i)
    {
        new(m_data + i) T(other.m_data[i]);
    }
}
//析构函数
template<class T>
vector <T>::~vector()
{
    // 调用每个元素的析构函数
    for (size_t i = 0; i < m_size; ++i) {
        m_data[i].~T();
    }
    ::operator delete(m_data);
    m_data = nullptr;
    m_size = 0;
    m_capacity = 0;
}

//赋值：重载“=”
template<typename T>
vector<T>& vector<T>::operator=(const vector & other)
{
    if (this == &other) {
        return *this;
    }
    vector temp(other); 
    this->swap(temp);

    return *this; 

}

//尾插
template<class T>
void vector<T>:: push_back(const T&val)
{
    if(m_size == m_capacity)
    {
        size_t new_capacity = (m_capacity==0)? 1 : m_capacity*2;
        reserve(new_capacity);
    }
    new (m_data+m_size) T(val);

    m_size++;
}
//尾删
template<class T>
void vector<T>:: pop_back()
{
    if(m_size == 0)
    {
        return ;
    }
    m_data[m_size-1].~T();
    m_size--;
}    

//重新设置大小
template<typename T>
void vector<T>::resize(size_t n) 
{
    if(m_size<n)
    {
        if(m_capacity < n)
        {
            size_t new_capacity = std::max(n, m_capacity * 2);
            T*new_data = static_cast<T*>(::operator new(new_capacity* sizeof(T)));
            for(size_t i=0; i < m_size; ++i )
            {
                new(new_data+i) T(std::move(m_data[i])); 
                m_data[i].~T();
            }

            ::operator delete (m_data);
            m_data=new_data;
            m_capacity = new_capacity;
        }
        
        size_t addNum = n-m_size;
        for(size_t i=0; i<addNum; ++i)
        {
            new(m_data + m_size + i)T();
        }
        m_size = n;

    }
    else if(m_size>n)
    {
       size_t delNum = m_size-n;
       for(size_t i =n; i<m_size; ++i)
       {
            m_data[i].~T();
       }
       m_size =n; 
    }
}

//插入元素
template<class T>
void vector<T>::insert(T * insert_begin_ptr , const T&val)
{
    if (insert_begin_ptr < m_data || insert_begin_ptr > m_data + m_size) {

        return;
    }
    size_t insert_pos = insert_begin_ptr - m_data;
    
    if(m_size==m_capacity)
    {
        size_t new_capacity = (m_capacity == 0) ? 1 : m_capacity * 2;
        T*new_data = static_cast<T*>(::operator new(new_capacity* sizeof(T)));
        for (size_t i = 0; i < insert_pos; ++i) {
            new (new_data + i) T(std::move(m_data[i]));
            m_data[i].~T(); // 析构原元素
        }

        for(size_t i=insert_pos; i<m_size; ++i )
        {
            new(new_data+i+1) T(std::move(m_data[i]));
            m_data[i].~T();
        }
        ::operator delete(m_data);
        m_data = new_data;
        m_capacity = new_capacity;

        insert_begin_ptr = m_data + insert_pos;
    }
    else{
        for(size_t i=m_size; i > insert_pos; --i)
        {
            new(m_data+i)T(std::move(m_data[i-1]));
            m_data[i-1].~T();
        }
    }
    new(insert_begin_ptr)T(val);
    m_size++;
}

//删除指定位置的一个元素；
template <class T>
T* vector<T>::erase(T * erase_ptr )
{
    size_t erase_pos = erase_ptr-m_data;
    if(m_size==0||erase_ptr<m_data||erase_ptr>=m_data+m_size)
    {
        return nullptr ;
    }
    
        
    for(size_t i= erase_pos; i < m_size-1; ++i)
    {
        m_data[i].~T();
        new(m_data+i)T(std::move(m_data[i+1]));
            
    }
    

    m_data[m_size-1].~T();
    m_size--;
    return erase_ptr;
}

//删除指定范围的元素；
template <class T>
T* vector<T>::erase(T* erase_begin_ptr, T* erase_end_ptr)
{
    size_t erase_begin_pos =erase_begin_ptr-m_data;
    size_t erase_end_pos =erase_end_ptr-m_data;
    size_t count=erase_end_pos-erase_begin_pos;

    size_t remain_size = m_size - erase_end_pos; 
    if(m_size==0||erase_begin_ptr<m_data||erase_end_ptr>m_data+m_size||erase_begin_ptr>=erase_end_ptr)
    {
        return erase_begin_ptr;
    }

    for (size_t i=0; i<remain_size;++i)
    {
        m_data[erase_begin_pos+i].~T();
        new(m_data+erase_begin_pos+i)T(std::move(m_data[i+erase_end_pos]));
    }
    for (size_t i = m_size - count; i < m_size; ++i) {
        m_data[i].~T();
    }
    m_size-=count;
    return erase_begin_ptr;

}

//清空元素
template<class T>
void vector<T>::clear()
{
    for(size_t i=0; i<m_size; ++i)
    {
        m_data[i].~T();
    }
    m_size = 0;
}

//读取数据，重载[]
template<class T>
T & vector<T>::operator[](size_t index)
{
    return m_data[index];
}

template<class T>
const T & vector<T>::operator[](size_t index)const
{
    return m_data[index];
}

//at()
template<class T>
const T & vector<T>:: at(size_t index) const
{
    if(index>=m_size||index<0)
    {
        throw std::out_of_range("vector::at: index out of range");
    }
    return m_data[index];
}

template<class T>
T& vector<T>::at(size_t index)
{
    if (index >= m_size) {
        throw std::out_of_range("vector::at: index out of range");
    }
    return m_data[index];
}

template<class T>
const T & vector<T>:: front() const
{
    if (m_size == 0) {
        throw std::out_of_range("vector::front(): empty container");
    }
    return m_data[0];
}
template<class T>
const T & vector<T>::back() const
{
    if (m_size == 0) {
        throw std::out_of_range("vector::back(): empty container");
    }
    return m_data[m_size-1];
}

template<class T>
T& vector<T>:: front()
{
    if (m_size == 0) {
        throw std::out_of_range("vector::front(): empty container");
    }
    return m_data[0];
}
template<class T>
T& vector<T>::back()
{   
    if (m_size == 0) {
        throw std::out_of_range("vector::back(): empty container");
    }
    return m_data[m_size-1];
}

template<class T>
void vector<T>::swap(vector & v)
{
    vector temp(v);
    //判断两容器的大小，如果大小不够应该对容器进行扩容
    //完成容器的互换
    std::swap(m_data, v.m_data);
    std::swap(m_size, v.m_size);
    std::swap(m_capacity, v.m_capacity);

}

template<class T>
void vector<T>::reserve(size_t new_capacity)
{
    if(new_capacity<=m_capacity)
    {
        return ;
    }

    T*new_data = static_cast<T*>(::operator new(new_capacity*sizeof(T)));
    size_t constructed = 0;
    try{
        for(size_t i = 0; i < m_size; ++i){
            new(new_data + i) T(std::move(m_data[i]));
            constructed++;
        }
    }catch(...){
        for(size_t i = 0; i < constructed; ++i)
        {
            new_data[i].~T();
        }
        ::operator delete(new_data);
        throw;
    }
    
    for(size_t i = 0;i<m_size;i++){
        m_data[i].~T();
    }
    ::operator delete(m_data);
    m_data = new_data;
    m_capacity = new_capacity;
}