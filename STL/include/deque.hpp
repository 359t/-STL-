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
    void push_back(const T&val);
    void push_back(T&& val);
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
    void destroy_elements();
    void deallocate_blocks(size_t start, size_t end);
    void reallocate_map(size_t new_map_size);
    void reallocate_map_for_front(size_t new_map_size);
    
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
        m_current_block(block_ptr), m_current(elem_ptr){
        if(block_ptr && *block_ptr){
            m_block_begin = *block_ptr;
            m_block_end = *block_ptr + BLOCK_SIZE;
        }else{
            m_block_begin = m_block_end = nullptr;
        }
    }

    const_iterator(const iterator& other):
    m_current_block(other.m_current_block),
    m_current(other.m_current),
    m_block_begin(other.m_block_begin),
    m_block_end(other.m_block_end){}

    //迭代器操作
    reference operator*() const { return *m_current; }
    pointer operator->() const { return m_current; }

    const_iterator& operator++() {
        ++m_current;
        if (m_current == m_block_end) {
            ++m_current_block;
            m_block_begin = *m_current_block;
            m_block_end = m_block_begin + BLOCK_SIZE;
            m_current = m_block_begin;
        }
        return *this;
    }

    const_iterator operator++(int) {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    const_iterator& operator--() {
        if(m_current == m_block_begin) {
            --m_current_block;
            m_block_begin = *m_current_block;
            m_block_end = m_block_begin + BLOCK_SIZE;
            m_current = m_block_end;
        }
        --m_current;
        return *this;
    }

    const_iterator operator--(int) {
        const_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    const_iterator& operator+=(difference_type n){
        if( n>=0 ) {
            while(n > 0) {
                difference_type elements_in_block =m_block_end - m_current;
                if( n < elements_in_block){
                    m_current +=n;
                    break;
                }
                n -= elements_in_block;
                ++m_current_block;
                m_block_begin = *m_current_block;
                m_block_end = m_block_begin + BLOCK_SIZE;
                m_current = m_block_begin;
            }
        } else{
            operator-=(-n);
        }
        return *this;
    }

    const_iterator operator+(difference_type n) const {
        const_iterator tmp = *this;
        tmp +=n;
        return tmp;
    }
    const_iterator operator-(difference_type n) const {
        const_iterator tmp = *this;
        tmp -=n;
        return tmp;
    }

    friend const_iterator operator+(difference_type n, const const_iterator& it){
        return it + n;
    }

    difference_type operator-(const const_iterator& other) const{
        if(m_current_block == other.m_current_block){
            return m_current - other.m_current;
        }
        difference_type blocks_diff = m_current_block - other.m_current_block;
        return blocks_diff * BLOCK_SIZE + 
               (m_current - m_block_begin) -
               (other.m_current - other.m_block_begin);
    }

    bool operator ==(const const_iterator& other) const{
        return m_current == other.m_current;
    }
    bool operator !=(const const_iterator& other) const{
        return !(*this == other);
    }
    bool operator <(const const_iterator& other) const{
        if(m_current_block == other.m_current_block){
            return m_current < other.m_current;
        }
        renturn m_current_block < other.m_current_block;
    }
    bool operator >(const const_iterator& other) const{
        return other < *this;
    }
    bool operator <=(const const_iterator& other) const{
        return !(*this > other);
    }
    bool operator >=(const const_iterator& other) const{
        return !(*this < other); 
    }


    // 辅助函数
    void set_block(T** new_block){
        m_current_block = new_block;
       m_block_begin = *new_block;
       m_block_end = m_block_begin + BLOCK_SIZE;
    }
    
};




template <class T>
deque<T>::deque(const size_t n, const T& val)
:m_map(nullptr),
 m_map_size(0),
 m_start_block(0),
 m_start_index(0),
 m_size(0){
    
    if(n == 0){
        allocate_map(MAP_INIT_SIZE);
        return;
    }
    size_t needed_blocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;

    m_map_size = std::max(MAP_INIT_SIZE, needed_blocks + 2);

    allocate_map(m_map_size);
    
    m_start_block = (m_map_size - needed_blocks) / 2;
    m_start_index = 0;
    m_size = n;
    try{

        allocate_blocks(m_start_block, m_start_block + needed_blocks);

        size_t current_block = m_start_block;
        size_t remaining = n;
        T* block_ptr = m_map[current_block];
        size_t index_in_block = 0;

        while (remaining > 0) {

            size_t elements_in_this_block = std::min(remaining, BLOCK_SIZE -index_in_block);

            for (size_t i = 0; i < elements_in_this_block; ++i){
                new (block_ptr + index_in_block + i) T(val);
            }
            remaining -= elements_in_this_block;
            index_in_block += elements_in_this_block;

            if( index_in_block == BLOCK_SIZE && remaining > 0) {
                ++current_block;
                block_ptr = m_map[current_block];
                index_in_block = 0;
            }
        }    
    }
    catch(...){
        destroy_elements();
        deallocate_blocks(m_start_block, m_start_block + (n + BLOCK_SIZE - 1 ) / BLOCK_SIZE);

        if (m_map) {
            delete[] reinterpret_cast<char*>(m_map);
            m_map = nullptr;
        }
        throw;       
    }
};

template <class T>
deque<T>::deque (const deque<T>& other){
    m_map = nullptr;
    m_map_size = 0;
    m_size = 0;
    m_start_block = 0;
    m_start_index = 0;

    if(other.empty()){
        allocate_map(MAP_INIT_SIZE);
        return;
    }
    size_t needed_blocks = (other.m_size + BLOCK_SIZE - 1)/ BLOCK_SIZE;

    m_map_size = other.m_map_size;
    allocate_map(m_map_size);

    m_start_block = other.m_start_block;
    m_start_index = other.m_start_index;
    m_size = other.m_size;

    try{

        size_t first_used_block = m_start_block;
        size_t last_used_block = m_start_block + needed_blocks - 1;
        if (m_start_index > 0 && needed_blocks >1){
            last_used_block++;
        }
        for (size_t = first_used_block; i<= last_used_block; ++i){
            if(other.m_map[i]){
                m_map[i] = reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
            }
        }
        auto src_it = other.begin();
        auto dst_it = begin();

        while (src_it != other.end()){

            new (&(*dst_it)) T(*src_it);
            ++src_it;
            ++dst_it;
        }

    }
    catch(...){
        clear();
        if(m_map){
            delete[] reinterpret_cast<char*>(m_map);
            m_map = nullptr;
        }
        throw;
    }
}

  //移动构造
template<class T>
deque<T>::deque(deque<T>&& other)noexcept
    :m_map(other.m_map),
     m_map_size(other.m_map_size),
     m_start_block(other.m_start_block),
     m_start_index(other.m_start_index),
     m_size(other.m_size){
    
    //将other置为空状态
    other.m_map = nullptr;
    other.m_map_size = 0;
    other.m_start_block = 0;
    other.m_start_index = 0;
    other.m_size = 0;
}

template<class T>
deque<T>::deque(std::initializer_list<T> init)
: m_map(nullptr),
  m_map_size(0),
  m_start_block(0),
  m_start_index(0),
  m_size(0){
    if(init.size()>0){
        size_t needed_blocks = (init.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;
        
        m_map_size = std::max(MAP_INIT_SIZE, needed_blocks + 4);
        allocate_map(m_map_size);

        m_start_block = m_map_size / 2 - needed_blocks / 2;
        m_start_index = 0;
        m_size = init.size();

        for (size_t i = 0; i < needed_blocks; ++i){
            m_map[m_start_block + i] = reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
        }

        auto src =init.begin();
        for(size_t block =0; block < needed_blocks; ++block){
            T* block_ptr = m_map[m_start_block + block];
            size_t elements_in_block = (block == needed_blocks -1)
            ?(init.size() - block*BLOCK_SIZE)
            :BLOCK_SIZE;

            for (size_t i = 0; i < elements_in_block; ++i){
                new(block_ptr + i) T(*src++);
            }
        }
    }else{
        allocate_map(MAP_INIT_SIZE);
    }
}

//析构函数
template<class T>
deque<T>::~deque (){}

//待验证
template<class T>
deque<T>& deque<T>::operator =(std::initializer_list<T> init){

    if(init.size() == 0) {
        clear();
        return *this;
    }

    try {

        deque<T temp;

        size_t needed_blocks = (init.size() + BLOCK_SIZE - 1) / BLOCK_SIZE;
        size_t new_map_size = std::max(MAP_INIT_SIZE, needed_blocks * 2);

        temp.allocate_map(new_map_size);
        temp.m_start_block = (new_map_size - needed_blocks) / 2;
        temp.m_start_index = 0;
        temp.m_size = init.size();
        temp.m_map_size = new_map_size;

        temp.allocate_blocks(temp.m_start_block, temp.m_start_block + needed_blocks);

        auto src = init.begin();
        for (size_t block_idx = 0; block_idx < needed_blocks; ++block_idx){
            T* block_ptr = temp.m_map[temp.m_start_block +block_idx];
            size_t elements_in_block = (block_idx == needed_blocks - 1)
            ? (init.size() - block_idx * BLOCK_SIZE)
            :BLOCK_SIZE;

        for (size_t i = 0; i < elements_in_block; ++i) {
            new (block_ptr + i) T(*src++);
            }
        }
        swap(temp);
    }catch(...){
        throw;
    }
    
    return *this;
}


template<class T>
deque<T>& deque<T>::operator=(deque<T>&& other)noexcept{
    if (this != &other) {
        clear();
    }
    if(m_map) {
        delete[] reinterpret_cast<char*>(m_map);
    }
    m_map = other.m_mapp;
    m_map_size = other.m_map_size;
    m_star_index = other.m_start_index;
    m_size = other.m_size;

    other.m_map = nullptr;
    other.m_map_size = 0;
    other.m_start_block = 0;
    other.m_start_index = 0;
    other.m_size = 0;
}

template<class T>
void deque<T>::assign(size_t n, const T&val){
    if( n == 0) {
        clear();
        return;
    }

    deque<T> temp;

    try{
        size_t needed_blocks = (n+ BLOCK_SIZE - 1) / BLOCK_SIZE;
        size_t map_size = std::max(MAP_INIT_SIZE, needed_blocks * 2);

        temp.allocate_map(map_size);
        temp.m_start_block = (map_size - needed_blocks) / 2;
        temp.m_start_index = 0;
        temp.m_size = n;
        temp.m_map_size = map_size;

        temp.allocate_blocks(temp.m_start_block, temp.m_start_block + needed_blocks);

        size_t current_block = temp.m_start_block;
        size_t remaining = n;
        size_t index_in_block = 0;

        while (remaining > 0)
        {
            T* block_ptr = temp.m_map[current_block];
            size_t elements_in_this_block = std:: min(remaining, BLOCK_SIZE - index_in_block);

            for(size_t i = 0; i < elements_in_this_block; ++i){
                new( block_ptr + index_in_block + i) T(val);
            }

            remaining -= elements_in_this_block;
            if(index_in_block + elements_in_this_block == BLOCK_SIZE && remaining > 0){
                current_block++;
                index_in_block = 0;
            } else{
                index_in_block +=elements_in_this_block;
            }
        }

        swap(temp);
        
    }catch(...){
        throw;
    }
}

template<class T>
void deque<T>::assign (T* begin_ptr, T* end_ptr){
    if(!begin_ptr || !end_ptr || begin_ptr > end_ptr){
        throw std::invalid_argument("Invalid pointer range");
    }

    size_t n = end_ptr - begin_ptr;

    if (is_range_within_deque(begin_ptr, end_ptr)){

        vector<T> temp(begin_ptr, end_ptr);
        deque<T> new_deque(temp.data(), temp.data() + temp.size());
        swap(new_deque); 
    } else {
        deque<T> new_deque(begin_ptr, end_ptr);
        swap(new_deque);
    }
}

template<class T>
void deque<T>::assign(std::initializer_list<T> init){
    *this = init;
}

template<class T>
void deque<T>::push_back(const T&val){

    if(m_size == 0){
        if (m_map == nullptr){
            allocate_map(MAP_INIT_SIZE);
            m_start_block = m_map_size / 2;
            m_start_index = 0;
        }

        if(m_map[m_start_block] == nullptr){
            m_map[m_start_block] = reinterpret_cast<T*>(
                new char[BLOCK_SIZE * sizeof(T)];
            )
        }
        new (m_map[m_start_block] + m_start_index) T(val);
        m_size = 1;
        return;
    }
    //插入新元素；
    size_t total_pos = m_start_index + m_size;
    size_t block_idx = m_start_block + total_pos / BLOCK_SIZE;
    size_t pos_in_block = total_pos% BLOCK_SIZE;

    //检查是否需要新块
    if (pos_in_block == 0){

        if(block_idx >= m_map_size){
            size_t new_size = m_map_size;
            while (new_size <= block_idx + 2){
                new_size *=2;
            }
            reallocate_map(new_size);

            total_pos = m_start_index + m_size;
            block_idx = m_start_block + total_pos / BLOCK_SIZE;
            pos_in_block = total_pos % BLOCK_SIZE;
        }
        
        if (m_map[block_idx] ==nullptr){
            m_map[block_idx] =reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
        }
    }

    new (m_map[block_idx] + pos_in_block) T(val);
    m_size++;
}

template <class T>
void deque<T>::push_back(T&& val){
    if(empty()){
        new (m_map[m_start_block] + m_start_index) T(std::move(val));
        m_size = 1;
        return;
    }   
    size_t next_position = m_start_index + m_size;
    size_t block_index = m_start_block + next_position / BLOCK_SIZE;
    size_t position_in_block = next_position % BLOCK_SIZE;

    if(position_in_block >= m_map_size) {

        size_t new_map_size = m_map_size * 2;
        while (new_map_size <= block_index + 2){
            new_map_size *= 2;
        }
        reallocate_map(new_map_size);

        next_position = m_start_index + m_size;
        block_index = m_start_block + next_position / BLOCK_SIZE;
        position_in_block = next_position%BLOCK_SIZE;
    }
    if (m_map[block_index] == nullptr){
        m_map[block_index] = reinterpret_cast<T*>(
            new char[BLOCK_SIZE * sizeof(T)]);
    }
    new(m_map[block_index] == nullptr){
        m_map[block_index] = reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
    }
}

template<class T>
void deque<T>::pop_back(){
    if (empty()){
        throw std::out_of_range("deque:: pop_back: deque is empty");
    }

    size_t total_positions = m_start_index + m_size;
    size_t last_block_index = m_start_block + (total_positions - 1) / BLOCK_SIZE;
    size_t last_element_index = (total_positions -1) % BLOCK_SIZE;

    T* last_element_ptr = m_map[last_block_index] + last_element_index;
    last_element_ptr->~T();

    m_size--;

    if(m_size == 0){
        m_start_index = 0;
    }
    else{
        size_t new_total_positions = m_start_index + m_size;
        size_t new_last_block_index = m_start_block + (new_total_positions -1) / BLOCK_SIZE;

        if (new_last_vlock_index < last_block_index){

            if (m_map[last_block_index]){
                delete[] reinterpret_cast<char*>(m_map[last_block_index]);
                m_map[last_block_index] = nullptr;
            }
        }
    }
}

template<class T>
void deque<T>:: push_front( const T&val){
    if(m_size == 0) {
        if(m_map == nullptr) {
            allocate_map(MAP_INIT_SIZE);
            m_start_block = m_map_size * 3/4;
            m_start_index = BLOCK_SIZE / 2;
        }else if (m_map[m_start_block] == nullptr){
            m_map[m_start_block] = reinterpret_cast<T*>(
                new char[BLOCK_SIZE * sizeof(T)]);
        }
        new (m_map[m_start_block] + m_start_index) T(std::forward<Args>(args)...);
        m_size = 1;
        return;
    }

    if (m_start_index > 0) {
        m_start_index--;
        new(m_map[m_start_block] + m_start_index) T(std::forward<Args>(args)...);
    }
    else{
        if (m_start_block ==0){
            size_t new_size = m_map_size * 2;
            while (new_size = m_map_size + 4){
                new_size *= 2;
            }
            reallocate_map_for_front(new_size);
        }
        m_start_block--;

        if (m_map[m_start_block]  ==nullptr) {
            m_map[m_start_block] = reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
        }
        m_start_index = BLOCK_SIZE - 1;
        new(m_map[m_start_block] + m_start_index) T(std::forward<Args>(args)...);
    }
    m_size++;
}

template<class T>
typename deque<T>::iterator deque<T>::begin(){
    if(empty()){
        return iterator(nullptr, nullptr);
    }
    return iterator(& m_map[m_start_block], m_map[m_start_block] + m_start_index);
}

template<class T>
typename deque<T>::iterator deque<T>::end(){
    if(empty()){
        return iterator(nullptr, nullptr)
    }
    size_t total_elements = m_start_inex + m_size ;
    size_t last_block = m_start_block + (total_elements - 1)/ BLOCK_SIZE;
    size_t index_in_last_block = (total_elements - 1)% BLOCK_SIZE; 

    return iterator(&m_map[last_block],m_map[last_block] + index_in_last_block +1);
}


//辅助函数实现
template <class T>
void deque<T>::allocate_map(size_t new_map_size){

    m_map = reinterpret_cast<T**>(new char[new_map_size * sizeof(*T)]);
    m_map_size = new_map_size;

    for(size_t i= 0; i < m_map_size; ++i){
        m_map[i] = nullptr;
    }
}

template <class T>
void deque<T>::allocate_blocks(size_t start_block, size_t end_block){

    for (size_t i = start_block; i < end_block; ++i){
        m_map[i] = reinterpret_cast<T*>(new char[BLOCK_SIZE * sizeof(T)]);
    }
}

template <class T>
void deque<T>::destroy_elements(){
    if (m_size == 0 || !m_map ) return;

    size_t current_block = m_start_block;
    size_t index_in_block = m_start_index;
    size_t remaining = m_size;

    while (remaining > 0) {
        T* block_ptr = m_map[current_block];
        size_t elements_in_this_block = std::min(remaining, BLOCK_SIZE -index_in_block);

        for (size_t i = 0; i < elements_in_this_block; ++i){
            (block_ptr + index_in_block + i)->~T();
        }
        remaining -= elements_in_this_block;
        index_in_block = 0;
        ++current_block;
    }
}

template <class T>
void deque<T>::deallocate_blocks(size_t start_block, size_t end_block) {

    for(size_t i = start_block; i < end_block; ++i){
        if (m_map[i]){

            delete[] reinterpret_cast<char*>(m_map[i]);
            m_map[i] = nullptr;
        }
    }
} 

template<class T>
void deque<T>::reallocate_map(size_t new_map_size){
    if (new_map_size <= m_map_size){
        return;
    }

    T** new_map = reinterpret_cast<T**>(new char[new_map_size * sizeof(T*)]);

    for (size_t i = 0; i < new_map_size; ++i){
        new_map[i] = nullptr;
    }

    size_t blocks_used = 0;
    if(m_size > 0){

        size_t first_used_block = m_start_block;
        size_t total_positions = m_start_index + m_size;
        size_t last_used_block = m_start_block + (total_positions - 1) / BLOCK_SIZE;

        for (size_t i = first_used_block; i <= last_used_block; ++i){
            if(i < m_map_size && m_map[i] != nullptr){
                new_map[new_start_block + (i - first_used_block)] = m_map[i];
            }
        }

        m_start_block = new_start_block;
    }
    else{
        m_start_block = new_map_size / 2;
        m_start_index = 0;
    }
    if(m_map){
        delete[] reinterpret_cast<char*>(m_map);
    }

    m_map = new_map;
    m_map_size = new_map_size;
}

template<class T>
void deque<T>::reallocate_map_for_front(size_t new_map_size) {
    if (new_map_size <= m_map_size) return;
    
    T** new_map = reinterpret_cast<T**>(new char[new_map_size * sizeof(T*)]);
    std::fill_n(new_map, new_map_size, nullptr);
    
    if (m_size > 0) {
        // 计算当前使用的块
        size_t total_pos = m_start_index + m_size;
        size_t first_block = m_start_block;
        size_t last_block = m_start_block + (total_pos - 1) / BLOCK_SIZE;
        size_t blocks_used = last_block - first_block + 1;
        
        // 将数据放在新map的后2/3处，为前端留出1/3空间
        size_t new_start = new_map_size * 2 / 3 - blocks_used;
        
        for (size_t i = 0; i < blocks_used; ++i) {
            new_map[new_start + i] = m_map[first_block + i];
            m_map[first_block + i] = nullptr;
        }
        
        m_start_block = new_start;
    } else {
        m_start_block = new_map_size * 2 / 3;
        m_start_index = BLOCK_SIZE - 1;
    }
    
    if (m_map) {
        delete[] reinterpret_cast<char*>(m_map);
    }
    
    m_map = new_map;
    m_map_size = new_map_size;
}