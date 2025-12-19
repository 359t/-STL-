#include <iostream>
#include "vector.hpp"

using namespace std;
void test01()
{
    vector<int> v;
    vector<int> v1(10,9);
    vector<int> v2(v1);
    cout<<v2.size()<<endl;
    cout<<v2.capacity()<<endl;
    cout<<v2[0]<<endl;
    v = v2;
    cout<<"v容器中的第一个参数是："<<v[1]<<endl;
}
void test02()
{
    vector<int> v;
    if(v.empty())
    {
        cout<<"容器为空"<<endl;
    }
    v.resize(100);
    cout << "修改后容器的大小为："<<v.size()<<endl;
    cout <<"修改后容器的容量为："<<v.capacity()<<endl;

}
void test03()
{
 
}

int main()
{;
    
    test02();
    return 0;
}