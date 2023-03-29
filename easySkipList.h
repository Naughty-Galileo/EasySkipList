#include <iostream>
#include <string>
#include <time.h>
#include <limits.h>

template<typename T>
struct Less {
    bool operator () (const T & a , const T & b) const {
        return a < b;
    }
};


template<typename K, typename V>
class Node {
public:
    Node() : key{0}, value{0}, level{0}, next{0} { }
    Node(K ky, V val, int le, Node* nxt = nullptr) : key(ky), value(val), level(le) { 
        next = new Node* [level+1];
        for (int i=0; i<= level; i++) next[i] = nxt;
    }
    ~Node() {
        delete[] next;
    }
    K get_key() {
        return key;
    }
    V get_value() {
        return value;
    }
public:
    Node<K,V>** next;
    const K key;
    V value;
    int level;
};

template<typename K, typename V, typename Comp = Less<K>>
class EasySkipList {
private:
    void init() {
        srand((uint32_t)time(NULL));
        level = length = 0;
        head->next = new Node<K,V> * [MAXL + 1];
        for (int i = 0; i <= MAXL; i++)
            head->next[i] = tail;
    }
    int randomLevel() {
        int lv = 1; while ((rand() & S) < PS) ++lv;
        return MAXL > lv ? lv : MAXL;
    }

    Node<K,V>* find(const K& key, Node<K,V>** update) {
        Node<K,V>* p = head;
        for (int i = level; i >= 0; i--) {
            while (p->next[i] != tail && less(p->next[i]->key, key)) {
                p = p->next[i];
            }
            update[i] = p;
        }
        p = p->next[0];
        return p;
    }

public:
    struct Iter {
        Node<K,V>* p;
        Iter() : p(nullptr) {};
        Iter(Node<K,V>* rhs) : p(rhs) {}
        Node<K,V>* operator ->()const { return (p);}
        Node<K,V>& operator *() const { return *p;}
        bool operator == (const Iter& rhs) { return rhs.p == p;}
        bool operator != (const Iter& rhs) { return !(rhs.p == p); }
        void operator ++() { p = p->next[0]; }
        void operator ++(int) { p = p->next[0]; }
    };

public:
    EasySkipList() : head(new Node<K,V>()), tail(new Node<K,V>()), less{Comp()} {
        init();    
    }
    EasySkipList(Comp _less) : head(new Node<K,V>()), tail(new Node<K,V>()),  less{_less} {
        init();
    }

    void insert(const K& key, const V& value) {
        Node<K,V> * update[MAXL + 1];
        Node<K,V>* p = find(key,update);
        
        if (p->key == key) {
            p->value = value;
            return;
        }

        int lv = randomLevel();
        if (lv > level) {
            lv = ++level;
            update[lv] = head;
        }
        Node<K,V>* newNode = new Node<K,V>(key, value, lv);
        for (int i = lv; i >= 0; --i) {
            p = update[i];
            newNode->next[i] = p->next[i];
            p->next[i] = newNode;
        }
        ++length;
    }

    bool erase(const K& key) {
        Node<K,V>* update[MAXL + 1];
        Node<K,V>* p = find(key, update);
        if (p->key != key) return false;

        for (int i = 0; i <= p->level; ++i) {
            update[i]->next[i] = p->next[i];
        }
        delete p;
        while (level > 0 && head->next[level] == tail) --level;
        --length;

        return true;
    }

    Iter find(const K&key) {
        Node<K,V>* update[MAXL + 1];
        Node<K,V>* p = find(key, update);

        if (p == tail) return tail;
        if (p->key != key) return tail;
        return Iter(p);
    }
    bool count(const K& key) {
        Node<K,V>* update[MAXL + 1];
        Node<K,V>* p = find(key, update);

        if (p == tail)return false;

        return key == p->key;
    }
    Iter end() {
        return Iter(tail);
    }   
    Iter begin() {
        return Iter(head->next[0]);
    }

private:
    int level;
    int length;
    static const int MAXL = 32;
    static const int P = 4;
    static const int S = 0xFFFF;
    static const int PS = S / P;
    static const int INVALID = INT_MAX;
    Node<K,V>* head, * tail;
    Comp less;
};
