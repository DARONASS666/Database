#ifndef BPLUSTREE_H
#define BPLUSTREE_H
#include <windows.h>
#include "lower_level_bplustree_functions.h"
#include <iostream>
#include <cassert>
#include <iomanip>
using namespace std;

template <class T>
class BPlusTree
{
public:
    class Iterator{
    public:
        friend class BPlusTree;
        Iterator(BPlusTree<T>* _it=NULL, int _key_ptr = 0):it(_it), key_ptr(_key_ptr){}

        T operator *(){
            if(false){
                cout<<"KEYPTR: "<<key_ptr<<endl;
                cout<<"DATACOUNT: "<<it->data_count<<endl;
                cout<<"ITERATOR VALUE: "<<it->data[key_ptr]<<endl;
                assert(key_ptr < it->data_count);
            }
            //not sure if this is correct
            return it->data[ key_ptr ];
        }
        Iterator operator++(int un_used){
            Iterator hold(it,key_ptr);
            if(key_ptr == it->data_count - 1)
            {
                key_ptr = 0;
                it = it->next;
            }
            else
                key_ptr++;
            return hold;
        }
        Iterator operator++(){
            if(key_ptr == it->data_count - 1)
            {
                key_ptr = 0;
                it = it->next;
            }
            else
                key_ptr++;
            return *this;
        }
        friend bool operator ==(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr );
        }

        friend bool operator !=(const Iterator& lhs, const Iterator& rhs){
            return (lhs.it != rhs.it || lhs.key_ptr != rhs.key_ptr );
        }

        void print_Iterator(){
            assert(key_ptr < it->data_count);
            cout << it->data[ key_ptr ] << " ";
        }

        bool is_null(){return !it;}
    private:
        BPlusTree<T>* it;
        int key_ptr;
        int iterator_size = 0;
    };

    BPlusTree(bool dups = false);
    //big three:
    BPlusTree(const BPlusTree<T>& other);
    ~BPlusTree();
    BPlusTree<T>& operator =(const BPlusTree<T>& RHS);

    void insert(const T& entry);                //insert entry into the tree
    void remove(const T& entry);                //remove entry from the tree

    /***/ void clear_tree();                          //clear this object (delete all nodes etc.)
    void copy_tree(const BPlusTree<T>& other);      //copy other into this object
    void copy_tree(const BPlusTree<T>& other, vector<BPlusTree<T> *> &vekter);


    /***/ bool contains(const T& entry) const;              //true if entry can be found in the array
    const T& get(const T& entry)const;          //return a reference to entry in the tree
    /***/ T& get(const T& entry);                     //return a reference to entry in the tree
    T& get_existing(const T& entry);
    /***/  Iterator find(const T& entry, bool & confirmed){    //return an iterator to this key. NULL if not there.
        int i = first_ge(data, data_count, entry);
        bool found = ( i < data_count && data[i] == entry);

        if(found){
            if(is_leaf()){
                confirmed = true;
                return Iterator(this,i);
            }
            else
                return subset [ i + 1]->find(entry , confirmed);
        }
        else{
            if(is_leaf())
            {
                return Iterator(NULL);
            }
            else
                return subset [ i ]->find(entry, confirmed);
        }
    }
    int size();
    /***/  bool empty() const;                         //true if the tree is empty

    /***/ void print_tree(int level = 0, ostream &outs=cout) const; //print a readable version of the tree
    /***/ friend ostream& operator<<(ostream& outs,const BPlusTree<T>& print_me){
        print_me.print_tree(0, outs);
        return outs;
    }

    //added functions
    bool is_valid();
    void print_linked_list(){
        BPlusTree<T>* temp = get_smallest_node();
        while(temp){ cout<<"{";
            print_array(temp->data,temp->data_count);
            cout<<"}->";
            temp = temp->next;
        }
        cout<<"|||";
    }

    /***/ Iterator begin();
    /***/ Iterator end();

private:
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    bool DEBUG = false;
    bool dups_ok;                       //true if duplicate keys may be inserted
    int data_count;                     //number of data elements
    T data[MAXIMUM + 1];                //holds the keys
    int child_count;                    //number of children
    BPlusTree* subset[MAXIMUM + 2];     //subtrees
    BPlusTree* next;
    /***/ bool is_leaf() const
    {return child_count == 0;}        //true if this is a leaf node
    int tree_size;
    /***/ T* find_ptr(const T& entry);        //return a pointer to this key. NULL if not there.

    //insert element functions
    /***/ void loose_insert(const T& entry);  //allows MAXIMUM+1 data elements in the root
    /*need to fix pointers*/  /*void*/ void fix_excess(int i);             //fix excess of data elements in child i

    //remove element functions:
    /***/   void loose_remove(const T& entry);  //allows MINIMUM-1 data elements in the root

    /***/  BPlusTree<T>* fix_shortage(int i);  //fix shortage of data elements in child i
    // and return the smallest key in this subtree
    /***/  BPlusTree<T>* get_smallest_node();
    /***/  BPlusTree<T>* get_largest_node();
    /***/ void get_smallest(T& entry);        //entry := leftmost leaf
    /***/ void get_biggest(T& entry);         //entry := rightmost leaf
    /***/ void remove_biggest(T& entry);      //remove the biggest child of this tree->entry
    /***/ void transfer_left(int i);          //transfer one element LEFT from child i
    /***/ void transfer_right(int i);         //transfer one element RIGHT from child i
    /***/ void rotate_left(int i);
    /***/ void rotate_right(int i);
    /***/ void merge_with_next_subset(int i);    //merge subset i with subset i+1

};
//---------------------------------------------------------------------
//            C T O R  /  B I G   T H R E E
//---------------------------------------------------------------------
template <typename T>
BPlusTree<T>::BPlusTree(bool dups):dups_ok(dups), data_count(0),
    child_count(0), next(NULL) , tree_size(0){
    //left emtpy
    //cout<<"B PLUS TREE CONSTRUCTOR FIRED....."<<endl;
    //cout<<"dups: "<<dups_ok<<endl;
    if(DEBUG)  cout<<endl<<"** default contructor fired!"<<endl;
}
template <typename T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& other):data_count(0), child_count(0) , next(NULL){
    if (DEBUG) cout<<"** copy constructor fired!"<<endl;
    //this->clear_tree();
    this->copy_tree(other);
    this->dups_ok = other.dups_ok;
}
template <typename T>
BPlusTree<T>::~BPlusTree(){
    if (DEBUG) cout<<"** destructor fired!"<<endl;
    this->clear_tree();
}
template <typename T>
BPlusTree<T>& BPlusTree<T>::operator =(const BPlusTree<T>& RHS){
    if (DEBUG) cout<<"** assignment operator fired!"<<endl;
    if(this != & RHS){
        this->clear_tree();
        this->dups_ok = RHS.dups_ok;
        this->copy_tree(RHS);
    }
    return *this;
}
//---------------------------------------------------------------------
//            B E G I N ( )   &   E N D ( )
//---------------------------------------------------------------------
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::begin(){
    if(DEBUG)  cout<<endl<<"------------- Iterator Begin -------------"<<endl;

    if(!empty())
        return Iterator(get_smallest_node());
    else
        return Iterator(NULL);
}
template <typename T>
typename BPlusTree<T>::Iterator BPlusTree<T>::end(){
    if(DEBUG)  cout<<endl<<"------------- Iterator End -------------"<<endl;
    return Iterator(NULL);
}
//---------------------------------------------------------------------
//            C O N T A I N S / F I N D / G E T / E T C .
//---------------------------------------------------------------------
template <typename T>
const T& BPlusTree<T>::get(const T& entry)const{          //return a reference to entry in the tree
    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    else{
        if (found) //inner node
            return subset[i+1]->get(entry);
        //or just return true?
        else //not found yet...
            return subset[i]->get(entry);
    }
}
template <typename T>
T& BPlusTree<T>::get(const T& entry) {
    //If entry is not in the tree, add it to the tree
    //assert(contains(entry));

    if (!contains(entry))
        insert(entry);
    return get_existing(entry);
}
template <typename T>
bool BPlusTree<T>::contains(const T& entry) const
{              //true if entry can be found in the array
    if(DEBUG)  cout<<endl<<"------------- Contains -------------"<<endl;

    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);

    if( is_leaf() )
        return ( found );
    else
    {
        if ( found ) return  (subset[ i + 1 ]->contains(entry)) ;
        else
            return (subset[ i ]->contains(entry));
    }
}
template <typename T>
T& BPlusTree<T>::get_existing(const T& entry){
    const bool debug = false;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i]==entry);
    if (is_leaf())
        if (found){
            return data[i];
        }
        else{
            if (debug) cout<<"get_existing was called with nonexistent entry"<<endl;
            assert(found);
        }
    else{
        if (found) //inner node
            return subset[i+1]->get(entry);
        //or just return true?
        else //not found yet...
            return subset[i]->get(entry);
    }
}
//---------------------------------------
//     V A L I D A T I O N
//---------------------------------------

template <typename T>
bool BPlusTree<T>::is_valid(){
    if(DEBUG)  cout<<endl<<"------------- Is Valid -------------"<<endl;
    if(is_leaf()){
        if(!verify_sorted_array(this->data,this->data_count)){
            cout<<"array on leaf level not sorted"<<endl;
            return false;
        }
        if(next && next->data[0] < data[data_count  - 1]){
            cout<<"iterator not sorted"<<endl<<"next data[0]: "<<next->data[0]<<
                  endl<<"data[data_count-1]"<<data[data_count-1]<<endl;
            return false;
        }
    }
    else{
        for(int i = 0 ; i < data_count ; i++)
        {
            if(!subset[i]->is_valid()){
                cout<<"subset[i]->is_valid() was not validated"<<endl;
                return false;
            }
            T oldest_child;
            subset[ i ]->get_biggest(oldest_child);
            if(data[i] < oldest_child){
                cout<<"data i :"<<data[i]<<endl<<"oldest child: "<<oldest_child<<endl;
                return false;
            }
            T first_nephew;
            subset[i+1]->get_smallest(first_nephew);
            if( data[i] != first_nephew ){
                cout<<"data i :"<<data[i]<<endl<<"first nephew: "<<first_nephew<<endl;
                return false;
            }
            if(!verify_sorted_array(this->subset[i]->data,this->subset[i]->data_count)) return false;
        }
    }
    return (verify_sorted_array(this->data,this->data_count));
}

template <typename T>
T* BPlusTree<T>::find_ptr(const T& entry){        //return a pointer to this key. NULL if not there.
    if(DEBUG)  cout<<endl<<"------------- Find Ptr -------------"<<endl;

    int i = first_ge(data, data_count, entry);
    bool found = ( i < data_count && data[i] == entry);

    if(found){
        if(is_leaf())
            return & data [ i ];
        else
            return subset [ i + 1 ]->find_ptr(entry);
    }
    else{
        if(is_leaf())
            return NULL;
        else
            return subset [ i ]->find_ptr(entry);
    }
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_smallest_node(){
    if(DEBUG) cout<<endl<<"----------- Get Smallest Node -----------"<<endl;

    if(is_leaf())
        return  this;
    else{
        return subset[ 0 ]->get_smallest_node();
    }
}
template <typename T>
BPlusTree<T>* BPlusTree<T>::get_largest_node(){
    if(DEBUG) cout<<endl<<"----------- Get Largest Node -----------"<<endl;

    if(is_leaf())
        return  this;
    else{
        return subset[ child_count - 1 ]->get_largest_node();
    }
}
template <typename T>
void BPlusTree<T>::get_smallest(T& entry){
    if(DEBUG) cout<<endl<<"------------- Get Smallest -------------"<<endl;

    if(is_leaf())
    {
        entry  = data[ 0 ] ;
        if (DEBUG)  cout<<"ENTRY IN GET SMALLEST: "<<entry<<endl;
    }
    else{
        subset[ 0 ]->get_smallest(entry);
    }
    return;
}

template <typename T>
void BPlusTree<T>::get_biggest(T& entry){
    if(DEBUG) cout<<endl<<"------------- Get Biggest -------------"<<endl;

    if(is_leaf())
        entry  = data[ data_count - 1 ] ;
    else{
        subset[child_count - 1 ]->get_biggest(entry);
    }
}

//------------------------------------------------
//   R E M O V E   F U N C T I O N S
//------------------------------------------------
template <typename T>
void BPlusTree<T>::remove(const T& entry){
    if(DEBUG)  cout<<endl<<"---------------- Remove ----------------"<<endl;

    loose_remove(entry);
    if(child_count == 1 && data_count == 0){
        BPlusTree<T>* shrink_ptr = subset[ 0 ];

        copy_array(this->data,this->data_count,shrink_ptr->data,shrink_ptr->data_count);
        copy_array(this->subset,this->child_count,shrink_ptr->subset,shrink_ptr->child_count);

        shrink_ptr->data_count = 0;
        shrink_ptr->child_count = 0;
        delete shrink_ptr;
        shrink_ptr = NULL;
    }
    if(DEBUG)  cout<<endl<<"-------------Passed Remove -------------"<<endl;
}
template <typename T>
void BPlusTree<T>::loose_remove(const T& entry){
    if(DEBUG)  cout<<endl<<"------------- Loose Remove -------------"<<endl;

    int i = first_ge(data, data_count, entry);
    bool found = ( i < data_count && data[i] == entry);

    T found_entry;
    if (is_leaf()){
        if(found){
            //decrement size
            BPlusTree<T>::Iterator it;
            it.iterator_size--;
            delete_item(data, i , data_count, found_entry);
            if(DEBUG)  cout<<endl<<"---------- Passsed Loose Remove A LEAF AND FOUND ----------"<<endl;
        }
    }
    else{
        if ( !found ){
            /*return */ subset[ i ]->loose_remove(entry);
            if(subset[ i ]->data_count < MINIMUM)
            {
                if (DEBUG) cout<<"shortage found in subset(i) .. in loose remove"<<endl;
                fix_shortage( i );
            }
        }
        else{
            assert(i < child_count - 1 );

            subset[ i + 1 ]->loose_remove( entry );

            if(subset[ i + 1 ]->data_count < MINIMUM)  fix_shortage ( i + 1 );
            if (DEBUG){
                cout<<"there is a shortage on subset (i+1) .. in loose remove"<<endl;
            }
            int j = first_ge(data,data_count,entry);
            found = (j <data_count && data[ j ] == entry);
            if (found) subset[ i + 1 ]->get_smallest(data[ j ]);
            else{
                j = first_ge(subset[ i ]->data,subset[ i ]->data_count,entry);
                found = ( j <subset[ i ]->data_count && subset[ i ]->data[ j ] == entry);
                if (found) subset[ i ]->subset [ j + 1 ]->get_smallest(subset[ i ]->data[ j ]);

                else if (i < child_count - 1){
                    j = first_ge(subset[ i + 1 ]->data,subset[ i + 1 ]->data_count,entry);
                    found = ( j <subset[ i + 1 ]->data_count && subset[ i +1]->data[ j ] == entry);
                    if (found) subset[ i + 1 ]->subset [ j + 1 ]->get_smallest(subset[ i + 1 ]->data[ j ]);
                }
            }
        }
        // --------------------------------------------------------
    }
    if(DEBUG)  cout<<endl<<"---------- Passsed Loose Remove ----------"<<endl;
}

template <typename T>
BPlusTree<T>* BPlusTree<T>::fix_shortage(int i){
    if(DEBUG) cout<<endl<<"------------- fix Shortage -------------"<<endl;
    if ((i<child_count - 1 ) && (subset[ i + 1 ]->data_count > MINIMUM)){
        if(DEBUG)    cout<<"entering transfer left (i+1)"<<endl;
        transfer_left( i + 1 );
        if(DEBUG)    cout<<"exited transfer left (i+1)"<<endl;
    }
    else if( i > 0 && subset[ i - 1 ]->data_count >MINIMUM){
        if(DEBUG)   cout<<" entering transfer right (i-1)"<<endl;
        transfer_right( i - 1 );
        if(DEBUG)    cout<<" exited transfer right (i-1)"<<endl;
    }
    else if( i + 1 < child_count ){
        if(DEBUG)    cout<<"entering merge (i)"<<endl;
        merge_with_next_subset( i );
        if(DEBUG)   cout<<"exited merge (i)"<<endl;
    }
    else if(i > 0 )
    {
        if(DEBUG)   cout<<" entering merge (i-1)"<<endl;
        merge_with_next_subset( i - 1 );
        if(DEBUG)    cout<<" exited merge (i-1)"<<endl;
    }
    if(DEBUG) cout<<endl<<"------------- Passed fix Shortage -------------"<<endl;
    return subset[ i ];
}

template <typename T>
void BPlusTree<T>::remove_biggest(T& entry){      //remove the biggest child of this tree->entry
    if(DEBUG) cout<<endl<<"------------- Remove Biggest -------------"<<endl;

    if(is_leaf())
        detach_item(data,data_count,entry);
    else{
        subset[child_count - 1]->remove_biggest(entry);
        if(subset[child_count-1]->data_count <MINIMUM)
            fix_shortage(child_count-1);
    }
    if(DEBUG) cout<<endl<<"------------- Passed Remove Biggest -------------"<<endl;
}
template <typename T>
void BPlusTree<T>::merge_with_next_subset(int i){
    if(DEBUG)  cout<<endl<<"-------- Merge With Next Subset --------"<<endl;
    if(subset[i]->is_leaf()){
        if(DEBUG) cout<<endl<<"-------- Merge a Leaf --------"<<endl;
        BPlusTree<T>* temp;
        T removed;
        if (DEBUG) cout<<"data in leaf merge: "; print_array(data,data_count); cout<<endl;
        delete_item(data,i,data_count,removed);
        if (DEBUG) cout<<"REMOVED in leaf merge: "<<removed<<endl;
        merge(subset[ i ]->data,subset[ i ]->data_count,subset[ i + 1 ]->data,subset[ i + 1 ]->data_count);
        merge(subset[i]->subset,subset[ i ]->child_count,subset[ i + 1 ]->subset,subset[ i + 1 ]->child_count);
        delete_item(subset, i + 1 ,child_count,temp);
        subset[ i ]->next = temp->next;
        delete temp;
        if(DEBUG) cout<<endl<<"-------- Passed Merge a Leaf --------"<<endl;

    }
    else{
        if(DEBUG) cout<<endl<<"-------- Merge a non-Leaf --------"<<endl;
        BPlusTree<T>* temp;
        T removed;
        delete_item(data,i,data_count,removed);
        if (DEBUG) cout<<"REMOVED in non-leaf merge: "<<removed<<endl;
        attach_item(subset[ i ]->data,subset[ i ]->data_count,removed);
        merge(subset[i]->data,subset[ i ]->data_count,subset[ i + 1 ]->data,subset[ i + 1 ]->data_count);
        merge(subset[i]->subset,subset[ i ]->child_count,subset[ i + 1 ]->subset,subset[ i + 1 ]->child_count);
        delete_item(subset,i + 1 ,child_count,temp);
        delete temp;
        if(DEBUG) cout<<endl<<"-------- Passed Merge a non-Leaf --------"<<endl;
    }
    if(DEBUG) cout<<endl<<"-------- Passed Merge Next Subset --------"<<endl;
}
template <typename T>
void BPlusTree<T>::transfer_left(int i){
    if(DEBUG)  cout<<endl<<"------------- Transfer left -------------"<<endl;
    if(subset[i]->is_leaf()){
        T deleted;
        delete_item(subset[ i ]->data, 0 ,subset[ i ]->data_count , deleted);
        attach_item(subset[ i - 1 ]->data , subset[ i - 1 ]->data_count , deleted);
        data[ i - 1 ] = subset[ i ]->data[ 0 ];
        if (DEBUG) cout<<"data [i -1] in transfer = "<<data[i-1]<<endl;
    }
    else
        rotate_left(i);
    if(DEBUG)  cout<<endl<<"-------------Passed Transfer left -------------"<<endl;
}

template <typename T>
void BPlusTree<T>::transfer_right(int i){
    if(DEBUG)  cout<<endl<<"------------- Transfer right -------------"<<endl;

    if(subset[i]->is_leaf()){
        if (DEBUG) cout<<"transfer right a leaf"<<endl;
        //        detach_item(subset[i]->data,subset[i]->data_count,subset[i+1]->data[0]);
        //        subset[i+1]->data_count++;
        insert_item(subset[i+1]->data,0 ,subset[i+1]->data_count,subset[i]->data[subset[i]->data_count-1]);
        subset[i]->data_count--;
        data[ i ] = subset[ i + 1 ]->data[0];
    }
    else
        rotate_right( i );
    if(DEBUG)  cout<<endl<<"------------- Passed Transfer right -------------"<<endl;
}

template <typename T>
void BPlusTree<T>::rotate_left(int i){
    if(DEBUG) cout<<endl<<"------------- Rotate left -------------"<<endl;
    attach_item(subset[i-1]->data,subset[i-1]->data_count,data[i-1]);
    delete_item(subset[i]->data,0,subset[i]->data_count,data[i-1]);
    if(!subset[i]->is_leaf()){
        delete_item(subset[i]->subset,0,subset[i]->child_count,subset[i-1]->subset[subset[i-1]->child_count]);
        subset[i-1]->child_count++;
    }
    if(DEBUG) cout<<endl<<"-------------Passed Rotate left -------------"<<endl;
}

template <typename T>
void BPlusTree<T>::rotate_right(int i){
    if(DEBUG) cout<<endl<<"------------- Rotate right -------------"<<endl;
    insert_item(subset[i+1]->data,0,subset[i+1]->data_count,data[i]);
    detach_item(subset[i]->data,subset[i]->data_count,data[i]);
    if(!subset[i]->is_leaf())
    {
        BPlusTree<T>* temp;
        detach_item(subset[i]->subset,subset[i]->child_count,temp);
        insert_item(subset[i+1]->subset,0,subset[i+1]->child_count,temp);
    }
    if(DEBUG) cout<<endl<<"------------- Passed Rotate right -------------"<<endl;
}
//------------------------------------------------
//    I N S E R T    F U N C T I O N S
//------------------------------------------------
template <typename T>
void BPlusTree<T>::insert(const T& entry){
    if(DEBUG)  cout<<endl<<"---------------- insert ----------------"<<endl;
    loose_insert(entry);

    if( data_count > MAXIMUM){
        BPlusTree<T>* temp = new BPlusTree<T>(dups_ok);
        copy_array(temp->data,temp->data_count,this->data,this->data_count);
        copy_array(temp->subset,temp->child_count,this->subset,this->child_count);
        data_count = 0;
        this->subset[0] = temp;
        child_count = 1;
        fix_excess(0);
    }
    if(DEBUG)  cout<<endl<<"---------------- Passed insert ----------------"<<endl;
}
template <typename T>
void BPlusTree<T>::loose_insert(const T& entry){
    //WARNIGN WARNING LOOSE INSERT SIZE SHIT MIGHT CONTAIN BUGS
    if(DEBUG)  cout<<endl<<"------------- Loose insert -------------"<<endl;
    int i = first_ge(data, data_count, entry);
    bool found = (i<data_count && data[i] == entry);
    // HANDLE hConsol = GetStdHandle(STD_OUTPUT_HANDLE);
    if(found){
        if(is_leaf()){
            if(dups_ok){
                //SetConsoleTextAttribute(hConsol,3);
                //cout<<"found and is leaf. dups_ok: |"<<entry<<"|"<<endl;
                //SetConsoleTextAttribute(hConsol,10);
                data[ i ] += entry;
            }
            else{
                // SetConsoleTextAttribute(hConsol,3);
                //cout<<"found, is leaf but not dup"<<entry<<endl;
                //SetConsoleTextAttribute(hConsol,10);
                data[ i ] = entry ;

            }
        }
        else{
            subset[ i + 1 ]->loose_insert(entry);
            if(subset[ i + 1 ]->data_count > MAXIMUM)
                fix_excess( i + 1 );
        }
    }
    else{
        if(is_leaf()){
            //increment size
            BPlusTree<T>::Iterator it;
            it.iterator_size--;
            insert_item(data,i,data_count,entry);
        }
        else{
            subset[ i ]->loose_insert(entry);
            if(subset[ i ]->data_count > MAXIMUM)
                fix_excess( i );
        }
    }
    if(DEBUG)  cout<<endl<<"------------- Passed Loose insert -------------"<<endl;
}
template <typename T>
void BPlusTree<T>::fix_excess(int i){
    if(DEBUG)  cout<<endl<<"------------- Fix Excess -------------"<<endl;
    if( subset[i]->is_leaf() ){
        insert_item(subset, i + 1 ,child_count,new BPlusTree<T>(dups_ok));
        split(subset[ i ]->data , subset[ i ]->data_count , subset[ i + 1 ]->data , subset[ i + 1 ]->data_count);

        T temp_item;
        detach_item(subset[ i ]->data,subset[ i ]->data_count,temp_item);
        insert_item(subset[ i + 1 ]->data , 0 , subset[ i + 1 ]->data_count , temp_item);

        subset[ i + 1 ]->next = subset[ i ]->next;
        subset[ i ]->next = subset[ i + 1 ] ;

        ordered_insert(data,data_count,temp_item);
    }
    else{
        insert_item(subset,i+1,child_count,new BPlusTree<T>(dups_ok));

        split(subset[ i ]->subset , subset[ i ]->child_count , subset[ i + 1 ]->subset , subset[ i + 1 ]->child_count);
        split(subset[ i ]->data , subset[ i ]->data_count , subset[ i + 1 ]->data , subset[ i + 1 ]->data_count);

        T temp_item;
        detach_item(subset[ i ]->data,subset[ i ]->data_count,temp_item);
        ordered_insert(data,data_count,temp_item);
    }
    if(DEBUG)  cout<<endl<<"------------- Passed Fix Excess -------------"<<endl;
}
//---------------------------------------------------------------
//      S I Z E  /  E M P T Y
//---------------------------------------------------------------
template <typename T>
int BPlusTree<T>::size(){
    int count = 0;
    //this function counts the number of keys in the btree
    for(BPlusTree<T>::Iterator it = begin() ; it != end() ; it++){
        count++;
    }
    cout<<"count based on iterator probing: "<<count<<endl;
    BPlusTree<T>::Iterator it;
    return  it.iterator_size;
}

template <typename T>
bool BPlusTree<T>::empty() const{
    if(DEBUG)  cout<<endl<<"------------- Empty -------------"<<endl;
    return (data_count == 0);
}
//---------------------------------------------------------------
//      C L E A R   /  C O P Y
//---------------------------------------------------------------
template <typename T>
void BPlusTree<T>::clear_tree(){
    if(DEBUG)  cout<<endl<<"------------- Clear Tree -------------"<<endl;

    if (is_leaf()){
    }
    else{
        for(int i=0 ; i < child_count; i++){
            subset[i]->clear_tree();
            delete subset[i];
        }
    }
    data_count = 0;
    child_count = 0;
    next = NULL;
    if(DEBUG)  cout<<endl<<"------------- Passed Clear Tree -------------"<<endl;
}
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other){
    if(DEBUG)  cout<<endl<<"------------- Shell Copy -------------"<<endl;
    assert(empty());
    if(DEBUG) cout<<"PASSED ASSERT IN SHELL_COPY"<<endl;
    vector<BPlusTree<T>* > vekter;
    copy_tree(other,vekter);
    this->get_largest_node()->next = NULL;
}
template <typename T>
void BPlusTree<T>::copy_tree(const BPlusTree<T>& other,vector<BPlusTree<T>* > & vekter){
    if(DEBUG)  cout<<endl<<"------------- Copy Tree -------------"<<endl;

    copy_array(this->data,this->data_count,other.data,other.data_count);
    this->child_count = other.child_count;
    for(int i = this->child_count - 1; i >= 0; i--){
        this->subset[i] = new BPlusTree<T>(other.dups_ok);
        this->subset[i]->copy_tree(*other.subset[i] , vekter);
        if( this->subset[i]->is_leaf() && vekter.empty() && i <= this->child_count - 1)
        {
            this->subset[ i ]->next = this->subset [ i + 1 ] ;
            if( i == 0) vekter.push_back( this->subset[ i ]) ;
        }
        else if(this->subset[i]->is_leaf() && i == this->child_count - 1)
        {
            if(vekter.size()) {
                this->subset[i]->next = vekter.front();
                vekter.pop_back();
            }
        }
    }
    if(DEBUG)  cout<<endl<<"------------- Passed Copy Tree -------------"<<endl;

}
//---------------------------------------------------------------
//                   P R I N T
//---------------------------------------------------------------
template <typename T>
void BPlusTree<T>::print_tree(int level , ostream &outs) const{ //print a readable version of the tree

    if(DEBUG)  cout<<endl<<"--------------- print ---------------"<<endl;
    HANDLE hConsol = GetStdHandle(STD_OUTPUT_HANDLE);
    if(child_count > 1){
        subset[child_count - 1]->print_tree(level+1,outs);
    }
    for(int i =0; i < this->data_count; i++){
        {
            if(i == 0)
            {
                SetConsoleTextAttribute(hConsol,level+1);
                outs<<setw(level*4)<<data[data_count - i - 1]<<endl;
                SetConsoleTextAttribute(hConsol,10);
            }
            else if(i == data_count-1)
            {
                SetConsoleTextAttribute(hConsol,level+1);
                outs<<setw(level*4)<<data[data_count - i - 1]<<endl;
                SetConsoleTextAttribute(hConsol,10);
            }
            else{
                SetConsoleTextAttribute(hConsol,level+1);
                outs<<setw(level*4)<<data[data_count - i - 1]<<endl;
                SetConsoleTextAttribute(hConsol,10);
            }
        }
        if(!is_leaf())
            subset[data_count - i -1]->print_tree(level+1 , outs);
    }
}
#endif // BPLUSTREE_H
