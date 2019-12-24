#ifndef MMAP_H
#define MMAP_H
#include <iostream>
#include <vector>
#include "bplustree.h"
#include "mpair.h"
using namespace std;

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V> > map_base;
    class Iterator{
    public:
        friend class MMap;
        Iterator(typename map_base::Iterator it){ _it = it;}
        Iterator operator ++(int unused){
            //return _it++;
            Iterator hold(_it);
            _it++;
            return hold;
        }
        Iterator operator ++(){
            return ++_it;
        }
        MPair<K, V> operator *(){
            return MPair<K,V>((*_it).key,(*_it).value_list);
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return ( lhs._it == rhs._it ) ;
        }
        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (lhs._it != rhs._it);
        }
        
    private:
        typename map_base::Iterator _it;
    };
    
    MMap();
    
    //  Iterators
    Iterator begin();
    Iterator end();
    
    //  Capacity
    int size() const;
    bool empty() const;
    
    //  Element Access
    const vector<V>& operator[](const K& key) const;
    vector<V>& operator[](const K& key);
    
    //  Modifiers
    void insert(const K& k, const V& v);
    void erase(const K& key);
    void clear();
    void print_iterator();
    
    //  Operations:
    bool contains(const K& key) const ;
    vector<V> &get(const K& key);
    
    Iterator find(const K& key);
    int count(const K& key);
    // I have not writtent hese yet, but we will need them:
    //    int count(const K& key);
    Iterator upper_bound(const K& key);
    Iterator lower_bound(const K& key);
    //    lower_bound
    //    upper_bound
    //    equal_range:
    
    
    
    /**/ bool is_valid(){return mmap.is_valid();}
    
    friend ostream& operator<<(ostream& outs, const MMap<K, V>& print_me){
        outs<<print_me.mmap<<endl;
        return outs;
    }
    
private:
    BPlusTree<MPair<K, V> > mmap;
};

//-----------------------------------------------------------
//-------- M U L T I  M A P  F U N C T I O N S -----
//-----------------------------------------------------------
template <typename K, typename V>
MMap<K,V>::MMap():mmap(true){
    // cout<<"MULTI MAP CONSTRUCTOR FIRED....."<<endl;
    //mmap = BPlusTree<MPair<K,V> >(true);
}
//  Iterators
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::begin(){
    return Iterator(mmap.begin());
}
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::end(){
    return Iterator(mmap.end());
}
//  Capacity
template <typename K, typename V>
int MMap<K,V>::size() const{
    cout<<"MMAP<K,V> = "<<mmap.size()<<endl;
    return mmap.size();
}
template <typename K, typename V>
bool MMap<K,V>::empty() const{
    return mmap.empty();
}
//  Element Access
template <typename K, typename V>
const vector<V>& MMap<K,V>::operator[](const K& key) const{
    MPair <K,V > p(key);
    return mmap.get(p).value_list;
}
template <typename K, typename V>
vector<V>& MMap<K,V>::operator[](const K& key){
    MPair <K,V> p(key);
    return mmap.get(p).value_list;
}
//  Modifiers
template <typename K, typename V>
void MMap<K,V>::insert(const K& k, const V& v){
    MPair<K,V> inserter(k,v);
    mmap.insert(inserter);
}
template <typename K, typename V>
void MMap<K,V>::erase(const K& key){
    MPair<K,V> remover(key);
    mmap.remove(remover);
}
template <typename K, typename V>
void MMap<K,V>::clear(){
    mmap.clear_tree();
}
//  Operations:
template <typename K, typename V>
bool MMap<K,V>::contains(const K& key) const {
    MPair<K,V> container(key);
    return mmap.contains(container);
}
template <typename K, typename V>
vector<V> & MMap<K,V>::get(const K& key){
    MPair<K,V > getter(key);
    return mmap.get(getter).value_list;
}
template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::find(const K& key){
    MPair<K,V> finder(key);
    return mmap.find(finder,true);
}
template <typename K, typename V>
int MMap<K,V>::count(const K& key){
    cout<<"MMAP<K,V> at key:"<<key<<" vector size"<<mmap.value_list.size()<<endl;
    MPair<K,V> p(key);
    return mmap.get(p).value_list.size();
}
template <typename K, typename V>
void MMap<K,V>::print_iterator(){
    int i = 0;
    for(Iterator it = begin(); it != end() ; it++)
    {
        cout<<"iterator index in MMAP: "<<i<<endl;
        cout<<*it<<endl;
        i++;
    }
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::lower_bound(const K& key){
    //    bool findme = false;
    //    return mmap.find(key , findme);
    for (Iterator it = begin() ; it != end() ; it++)
    {
      //  cout<<"iterator in lower bound is |"<<*it<<"| "<<endl;
        //cout<<"our key is: "<<key<<endl<<"key in for loop is:"<<(*it).key<<endl;
        if ( key <= (*it).key )
            return it;
    }
    //cout<<"broke out of the loop in lower bound, returning null..."<<endl;
    return Iterator(NULL);
}

template <typename K, typename V>
typename MMap<K,V>::Iterator MMap<K,V>::upper_bound(const K& key){
    //bool findme = false;
    // Iterator it  = mmap.find(key,findme);
    //    if(findme)
    //    {
    //        cout<<"key: "<<key<<" found in tree"<<endl;
    //  return  it++;
    //    }
    //    cout<<"didn't find: "<<key<<"in tree"<<endl;
    //    return it;
    for (Iterator it = begin() ; it != end() ; it++)
    {
       // cout<<"our key is: "<<key<<endl<<"key in for loop is:"<<(*it).key<<endl;
        //cout<<"iterator in upper bound is |"<<*it<<"| "<<endl;
        if ( key < (*it).key )
            return it;
    }
    cout<<"broke out of the loop in upper bound, returning null..."<<endl;
    return Iterator(NULL);
}

// I have not writtent hese yet, but we will need them:
//    int count(const K& key);
//    lower_bound
//    upper_bound
//    equal_range:

#endif // MMAP_H
