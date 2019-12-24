#ifndef LIST_FUNCTIONS_H
#define LIST_FUNCTIONS_H
#include <iostream>
#include <cassert>
using namespace std;

template <class T>
struct node{
     T _item;
    node<T>* _next;
    node(const T& item = T(), node<T>* next = NULL):_item(item), _next(next){}
    friend ostream& operator << (ostream& outs, const node<T>& print_me){
        outs<<"["<<print_me._item<<"]->";
        return outs;
    }
};

//initializes head to NULL
template <class T>
node<T>* init_head(node<T>* &head)
{
    head = NULL;
    return head;
}

//true if head is NULL, false otherwise.
template <class T>
bool empty(const node<T>* head)
{
    return (!head);
}

//makes a copy of the list, returns a pointer to the last node:
template <class T>
node<T>* copy_list(node<T>* head, node<T>* & cpy)
{
   // cout<<"IN COPY LIST FOR LINKED LIST LOWER LEVEL"<<endl;
   // assert(head);

    node<T>* walker = head;
    node<T>* cpywalker = NULL;
    cpy=NULL;
    while(walker)
    {
        if(!cpy)
        {
            insert_head(cpy,walker->_item);
            cpywalker = cpy;
        }
        else
        {
            cpywalker = insert_after(cpy,cpywalker,walker->_item);
        }
        walker = walker->_next;
    }
    return cpywalker;
}
template <typename T>
T _delete(node<T>* &head_ptr, node<T>* remove)
{
    T deleted = remove->_item;
    _remove(head_ptr,remove);
    return deleted;
}

template <typename T>
node<T>* _remove(node<T>* &head_ptr,node<T>* remove)
{
    assert(head_ptr);
    assert(remove);
    if(head_ptr != remove)
    {
        _previous(head_ptr,remove)->_next = remove->_next;
    }
    else
    {
        head_ptr = head_ptr->_next;
    }
    remove->_next = NULL;
    return remove;
}

//insert at the beginning of the list:
template <class T>
node<T>* insert_head(node<T>* &head, T item)
{
    node<T>* temp = new node<T>(item);

    temp->_next = head;

    head = temp;

    return temp;
}

//insert_after: if after is NULL, inserts at head
template <class T>
node<T>* insert_after(node<T>* &head, node<T>* &after, const T& item)
{
    //WHEN FOUND RETURNS NULL, IT COMES HERE AND TO !AFTER AND INSERTS AT HEAD SO ITS A PROBLEM
    if(!after)
    {
        return insert_head(head,item);
    }
    else
    {
        return insert_head(after->_next,item);
    }
}

template <class T>
node<T>* insert_before(node<T>* head,node<T> *pos, T item)
{
    insert_after(head,pos,pos->_item);
    pos->_item=item;
    return pos;

}

//WARNING : ASK BARKESHLI ABOUT THIS
template <typename T>
node<T>* _find(node<T>* &head_ptr,T item)
{
    node<T>* tempwalker = head_ptr;

    while(tempwalker)
    {
        if (tempwalker -> _item == item)
            return tempwalker;

        tempwalker= tempwalker->_next;
    }
    return NULL;
}

//delete the node at the head of the list, reuturn the item:
template <class T>
T delete_head(node<T>* &head)
{
    //cout<<"delete head called..."<<endl;
    assert(head);

    node<T>* walker = head;
    T item = head->_item;

    head = head->_next;
    delete walker;
    return item;

}

template <typename T>
node<T>* _previous (node<T>* head_ptr, node<T>* me)
{
    assert(me);
    assert(head_ptr);

    node<T>* walker = head_ptr;
    while (walker)
    {
        if(walker->_next == me)
        {
            return walker;
        }
        walker= walker -> _next;
    }
    return NULL;
}
//deletes all the nodes in the list
template<class T>
void delete_all(node<T>*&head)
{
    //cout<<"head in delete all appears to be: "<<endl<<*head<<endl;
    assert(head);
    while (head)
    {
        delete_head(head);
    }
    //cout<<"delete all passed..."<<endl;
}

//print the list and return outs
template<class T>
ostream& print_list(const node<T>* head, ostream& outs=cout)
{
    const node<T>* walker = head;
    outs<<"H-ptr->";
    while (walker)
    {
        outs<<"["<<walker->_item<<"]->";
        walker = walker->_next;
    }
    cout<<"|||";
    return outs;
}
#endif // LIST_FUNCTIONS_H
