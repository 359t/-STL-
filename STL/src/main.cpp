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