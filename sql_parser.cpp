#include "sql_parser.h"
int sql_parser::parser_table[MAX_ROWS_PARSER][MAX_COLUMNS_PARSER];
sql_parser::sql_parser(string input)
{
    //make the table for parser
    init_table(parser_table);
    make_table_for_parser(parser_table);
    
    char* str = new char[input.length()+1];
    memcpy(str, input.c_str(), input.length()+1);
    stk.set_string(str);
    //as soon as input comes in, tokenize it and parse into words
    parse_into_words();
    init_key_words();
}

sql_parser::sql_parser(char str[]):stk(str)
{
    //make the table for parser
    init_table(parser_table);
    make_table_for_parser(parser_table);
    
    //as soon as input comes in, tokenize it and parse into words
    parse_into_words();
    init_key_words();
    //cout<<keywords<<endl;
}
//as soon as input comes in, process it without the spaces and push them into a queue
void sql_parser::parse_into_words(){
    Token t;
    stk>>t;
    while(stk.more()){
        if(t.type_string() != "SPACE")
            parse_queue.push(t.get_token());
        stk>>t;
    }
}
//determines the success or the fail of the command but clears the map
//before it gets run through a state machine,it must be cleared
bool sql_parser::parse_tree(MMap<string,string>  & ptree){
    ptree.clear();
    return machine(ptree);
}
//for a specific string, if its is a keywords, we wanna map it to the index it belongs to
//if not, we want to set the column for special names that are not keywords
int sql_parser::column_number(string input){
    bool debug = false;
    if(keywords.contains(input))
    {
        if(debug) cout<<"value for input |"<<input<<"| is "<<keywords[input]<<endl;
        return keywords[input];
    }
    else return SYMBOL;
}

//after you parse input into words, identify them and parse
//them into commands and push them into a command queue
bool sql_parser::machine(MMap<string,string>  & ptree)
{
    //make an output queue to process commands and send them to whoever needs the comands
    string processer ;
    bool found_where = false;
    bool is_all = false;
    int column;
    int newstate = 0;
    bool debug = false;
    //while there are more words in queue, keep processing them
    while(!parse_queue.empty() && newstate != -1){
        processer = parse_queue.pop();
        //if(processer == "cls" ) return true;

        column = column_number(processer);
        if(debug) {
            cout<<"processed: "<<"||"<<processer<<"||"<<endl;
            cout<<"new column: "<<column<<endl;
            cout<<"state: "<<newstate<<endl;
        }

        newstate =  parser_table[newstate][column];
        switch (column){
        case HELP:
        case SELECT:
        case CREATE:
        case DROP:
        case LOAD :
        case PRINT :
        case STORE :
        case INSERT:
            ptree[COMMAND] += processer;
            break;
        case WHERE :
            found_where = true;
            break;
        case COMPAREVALUE:
            ptree[OP] += processer;
            break;
        case ASTERISK:
            is_all = true;
            break;
        case AND:
            ptree["and"] += "and";
            break;
        case OR:
            ptree["or"] +="or";
            break;
        case COMMA:
            break;
        default:
            //the case where it handles specifi names and not keywords
            if(newstate == fieldnamestate || newstate == fieldnamestate2)
                ptree[FIELDLIST] +=processer;
            if(newstate == tablenamestate || newstate == createtablenamestate
                    || newstate == inserttablenamestate)
                ptree[TABLENAMEE] += processer;
            if(newstate == columnstate)
                ptree[COLUMN] += processer;
            if(newstate == valuestate || newstate == insertvaluestate)
                ptree[VALUE] += processer;
        }

    }
    if(is_all) ptree[ALL] += YES;
    else ptree[ALL] += NO;
    if(found_where)  ptree[WHEREE] += YES;
    else  ptree[WHEREE] += NO;
    return (is_success(parser_table,newstate));
}

//this is the state machine that we will use to validate commands
void sql_parser::make_table_for_parser(int _table[][MAX_COLUMNS])
{
    //mark fail and success states for a command like success
    mark_fail(_table,0);
    mark_fail(_table,1);
    mark_fail(_table,2);
    mark_fail(_table,3);
    
    //success only when you end up with a table name at the end
    mark_success(_table,4);
    
    //basically reserved for fieldnames
    mark_fail(_table,9);
    //reserved for commas
    mark_fail(_table,10);
    //reserved for print
    mark_fail(_table,5);
    //reserved for drop
    mark_fail(_table,6);
    //reserved for store
    mark_fail(_table,7);
    //reserved for load
    mark_fail(_table,8);
    //reserved for where
    mark_fail(_table,11);
    //reserved for lhs of condition
    mark_fail(_table,12);
    //reserved for condition operator ie: = , > , <
    mark_fail(_table,13);
    //reserved for rhs of condition
    mark_success(_table,14);
    //reserved for the word and
    mark_fail(_table,15);
    //reserved for insert into
    mark_fail(_table,16);
    mark_fail(_table,17);
    mark_fail(_table,18);
    mark_fail(_table,19);
    mark_fail(_table,20);
    mark_fail(_table,21);
    mark_success(_table,22);
    mark_fail(_table,23);
    //reserved for create table
    mark_fail(_table,24);
    mark_fail(_table,25);
    mark_fail(_table,26);
    mark_fail(_table,27);
    mark_fail(_table,28);
    mark_fail(_table,29);
    mark_fail(_table,30);
    mark_fail(_table,31);
    mark_success(_table,32);
    mark_fail(_table,33);
    mark_fail(_table,34);
    mark_success(_table,40);
    mark_success(_table,41);

    //----------------------------------------------------------------------------
    //----------S T A T E   M A C H I N E   D E F I N I T I O N S -------
    //----------------------------------------------------------------------------
    //state machine for select
    //select <column expr0>,<column expr1>,... from <table0>,<table1>,
    //... where <cond0> and <cond1> and ...
    //It is an error to write a select statement that involved no columns of the listed tables.
    mark_cell(0,_table,SELECT,1);
    mark_cell(1,_table,SYMBOL,9);
    mark_cell(9,_table,COMMA,10);
    mark_cell(10,_table,SYMBOL,9);
    mark_cell(9,_table,FROM,3);
    mark_cell(1,_table,ASTERISK,2);
    mark_cell(2,_table,FROM,3);
    mark_cell(3,_table,SYMBOL,4);
    mark_cell(4,_table,COMMA,3);
    mark_cell(4,_table,WHERE,11);
    mark_cell(11,_table,SYMBOL,12);
    mark_cell(12,_table,COMPAREVALUE,13);
    mark_cell(13,_table,SYMBOL,14);
    mark_cell(14,_table,AND,15);
    mark_cell(14,_table,OR,15);
    mark_cell(15,_table,SYMBOL,12);
    
    //state machine for print
    // print <table name>
    mark_cell(0,_table,PRINT,5);
    mark_cell(5,_table,SYMBOL,4);
    
    //state machine for drop
    //drop <table name>
    mark_cell(0,_table,DROP,6);
    mark_cell(6,_table,SYMBOL,4);
    
    
    //state machine for store
    //store <table name>
    mark_cell(0,_table,STORE,7);
    mark_cell(7,_table,SYMBOL,4);
    
    //state machine for load
    //load <table name>
    mark_cell(0,_table,LOAD,8);
    mark_cell(8,_table,SYMBOL,4);
    
    //state machine for create table
    //create table <table name> (<column0 name> <type0>, <column1 name> <type1>, ...)
    mark_cell(0,_table,CREATE,24);
    mark_cell(24,_table,TABLE,26);
    mark_cell(26,_table,SYMBOL,28);
    mark_cell(28,_table,SYMBOL,30);
    mark_cell(30,_table,INTT,32);
    mark_cell(30,_table,FLOATT,32);
    mark_cell(30,_table,STRINGG,32);
    mark_cell(32,_table,COMMA,28);
    
    //state machine for insert into
    //insert into <table name> values <literal0>,<literal1>,...
    mark_cell(0,_table,INSERT,16);
    mark_cell(16,_table,INTO,18);
    mark_cell(18,_table,SYMBOL,20);
    mark_cell(20,_table,VALUES,21);
    mark_cell(21,_table,SYMBOL,22);
    mark_cell(22,_table,COMMA,23);
    mark_cell(23,_table,SYMBOL,22);

    //IF USER INPUTS YES
    mark_cell(0,_table,CLS,40);

    //user inputs help
    mark_cell(0,_table,HELP,41);
}

void sql_parser::init_key_words(){
//keywords that are used in our map
    //for specific commands
    //the more keywords u have, the stronger the state machine will be
    keywords["cls"] = CLS;
    keywords["help"] = HELP;
    keywords["select"] = SELECT;
    keywords["*"] = ASTERISK;
    keywords[","] = COMMA;
    keywords["from"]  = FROM ;
    keywords["drop"]  = DROP ;
    keywords["load"]  = LOAD ;
    keywords["print"]  = PRINT ;
    keywords["store"]  = STORE ;
    keywords["where"]  = WHERE ;
    keywords["and"]  = AND ;
    keywords["or"] = OR;
    keywords[">"]  = COMPAREVALUE ;
    keywords[">="]  = COMPAREVALUE ;
    keywords["="]  = COMPAREVALUE ;
    keywords["=="]  = COMPAREVALUE ;
    keywords["!="]  = COMPAREVALUE ;
    keywords["<"]  = COMPAREVALUE ;
    keywords["<="]  = COMPAREVALUE ;
    keywords["insert"]  = INSERT ;
    keywords["into"]  = INTO ;
    keywords["create"]  = CREATE ;
    keywords["values"]  = VALUES ;
    keywords["table"] = TABLE ;
    keywords["integer"]  = INTT ;
    keywords["int"]  = INTT ;
    keywords["float"]  = FLOATT ;
    keywords["string"] =  STRINGG ;
}
