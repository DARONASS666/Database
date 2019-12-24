#ifndef SQL_PARSER_H
#define SQL_PARSER_H
#include <iostream>
#include <string.h>
#include "my_queue.h"
#include "token.h"
#include "mmap.h"
#include "map.h"
#include "state_machine.h"
#include "stokenizer.h"

using namespace std;

class sql_parser
{
public:
    sql_parser(string input);
    sql_parser(char str[]);
    void parse_into_words();
    void init_key_words();
    int column_number(string input);
    bool parse_tree(MMap<string, string> &ptree);
    bool machine(MMap<string,string>  & ptree);
private:
    my_queue<string> parse_queue;
    STokenizer stk;
    Map<string,int> keywords;//give every keyword their column number
    MMap<string,string> Parsetree; // parsetree of commands that will be given to table class

    //this is the function that will set the state machine for the parser of the commands
    void make_table_for_parser(int _table[][MAX_COLUMNS_PARSER]);
    
    //this is the 2-D Array that will be used to store the success and fail states
    static int parser_table[MAX_ROWS_PARSER][MAX_COLUMNS_PARSER];
};

#endif // SQL_PARSER_H
