#include "easySkipList.h"
#include <string>

using namespace std;

int main()
{
    {
        //使用lambda
        auto cmp = [](const string& a, const string& b) {return a.length() < b.length(); };
        EasySkipList <string, int, decltype(cmp)> list(cmp);
        list.insert("aab", 1321);
        list.insert("hello", 54342);
        list.insert("world", 544);
        for (auto it = list.begin(); it != list.end(); it++) {
            cout << it->key << " " << it->value << endl;
        }
    }

    cout << "==================================" << endl;
    
    {
        //使用仿函数
        struct cmp {
            bool operator()(int a, int b) {
                return a > b;
            }
        };

        EasySkipList < int, int, cmp> list{};

        for (int i = 1; i <= 10; i++) list.insert(rand()%20, rand());
        
        for (auto it = list.find(10); it != list.end(); it++) {
            cout << it->key << " " << it->value << endl;
        }
    }

    cout << "==================================" << endl;

    {
        //默认小于号
        EasySkipList<int, int>list;
        list.insert(1, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        list.insert(5, 3);
        list.insert(1, 3);
        list.insert(4, 3);
        for (auto it = list.begin(); it != list.end();it++) {
            cout << it->key << " " << it->value << endl;
        }
       
    }

    // {
    //     //可以添加 T && 实现move语义
    //     //添加重载 []
    // }
    
}
