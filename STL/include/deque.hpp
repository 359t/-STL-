#pragma once
#include<iostream>
#include <stdexcept>


template <class T>
class deque
{
private:

    //分块参数大小
    static const size_t BLOCK_SIZE = 512;
    static const size_t MAP_INIT_SIZE = 8;

    T** m_map ;             //指针数组，每个指向一个数据块；
    size_t m_map_size;      //指针数组的大小
    size_t m_start_block;   //第一个有效数据块索引
    size_t m_start_index;   //第一个有效元素索引；
    size_t m_size;          //元素总数；

public:

    //构造
    deque () = default;
    deque (const size_t n, const T& val);
    deque (const deque<T>& other);

    //移动构造
    deque(deque<T>&& other) noexcept;

    //初始化列表
    deque(std::initializer_list<T> init);

    //析构
    ~deque ();
    
    //赋值
    deque<T>& operator =(const deque<T>& other);
    deque<T>& operator =(deque<T>&& other) noexcept;
    deque<T>& operator =(std::initializer_list<T> init);

    void assign(size_t n, const T&val);
    void assign (T* begin_ptr, T* end_ptr);
    void assign(std::initializer_list<T> init);

    //尾插和尾删//头插和头删
    void push_back( const T&val);
    void push_bach(T&& val);
    void pop_back();
    void push_front( const T&val);
    void push_front(T&& val);
    void pop_front();

    //大小
    bool empty() const { return m_size == 0;};
    size_t size () const {return m_size;};
    size_t max_size() const {return size_t(-1);};
    void resize (size_t size );
    void resize (size_t num, size_t elem);

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
    class iterator;
    class const_iterator;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    //交换
    void swap(deque<T>& other) noexcept;

private:
    //重新分配内存辅助函数：
    void allocate_map(size_t new_map_size);
    void allocate_blocks(size_t start, size_t end);
    void deallocate_blocks(size_t start, size_t end);
    
    //内部迭代器位置计算
    iterator internal_begin();
    iterator internal_end();

    //在pos位置之前或之后移动元素
    void move_elements_forward(iterator pos, size_t n);
    void move_elements_backward(iterator pos, size_t n);
    
};

template<class T>
class iterator{
private:
    T** m_current_block; //当前所在块的指针；
    T* m_current;        //当前元素指针；
    T* m_block_begin;    //当前块起始位置；
    T* m_block_end;      //当前块结束位置；

    friend class deque<T>;
    friend class const_iterator;

    void set_block(T** new_block);

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference =T&;

    iterator(): m_current_block(nullptr), m_current(nullptr),
                m_block_begin(nullptr), m_block_end(nullptr){}

    iterator(T** block_ptr, T* elem_ptr):
        m_current_block(block_ptr), m_current(elem_ptr)
    {
        if(block_ptr && *block_ptr){
            m_block_begin = *block_ptr;
            m_block_end = *block_ptr + BLOCK_SIZE;
        }
        else{
            m_block_begin = m_block_end = nullptr;
        }
    }

    //迭代器操作
    reference operator*() const{return *m_current;}
    pointer operator->() const{return m_current;}
    iterator& operator++(){
        ++m_current;
        if(m_current == m_block_end){
            ++m_current_block;
            m_block_begin= *m_current_block;
            m_block_end = m_block_begin + BLOCK_SIZE;
            m_current = m_block_begin;
        }
        return *this;
    }
    iterator operator++(int){
        iterator tem = *this;
        ++(*this);
        return tmp;
    }

    iterator& operator--(){
        if(m_current == m_block_end){
            --m_current_block;
            m_block_begin = *m_current_block;
            m_block_end = m_block_begin + BLOCK_SIZE;
            m_current = m_block_end;
        }
        --m_current;
        return *this;
    }
    iterator operator--(int){
        iterator tmp = *this;
        --(*this);
        return tmp;
    }

    iterator& operator +=(difference_type n){
        if(n >= 0){
            while(n > 0){
                difference_type elements_in_block = m_block_end - m_current;
                if(n < elements_in_block){
                    m_current += n;
                    break;
                }
                n -=elements_in_block;
                ++m_current_block;
                m_block_begin = *m_current_block;
                m_block_end = m_block_begin + BLOCK_SIZE;
                m_current = m_block_begin;
            }
        }
        else{
            operator-=(-n);
        }
        return *this;
    }
    iterator& operator -=(difference_type n){
        if(n >= 0){
            while(n > 0){
                difference_type elements_in_block = m_current - m_block_begin;
                if(n <= elements_in_block){
                    m_current -= n;
                    break;
                   }
            n -= elements_in_block + 1;
            --m_current_block;
            m_block_begin = *m_current_block;
            m_block_end = m_block_begin + BLOCK_SIZE;
            m_current = m_block_end - 1;
            }
        }else{
            operator+=(-n);
        }
        return *this;
    }
    iterator operator+(difference_type n)const{
        iterator tmp = *this;
        tmp += n;
        return tmp;
    }
    iterator operator-(difference_type n)const{
        iterator tmp = *this;
        tmp += n;
        return tmp;
    }
    friend iterator operator+(difference_type n, const iterator& it){
        return it + n;
    }
    difference_type operator-(const iterator& other) const{
        if(m_current_block == other.m_current_block){
            return m_current - other.m_current;
        }
        difference_type blocks_diff = m_current_block - other.m_current_block;
        return blocks_diff * BLOCK_SIZE + 
               (m_current - m_block_begin) -
               (other.m_current - other.m_block_begin);
    }

    bool operator ==(const iterator& other) const{
        return m_current == other.m_current;
    }
    bool operator !=(const iterator& other) const{
        return !(*this == other);
    }
    bool operator <(const iterator& other) const{
        if(m_current_block == other.m_current_block){
            return m_current < other.m_current;
        }
        renturn m_current_block < other.m_current_block;
    }
    bool operator >(const iterator& other) const{
        return other < *this;
    }
    bool operator <=(const iterator& other) const{
        return !(*this > other);
    }
    bool operator >=(const iterator& other) const{
        return !(*this < other); 
    }


    // 辅助函数
    void set_block(T** new_block){
        m_current_block = new_block;
       m_block_begin = *new_block;
       m_block_end = m_block_begin + BLOCK_SIZE;
    }
};

template<class T>
class const_iterator
{
private:
    T** m_current_block;
    const T* m_current;
    const T* m_block_begin;
    const T* m_block_end;

    friend class deque<T>;

    void set_block(T** new_block);
        
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using reference =const T&;

    const_iterator() : m_current_block(nullptr), m_current(nullptr),
                      m_block_begin(nullptr), m_block_end(nullptr){}
    const_iterator(T** block_ptr, const T* elem_ptr) :
        m_current_block(block_ptr), m_current(elem_ptr)
    {
        
    }
};

template <class T>
deque<T>::deque(const size_t n, const T& val){

};
