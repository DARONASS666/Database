#ifndef MY_QUEUE_H
#define MY_QUEUE_H
#include <iostream>
#include <list_functions.h>

using namespace std;

template <class T>
class my_queue{
public:
    my_queue():_head(NULL), _tail(NULL){}

    ~my_queue();
    my_queue(const my_queue<T>& other);
    my_queue<T> &operator =(const my_queue& rhs);

    void push(T item);
    T pop();
    bool empty();
    T front();

    friend ostream& operator <<(ostream& outs, const my_queue& q){
        return print_list(q._head, outs);

    }

private:
    node<T>* _head ;
    node<T>* _tail ;
};
template <typename T>
my_queue<T>::~my_queue()
{
    //cout<<"destructor fired..."<<endl;
    if(_head)
    {
        delete_all(_head);
        _tail = NULL;
    }
   // cout<<"destructor passed..."<<endl;

}
template <typename T>
my_queue<T>::my_queue(const my_queue<T>& other)
{
    //cout<<"copy constructor fired..."<<endl;

    _tail = copy_list(other._head,this->_head);
    //cout<<"copy constructor passed..."<<endl;
}
template <typename T>
my_queue<T> & my_queue<T>::operator =(const my_queue<T>& rhs)
{
    //cout<<"assignment operator fired.."<<endl;
    //cout<<"head is:"<<_head<<endl;

    if(this != &rhs)
    {
        //cout<<"head is:"<<_head<<endl;
        if(_head)
        {
            _tail = NULL;
            delete_all(_head);
        }
        //cout<<"delete all in assignment op passed"<<endl;
        _tail = copy_list(rhs._head,this->_head);
    }
    //cout<<"assignment operator passed.."<<endl;

    return *this;
}
template <typename T>
void my_queue<T>::push(T item)
{
    if(!_tail && !_head )
    {
        insert_head(_head,item);
        _tail = _head;
    }
    else
    {
        _tail = insert_after(_head,_tail,item);
    }
}
template <typename T>
T my_queue<T>::pop()
{
    if(!empty())
    {
        //cout<<"queue pop entered"<<endl;
        if(_head == _tail)
        {
            _tail=NULL;
        }
        return delete_head(_head);
    }
    exit(-1);
}
template <typename T>
bool my_queue<T>::empty()
{
    return _head == NULL;
}
template <typename T>
T my_queue<T>::front()
{
    assert(!empty());
    return _head->_item;
}

#endif // MY_QUEUE_H
