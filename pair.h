#ifndef PAIR_H
#define PAIR_H
#include <cassert>
#include <iostream>
using namespace std;

template <typename K,typename V>
struct Pair{
    K key;
    V value;
    bool debug = false;
    Pair(const K& k=K(), const V& v=V()):key(k) , value(v){}
    friend ostream& operator <<(ostream& outs, const Pair<K, V>& print_me){
        if(print_me.debug){
            return outs <<"key is :"<<print_me.key<<" , value is :"<<print_me.value;
        }
        else{
            return outs << "["<<print_me.key<<":"<<print_me.value<<"]";
        }
    }
    friend bool operator ==(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key != rhs.key);
    }
    friend bool operator < (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key < rhs.key);
    }
    friend bool operator > (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key > rhs.key);
    }
    friend bool operator <= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        return (lhs.key >= rhs.key);
    }
    friend Pair<K, V> operator + (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        assert(lhs.key == rhs.key);
        return Pair<K, V>(lhs.key, lhs.value+rhs.value);
    }
    friend Pair<K, V> operator += (const Pair<K, V>& lhs, const Pair<K, V>& rhs){
        cout<<"pair += operator fired.."<<endl;
    }

};
#endif // PAIR_H
