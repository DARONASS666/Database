#include "stokenizer.h"

int STokenizer::_table[MAX_ROWS][MAX_COLUMNS];
STokenizer::STokenizer()
{
    _pos = 0;
    make_table(_table);
}
STokenizer::STokenizer(char str[])
{
    set_string(str);
    make_table(_table);
}

bool STokenizer::done()
{
    return (_pos >= strlen(_buffer));
}
bool STokenizer::more()
{
    return (_pos <= strlen(_buffer));
}

//---------------
//extract one token (very similar to the way cin >> works)
STokenizer& operator >> (STokenizer& s, Token& t)
{
    string temp;  // = t.get_token();
    //check if it is alpha
    if(s.get_token(ALPHA_START,temp))
    {
        t = Token(temp,TYPE_ALPHA);
        return s;
    }
    //check if its a number
    else if(s.get_token(DIGIT_START,temp))
    {
        t = Token(temp,TYPE_DIGIT);
        return s;
    }
    //check if its a punctuation
    else if(s.get_token(PUNCT_START,temp))
    {
        t = Token(temp,TYPE_PUNCT);
        return s;
    }
    else if(s.get_token(COMMA_START,temp)){
        t = Token(temp,TYPE_COMMA);
        return s;
    }
    //check if its a space
    else if(s.get_token(SPACE_START,temp))
    {
        t = Token(temp,TYPE_SPACE);
        return s;
    }
    else if(s.get_token(LTE_START,temp))
    {
        t = Token(temp,TYPE_LTE);
        return s;
    }
    else if(s.get_token(GTE_START,temp))
    {
        t = Token(temp,TYPE_GTE);
        return s;
    }
    else if(s.get_token(EQUAL_START,temp))
    {
        t = Token(temp,TYPE_EQUAL);
        return s;
    }
    else
    {
        t = Token(string(1,s._buffer[s._pos]),TYPE_UNKNOWN);
        s._pos++;

    }
    return s;
}
//set a new string as the input string
void STokenizer::set_string(char str[])
{
    strcpy(_buffer,str);
    _pos = 0;
}
//...............................private functions...................//
//create table for all the tokens we will recognize
//                      (e.g. doubles, words, etc.)
void STokenizer::make_table(int _table[][MAX_COLUMNS])
{
    init_table(_table);


    mark_fail(_table, 0);            //Mark states 0 and 2 as fail states
    mark_success(_table, 1);         //Mark states 1 and 3 as success states
    mark_fail(_table, 2);
    mark_success(_table, 3);

    mark_fail(_table, 4);            //Mark states 4 and 6 as fail states
    mark_success(_table, 5);        //Mark state 5 as success state
    mark_fail(_table, 6);
    mark_success(_table, 7);
    mark_success(_table, 8);
    mark_success(_table, 9);
    mark_success(_table, 10);

    mark_fail(_table , 11);
    mark_success(_table , 12);

    mark_fail(_table , 13);
    mark_success(_table , 14);

    mark_fail(_table,15);
    mark_success(_table,16);

    mark_success(_table,17);
    mark_success(_table,18);
    mark_success(_table,19);
    mark_success(_table,20);
    mark_success(_table,21);
    mark_success(_table,22);
    mark_success(_table,23);


    //doubles:
    mark_cells(0,_table, DIGITS, 1);         //state [0] --- DIGITS ---> [1]
    mark_cells(0,_table,DECIMALS, 2);           //state [0] --- '.' ------> [2]
    mark_cells(1,_table, DIGITS, 1);         //state [1] --- DIGITS ---> [1]
    mark_cells(1,_table,DECIMALS, 2);           //state [1] --- '.' ------> [2]
    mark_cells(2,_table, DIGITS, 3);      //state [2] --- DIGITS ---> [3]
    mark_cells(3,_table, DIGITS, 3);     //state [3] --- DIGITS ---> [3]

    //alphabet:
    mark_cells(4,_table, ALPHA, 5);
    mark_cells(5,_table, ALPHA, 5);
    mark_cell(5,_table,39, 6);
    mark_cells(6,_table, ALPHA, 7);
    mark_cells(7,_table, ALPHA, 8);
    mark_cells(5,_table, DIGITS, 9);
    mark_cells(9,_table, DIGITS, 9);
    mark_cells(9,_table, ALPHA, 10);
    mark_cells(10,_table, ALPHA, 10);
    mark_cells(10,_table, DIGITS, 9);


    //space:
    mark_cells(11,_table,SPACE,12);
    mark_cells(12,_table,SPACE,12);

    //punctuation:
    mark_cells(13,_table,PUNCT,14);
    mark_cells(14,_table,PUNCT,14);

    //comma:
    mark_cells(15,_table,COMMAS,16);

    //lessthanorequalto:         // < 60
    mark_cell(17,_table,60,18);
    mark_cell(18,_table,61,23);

    //greaterthanorequalto:      // > 62
    mark_cell(19,_table,62,20);
    mark_cell(20,_table,61,23);
    //double-equalto:             // = 61
    mark_cell(21,_table,61,22);
    mark_cell(22,_table,61,23);

}

//extract the longest string that match
//     one of the acceptable token types
bool STokenizer::get_token(int start_state, string& token)
{
    if(_table[start_state][_buffer[_pos]] == -1) return false;
    int valid_pos = _pos;
    int start_pos = _pos;


    while(start_state != - 1 && !done())
    {
        start_state = _table[start_state][_buffer[_pos++]];
        if(is_success(_table,start_state))
        {
            valid_pos = _pos;
        }
    }
    for(int i = start_pos; i< valid_pos; i++) token.push_back(_buffer[i]);
    _pos = valid_pos;
    return valid_pos - start_pos >0;
    //this function returns if it successfully parsed the characters
    //into a single identified token.
}
//---------------------------------
