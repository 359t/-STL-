#pragma once
#include<iostream>
#include <stdexcept>



template <class T>
class stack
{
private:

    T* m_data;
    size_t m_size;
    size_t m_capacity;
    size_t m_top;

public:

    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;

    stack() noexcept;

    //指定初始容量
    explicit stack(size_t initial_capacity);
    //拷贝
    stack(const stack& other);
    //移动构造
    stack(stack&& other) noexcept;
    //初始化列表构造
    stack(std::initializer_list<value_type> init);

    //析构函数
    ~stack();

    //赋值运算符
    stack& operator=(const stack& other);
    //移动赋值
    stack& operator=(stack&& other) noexcept;
    //从初始化列表赋值
    stack& operator=(std::initializer_list<value_type> init);

    //元素访问
    reference top(); //访问栈顶元素
    const_reference top()const;
    
    reference at(size_t n);
    const_reference at(size_t n) const;
    reference operator[](size_t n);
    const_reference operator[](size_t n) const;

    //容量操作
    bool empty() const noexcept;
    //返回元素数量
    size_t size() const noexcept;
    //返回容量
    size_t capacity() const noexcept;
    //预留容量
    void reserve(size_t new_capacity);
    //收缩容量到合适大小
    void shrink_to_fit();

    //修改器
    //在栈顶插入元素（拷贝)
    void push(const value_type& value);
    //在栈顶插入元素（移动）
    void push(value_type&& value);
    //在栈顶构造元素
    template<typename... Args>
    reference emplace(Args&&... args);
    //移除栈顶元素
    void pop();
    //清空栈（不释放内存）
    void clear() noexcept;
    //交换
    void swap(stack& other) noexcept;

    //比较运算符
    //相等比较
    bool operator==(const stack& other) const;
    bool operator!=(const stack& other) const;
    //字典序比较
    bool operator<(const stack& other) const;
    bool operator<=(const stack& other) const;
    bool operator>(const stack& other) const;
    bool operator>=(const stack& other) const;

    //迭代器支持
    //前向迭代器
    class iterator;
    class const_iterator;
    
    iterator begin() noexcept { return iterator(m_data);}
    iterator end() noexcept { return iterator(m_data + m_size);}
    const_iterator begin() const noexcept { return const_iterator(m_data); }
    const_iterator end() const noexcept { return const_iterator( m_data+m_size); }
    const_iterator cbegin() const noexcept { return const_iterator(m_data); }
    const_iterator cend() const noexcept{ return const_iterator(m_data + m_size); }

    //反向迭代器
    class reverse_iterator;
    class const_reverse_iterator;

    reverse_iterator rbegin() noexcept {  return reverse_iterator(m_data + m_size); }
    reverse_iterator rend() noexcept { return reverse_iterator(m_data); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(m_data + m_size);}
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(m_data); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(m_data + m_size); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(m_data); }

private:
//私有辅助方法
    //重新分配内存
    void reallocate(size_t new_capacity);
    //确保有足够容量
    void ensure_capacity(size_t min_capacity);
    //释放所有内存
    void deallocate_all();
    //复制元素从源到目标
    void copy_elements( T*dest, const T* src, size_t count);
    //移动元素从源到目标
    void move_elements(T*dest, T*src, size_t count);
    //默认初始容量
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr size_t GROWTH_FACTOR = 2;
    

};

template<typename T>
class stack<T>::iterator {
private:
    T* m_ptr;
    
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    
    // 构造函数
    explicit iterator(T* ptr = nullptr) noexcept : m_ptr(ptr) {}
    
    // 解引用
    reference operator*() const {return *m_ptr; }
    pointer operator->() const { return m_ptr; }
    reference operator[](difference_type n) const { return m_ptr[n]; }
    
    // 递增递减
   iterator& operator++() { ++m_ptr; return *this; }
    iterator operator++(int) { iterator temp = *this; ++m_ptr; return temp; }
    iterator& operator--() { --m_ptr; return *this; }
    iterator operator--(int) { iterator temp = *this; --m_ptr; return temp; }
    
    // 算术运算
    iterator operator+(difference_type n) const { return iterator(m_ptr + n); }
    iterator operator-(difference_type n) const { return iterator(m_ptr -n); }
    difference_type operator-(const iterator& other) const { return m_ptr - other.m_ptr; }
    
    // 关系运算
    bool operator==(const iterator& other) const { return m_ptr ==other.m_ptr; }
    bool operator!=(const iterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const iterator& other) const { return m_ptr < other.m_ptr; }
    bool operator<=(const iterator& other) const { return m_ptr > otehr.m_ptr; }
    bool operator>(const iterator& other) const { return m_ptr > other.m_ptr; }
    bool operator>=(const iterator& other) const { return m_ptr >= other.m_ptr; }
    
    // 复合赋值
    iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
    iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }
    
    //与整数的计算（需要定义为友元）
    friend iterator operator+ (difference_type n, const iterator& it){
        return iterator(it.m_ptr + n);
    }
};

template<typename T>
class stack<T>::const_iterator{
private:
    const T* m_ptr;

public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference =const T&;

    explicit const_iterator(const T* ptr = nullptr) noexcept : m_ptr(ptr) {}
    const_iterator( const iterator& it) noexcept : m_ptr(it.m_ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() const { return m_ptr; }
    reference operator[](difference_type n) const { return m_ptr[n]; }

    const_iterator& operator++() { ++m_ptr; return *this; }
    const_iterator operator++(int) { const_iterator temp = *this; ++m_ptr; return temp; }
    const_iterator& operator-- () { --m_ptr; return *this; }
    const_iterator operator--(int) { const_iterator temp = *this; --m_ptr; return temp; }

    const_iterator operator+(difference_type n) const { return const_iterator( m_ptr + n); }
    const_iterator operator-(difference_type n) const { return const_iterator( m_ptr - n); }
    const_iterator operator-(const const_iterator& other) const { return m_ptr - other.m_ptr; }

    bool operator==(const const_iterator& other) const{ return m_ptr == other.m_ptr;}
    bool operator!=(const const_iterator& other) const{ return m_ptr != other.m_ptr;}
    bool operator<(const const_iterator& other) const{ return m_ptr < other.m_ptr;}
    bool operator<=(const const_iterator& other) const{ return m_ptr <= other.m_ptr;}
    bool operator>(const const_iterator& other) const{ return m_ptr > other.m_ptr;}
    bool operator>=(const const_iterator& other) const{ return m_ptr >= other.m_ptr;}

    const_iterator& operator+=(difference_type n) { m_ptr += n; return *this; }
    const_iterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

    friend const_iterator operator+(difference_type n, const const_iterator& it) {
        return const_iterator(it.m_ptr + n);
    }
};

template<typename T>
class stack<T>::reverse_iterator{
private:
    T* m_ptr;
public:
    using iterator_category = std::random_acces_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using referencce = T&;

    explicit reverse_iterator(T* ptr = nullptr) noexcept : m_ptr(ptr) {}

    referencce operator*() const { return *(m_ptr - 1); }
    pointer operator->() const { return m_ptr - 1; }
    reference operator[](difference_type n) const { return *(m_ptr - 1 -n); }

    // 递增递减（方向相反）
    reverse_iterator& operator++() { --m_ptr; return *this; }
    reverse_iterator operator++(int) { reverse_iterator temp = *this; --m_ptr; return temp; }
    reverse_iterator& operator--() { ++m_ptr; return *this; }
    reverse_iterator operator--(int) { reverse_iterator temp = *this; ++m_ptr; return temp; }
    
    // 算术运算
    reverse_iterator operator+(difference_type n) const { return reverse_iterator(m_ptr - n); }
    reverse_iterator operator-(difference_type n) const { return reverse_iterator(m_ptr + n); }
    difference_type operator-(const reverse_iterator& other) const { return other.m_ptr - m_ptr; }
    
    // 关系运算
    bool operator==(const reverse_iterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const reverse_iterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const reverse_iterator& other) const { return m_ptr > other.m_ptr; }
    bool operator<=(const reverse_iterator& other) const { return m_ptr >= other.m_ptr; }
    bool operator>(const reverse_iterator& other) const { return m_ptr < other.m_ptr; }
    bool operator>=(const reverse_iterator& other) const { return m_ptr <= other.m_ptr; }
    
    // 复合赋值
    reverse_iterator& operator+=(difference_type n) { m_ptr -= n; return *this; }
    reverse_iterator& operator-=(difference_type n) { m_ptr += n; return *this; }
    
    // 转换为普通iterator
    iterator base() const { return iterator(m_ptr); }
    
    friend reverse_iterator operator+(difference_type n, const reverse_iterator& it) {
        return reverse_iterator(it.m_ptr - n);
    }


};

template<typename T>
class stack<T>::const_reverse_iterator {
private:
    const T* m_ptr;
    
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = const T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
    
    explicit const_reverse_iterator(const T* ptr = nullptr) noexcept : m_ptr(ptr) {}
    const_reverse_iterator(const reverse_iterator& it) noexcept : m_ptr(it.base()) {} // 转换
    
    // 解引用
    reference operator*() const { return *(m_ptr - 1); }
    pointer operator->() const { return m_ptr - 1; }
    reference operator[](difference_type n) const { return *(m_ptr - 1 - n); }
    
    // 递增递减
    const_reverse_iterator& operator++() { --m_ptr; return *this; }
    const_reverse_iterator operator++(int) { 
        const_reverse_iterator temp = *this; 
        --m_ptr; 
        return temp; 
    }
    const_reverse_iterator& operator--() { ++m_ptr; return *this; }
    const_reverse_iterator operator--(int) { 
        const_reverse_iterator temp = *this; 
        ++m_ptr; 
        return temp; 
    }
    
    // 算术运算
    const_reverse_iterator operator+(difference_type n) const { 
        return const_reverse_iterator(m_ptr - n); 
    }
    const_reverse_iterator operator-(difference_type n) const { 
        return const_reverse_iterator(m_ptr + n); 
    }
    difference_type operator-(const const_reverse_iterator& other) const { 
        return other.m_ptr - m_ptr; 
    }
    
    // 关系运算
    bool operator==(const const_reverse_iterator& other) const { return m_ptr == other.m_ptr; }
    bool operator!=(const const_reverse_iterator& other) const { return m_ptr != other.m_ptr; }
    bool operator<(const const_reverse_iterator& other) const { return m_ptr > other.m_ptr; }
    bool operator<=(const const_reverse_iterator& other) const { return m_ptr >= other.m_ptr; }
    bool operator>(const const_reverse_iterator& other) const { return m_ptr < other.m_ptr; }
    bool operator>=(const const_reverse_iterator& other) const { return m_ptr <= other.m_ptr; }
    
    // 复合赋值
    const_reverse_iterator& operator+=(difference_type n) { m_ptr -= n; return *this; }
    const_reverse_iterator& operator-=(difference_type n) { m_ptr += n; return *this; }
    
    // 转换为const_iterator
    const_iterator base() const { return const_iterator(m_ptr); }
    
    friend const_reverse_iterator operator+(difference_type n, const const_reverse_iterator& it) {
        return const_reverse_iterator(it.m_ptr - n);
    }
};