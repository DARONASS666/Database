//DARON ASSADOURIAN
//FINAL PROJECT DATABASE CS 8 FINAL


#include <iostream>
#include "map.h"
#include "mmap.h"
#include "sql_parser.h"
#include "table.h"
#include "mysql.h"
#include "ftokenizer.h"

using namespace std;
void test(){
    BPlusTree<int> bt;
    char command;
    while(command != 'E')
    {
        cout<<"========================================================================"<<endl;
        cout<<"|[I]nsert remo[V]e c[O]ntains [S]ize [R]andom [C]lear veri[F]y [E]xit |"<<endl;
        cout<<"========================================================================"<<endl;
        cin>>command;
        cout<<endl;
        
        switch(command)
        {
        int item;
        
        case 'i':
        case 'I':
            cin>>item;
            cout<<"inserting "<<item<<" into tree..."<<endl;
            bt.insert(item);
            break;
        case 's':
        case 'S':
        {
            cout<<"tree size:"<<bt.size()<<endl;
            system("pause");
        }
            break;
        case 'C':
        case 'c':
            bt.clear_tree();
            break;
        case 'v':
        case 'V':
            cin>>item;
            bt.remove(item);
            break;
        case 'O':
        case 'o':
            cin>>item;
            if (bt.contains(item))
                cout<<"found "<<item<<endl;
            else
                cout<<"didn't find "<<item<<endl;
            system("pause");
            break;
        case 'r':
        case 'R':
        {
            int ran = rand() % 100;
            cout<<"inserting "<<ran<<" into tree"<<endl;
            bt.insert(ran);
            cout<<endl;
        }
            break;
        case 'f':
        case 'F':
            if(bt.is_valid())
                cout<<"BTREE IS VALID"<<endl;
            else
                cout<<"BTREE IS INVALID"<<endl;
            system("pause");
            break;
        case 'x':
        case 'X':
            cout<<"printing linked list on the bottom of tree......."<<endl;
            bt.print_linked_list();
            cout<<endl<<"ended printing the list ......"<<endl;
            
        case 'e':
        case 'E':
            char exited;
            cout<<"are you sure you want to exit?"<<endl;
            cout<<"input y to continue"<<endl;
            cin>>exited;
            if(exited == 'y' || exited == 'Y')
            {
                cout<<"User Exited Main Menu with EXIT CODE 666"<<endl;
                cout<<"====================="<<endl<<endl;
                exit(666);
            }
        }
        system("pause");
        system("cls");
        cout<<bt<<endl;
        bt.print_linked_list(); cout<<endl;
        
    }
    
}
bool FoundIn(string checkthis ){
    string keywords [61] = { "SIGNED", "WANTS" , "TO" , "VOLUNTEER" , "DAYS" , "AGO" , "MONTHS" , "FOR",
                             "FRIENDS" , "RAISE" , "MONEY" , "ABOUT" , "THIS" , "ON" , "FACEBOOK" ,
                             "Join" , "me" , "Get" , "more" , "about" , "or" , "direct" , "surf" , "to" , "MONTH" ,
                             "united" , "UNITED" , "United" , "POSTED" , "posted" , "Armenian" , "ARMENIAN" ,
                             "I" , "signed" , "of" , "to" , "volunteer" , "for" , "Unified" , "Young" , "Armenians" ,
                             "(UYA)" , "UYA" , "UP" , "up" , "Retweet" , "Favorite" , "Reply" , "FRIEND" , "signed" , "ONLINE"
                             "TWEETED" , "LINK" , "PAGE" , "JACKDANIEL101" , "LAPTOP" , "ADAPTOR" , "MURAH"
                             "the" , "M" , "ONTHS" ,
                           };

    for(int i = 0 ; i < 61; i++)
    {
        if(keywords[ i ] == checkthis) return true;
    }
    return false;
}

int main()
{
//    //    ifstream uya;
//    //    uya.open("uya.text");
//    FTokenizer ftk("uya.txt");
//    Token t;
//    my_queue<string> listOfWords;
//    string processed;

//    ftk>>t;
//    while(ftk.more())
//    {
//        if(t.type_string() == "ALPHA" )
//        {
//            processed = t.get_token();
//            if(!FoundIn(processed))
//                listOfWords.push(processed);
//        }
//        ftk>>t;
//    }
//    cout<<listOfWords<<endl;

    test();
    //    sql.run();
    return 666;
}

//when i try to find iterator in a +tree and the item doesnt exist, program crashes

//BUG IS FROM WHEN I COPY A MULTIMAP INTO ANOTHER ONE, SOMETHING IS WRONG IN
//MMAP COPY
//update: copy crashes only when printing iterators, copying maps and mmaps work
//bug probably at bplustree


//to keep track of bplustree size , make a private unique_size for iterator and increment this size everytime u insert
//a unique value on a leaf level.
