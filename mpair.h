#ifndef MPAIR_H
#define MPAIR_H
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;
template <typename K,typename V>

struct MPair{
    K key;
    vector<V> value_list;
    bool debug  = false;

    //--------------------------------------------------------------------------------
    /*  MPair CTORs:
     *  must have these CTORs:
     *  - default CTOR / CTOR with a key and no value: this will create an empty vector
     *  - CTOR with a key AND a value: pushes the value into the value_list
     *  _ CTOR with a key and a vector of values (to replace value_list)
     */
    //--------------------------------------------------------------------------------
    MPair(const K& k=K()){
        key = k;
    }
    MPair(const K& k, const V& v){
        key = k;
        value_list.push_back(v);
    }
    MPair(const K& k, const vector<V>& vlist){
        key = k;
        value_list.clear();
        for(unsigned int i = 0 ; i < vlist.size() ; i++ )
            value_list.push_back(vlist[i]);
    }
    //--------------------------------------------------------------------------------

    friend ostream& operator <<(ostream& outs, const MPair<K, V>& print_me){
        const bool too_much = false;
        if (too_much) outs<<"key: ";
        outs<<print_me.key<<": ";
        if (too_much) outs<<endl;
        for(unsigned int i = 0; i < print_me.value_list.size() ; i++){
            if (too_much) outs<<"value["<<i<<"]: ";
            outs<<"{"<<print_me.value_list[i]<<"}-->";
        }
        outs<<"|||";
        return outs;
    }
    friend bool operator ==(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key == rhs.key);
    }
    friend bool operator !=(const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key != rhs.key);
    }
    friend bool operator < (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key < rhs.key);
    }
    friend bool operator > (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key > rhs.key);
    }
    friend bool operator <= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key <= rhs.key);
    }
    friend bool operator >= (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        return (lhs.key >= rhs.key);
    }
    friend MPair<K, V> operator + (const MPair<K, V>& lhs, const MPair<K, V>& rhs){
        assert(lhs.key == rhs.key);
        MPair<K,V> sum(lhs.key,lhs.value_list);
        for(int i = 0 ; i< rhs.value_list.size() ; i++)
            sum.value_list.push_back(rhs.value_list[i]);
        return sum;
    }

        MPair<K, V>& operator += (const MPair<K, V>& rhs){
            assert (rhs.value_list.size() <=1);
            key = rhs.key;
            for(unsigned int i = 0 ; i< rhs.value_list.size() ; i++)
            {
                value_list.push_back(rhs.value_list[i]);
            }
            return *this;
        }

//    friend  MPair<K, V>& operator += (const MPair<K,V> & lhs , const MPair<K, V>& rhs){
//        assert (rhs.value_list.size() <=1);
//        MPair<K,V> temp(lhs.key , lhs.value_list);
//        temp.value_list.push_back(rhs.value_list.at(0));
//        return temp;
//    }
};
//--------------------------------------------------------------------------------
#endif // MPAIR_H
