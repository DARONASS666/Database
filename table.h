#ifndef TABLE_H
#define TABLE_H
#include <vector>
#include <cstring>
#include "map.h"
#include <fstream>
#include <iostream>
#include "mmap.h"
#include "record_utility_functions.h"
using namespace std;

class Table
{
public:
    Table();
    Table(string tablename); // when you load a table
    Table(string tablename,vector<string> fieldlist); // when you create something
    void insert(vector<string> v);
    void reindex();
    void show_multimap_values();
    Table select(string newtable,const vector<string> & fieldlist ,
                 string fieldname, string operation , string value);
    Table select(string newtable, vector<string> fieldlist);
    void store(vector<string> fieldlist);
    void load(string tablename);
    void initrecordcount();
    void printline() const;


    void show() const;
    friend ostream& operator<<(ostream& outs, const Table& print_me){
        print_me.show();
        return outs;
    }
    string get_table_name(){ return table_name;}
    int get_field_size(){ return field_size;}
    Map<string,long> get_field_list(){ return field_list;}
private:
    string table_name;
    int field_size;
    int record_count; // count how many records in this table
    //vector<MMap<string,long> > indices;
    MMap<string,long> indices[ 10 ];
    Map<string,long> field_list;
};

#endif // TABLE_H
