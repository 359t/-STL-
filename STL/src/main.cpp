#include <iostream>
#include "vector.hpp"
#include "deque.hpp"

using namespace std;
void vector_Test()
{
    vector<int> v;
    vector<int> v1(10,9);
    vector<int> v2(v1);
    cout<<"容器v2的大小为"<<v2.size()<<endl;
    cout<<"容器v2的容量为"<<v2.capacity()<<endl;
    cout<<"容器v2的第一个参数为"<<v2[0]<<endl;
    v = v2;
    cout<<"v容器中的第一个参数是："<<v[1]<<endl;
    
    if(v.empty())
    {
        cout<<"容器为空"<<endl;
    }
    v.resize(100);
    cout << "修改后容器的大小为："<<v.size()<<endl;
    cout <<"修改后容器的容量为："<<v.capacity()<<endl;
}
void deque_Test()
{
    deque<int> d1;
    d1.assign(10,100);
    cout<< "d1中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d1.begin(); it < d1.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    deque<int> d2(d1);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d2中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d2.begin(); it < d2.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d2.push_back(10);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "尾部增加元素后d2中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d2.begin(); it < d2.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d2.push_front(11);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "头部增加元素后d2中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d2.begin(); it < d2.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d2.pop_back();
    cout<< "-------------------------------------------"<<endl; 
    cout<< "尾部删除元素后d2中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d2.begin(); it < d2.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d2.pop_front();
    cout<< "-------------------------------------------"<<endl; 
    cout<< "头部删除元素后d2中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d2.begin(); it < d2.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
    
    deque<int> d3;
    d3 = d2;
    cout<< "-------------------------------------------"<<endl; 
    cout<< "头部删除元素后d3中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d3.begin(); it < d3.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    deque<int> d4;
    d4 = {1,2,3};
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d4中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d4.begin(); it < d4.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d4.clear();
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d4中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d4.begin(); it < d4.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d4.assign(10,20);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d4中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d4.begin(); it < d4.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;
   
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d4的大小为："<<d4.size()<<endl;
    d4.empty();
    cout<< "d4.empty()后的大小为："<<d4.size()<<endl; 
    d4.resize(100,10);
    cout<<"d4.resize()后的大小为："<< d4.size()<<endl;
    cout<<endl;

    d3.insert(d3.begin()+1,99);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "insert后d3中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d3.begin(); it < d3.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d3.erase(d3.begin()+1);
    cout<< "-------------------------------------------"<<endl; 
    cout<< "erase后d3中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d3.begin(); it < d3.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    d3.clear();
    cout<< "-------------------------------------------"<<endl; 
    cout<< "clear后d3中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d3.begin(); it < d3.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    deque<int> d7(10,10);
    for( int i = 0; i < 10;i++){
        d7[i] = 100+i;
    }
    cout<< "-------------------------------------------"<<endl; 
    cout<< "d7中包含的元素为："<<endl; 
    for( deque<int>::iterator it = d7.begin(); it < d7.end(); it++)
    {
        cout<<*it<<" ";
    }
    cout<<endl;

    cout<< "-------------------------------------------"<<endl; 
    cout << " d7中第一个元素为"<<d7.at(0)<<endl;
    cout << " d7中第front个元素为"<<d7.front()<<endl;
    cout << " d7中第back个元素为"<<d7.back()<<endl;
    cout<<endl;



}

void test03()
{
 
}

int main()
{;
    //vector_Test();
    deque_Test();
    return 0;
}