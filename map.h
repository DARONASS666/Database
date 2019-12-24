#ifndef MAP_H
#define MAP_H
#include <iostream>
#include "bplustree.h"
#include "pair.h"
using namespace std;

template <typename K, typename V>
class Map
{
public:
    typedef BPlusTree<Pair<K, V> > map_base;
    class Iterator{
    public:
        friend class Map;
        Iterator(typename map_base::Iterator it){
            _it = it;
        }
        Iterator operator ++(int unused){
            Iterator hold(_it);
            ++_it;
            return hold;
        }
        Iterator operator ++(){
            return ++_it;
        }
        Pair<K, V> operator *(){
            return Pair<K,V>((*_it).key,(*_it).value);
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return ( lhs._it == rhs._it );
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return ( lhs._it != rhs._it );
        }
        
    private:
        typename map_base::Iterator _it;
        
    };
    
    Map();
    //  Iterators
    Iterator begin();
    Iterator end();
    void print_iterator();
    
    //  Capacity
    int size();
    bool empty() const;
    
    //  Element Access
    V& operator[](const K& key);
    V& at(const K& key);
    const V& at(const K& key) const;
    
    //  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    V get(const K& key);
    
    //  Operations:
    Iterator find(const K& key);
    bool contains(const Pair<K, V>& target) const;
    bool contains(const Pair<K, V>& target);
    
    bool contains(const K& key) const;
    bool contains(const K& key);
    // I have not written these yet, but we will need them:
    Iterator upper_bound(const K& key);
    Iterator lower_bound(const K& key);
    //    int count(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    
    /**/ bool is_valid(){return map.is_valid();}
    
    /**/   friend ostream& operator<<(ostream& outs, const Map<K, V>& print_me){
        outs<<print_me.map<<endl;
        return outs;
    }
private:
    int key_count;
    BPlusTree<Pair<K, V> > map;
};


//-------------------------------------------------------
//----------- M A P  F U N C T I  O N S -----------
//-------------------------------------------------------
template <typename K, typename V>
Map<K,V>::Map(){
    key_count = 0;
    map = BPlusTree<Pair<K,V> >(false);
}
//  Iterators
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::begin(){
    return map.begin();
}
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::end(){
    return map.end();
}
//  Capacity
template <typename K, typename V>
int Map<K,V>::size(){
    //cout<<"MAP<K,V> = "<<map.size()<<endl;
    return map.size();
}

template <typename K, typename V>
bool Map<K,V>::empty() const{
    return map.empty();
}
//  Element Access
template <typename K, typename V>
V& Map<K,V>:: operator[](const K& key){
    return map.get(Pair<K,V>(key)).value;
}
template <typename K, typename V>
V& Map<K,V>::at(const K& key){
    return map.get(Pair<K,V>(key)).value;
}
template <typename K, typename V>
const V& Map<K,V>::at(const K& key) const{
    return map.get(Pair<K,V>(key)).value;
}
//  Modifiers
template <typename K, typename V>
void Map<K,V>::insert(const K& k, const V& v){
    Pair<K,V> inserter(k,v);
    map.insert(inserter);
    key_count++;
}
template <typename K, typename V>
void Map<K,V>::erase(const K& key){
    Pair<K,V> eraser(key,V());
    map.remove(eraser);
    key_count--;
}
template <typename K, typename V>
void Map<K,V>::clear(){
    map.clear_tree();
    key_count = 0;
}
template <typename K, typename V>
V Map<K,V>::get(const K& key){
    Pair<K,V> getter(key);
    return map.get(getter).value;
}
//  Operations:
template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::find(const K& key){
    Pair<K,V> finder(key);
    return Iterator(map.find(finder,true));
}
template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) const{
    return map.contains(target);
}
template <typename K, typename V>
bool Map<K,V>::contains(const Pair<K, V>& target) {
    return map.contains(target);
}
template <typename K, typename V>
bool Map<K,V>::contains(const K& key) const{
    return map.contains(key);
}
template <typename K, typename V>
bool Map<K,V>::contains(const K& key){
    return map.contains(key);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::lower_bound(const K& key){
    for (Iterator it = begin() ; it != end() ; it++)
    {
        cout<<"iterator in lower bound is |"<<*it<<"| "<<endl;
        if ( key <= (*it).key )
            return it;
    }
    cout<<"broke out of the loop in lower bound, returning null..."<<endl;
    return Iterator(NULL);
}

template <typename K, typename V>
typename Map<K,V>::Iterator Map<K,V>::upper_bound(const K& key){
    for (Iterator it = begin() ; it != end() ; it++)
    {
        cout<<"iterator in upper bound is |"<<*it<<"| "<<endl;
        if ( key < (*it).key ) return it;
    }
    cout<<"broke out of the loop in lower bound, returning null..."<<endl;
    return Iterator(NULL);
}
template <typename K, typename V>
void Map<K,V>::print_iterator(){
    for(Iterator it = begin() ; it != end() ; it++)
        cout<<*it<<endl;
}
#endif // MAP_H
