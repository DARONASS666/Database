#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>
using namespace std;
//constants for STokenizer and state machine
const int MAX_ROWS = 30;
const int MAX_COLUMNS = 256;
const int MAX_BUFFER = 1000;
const char DIGITS[] = "0123456789";
const char DECIMALS[] = ".";
const char ALPHA[] ="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char SPACE[] = " ";
const char COMMAS[] = ",";
const char PUNCT[] = "!\"()?;:,.*";
const int ALPHA_START = 4;
const int DIGIT_START = 0;
const int SPACE_START = 11;
const int PUNCT_START = 13;
const int COMMA_START = 15;
const int UNKNOWN_START = 29;
const int TYPE_ALPHA = 2;
const int TYPE_DIGIT = 1;
const int TYPE_SPACE = 3;
const int TYPE_PUNCT = 4;
const int TYPE_UNKNOWN = 5;
const int TYPE_COMMA = 6;
const int TYPE_LTE = 7;
const int TYPE_GTE = 8;
const int TYPE_EQUAL = 9;
const int LTE_START =17;
const int EQUAL_START =21;
const int GTE_START =19;
//constants for sql parser
const int MAX_ROWS_PARSER = 50 ;
const int MAX_COLUMNS_PARSER = 256 ;
//const int COMMA =  44;
//const int ASTERISK = 42;
const char CONDITIONS[] = ">=<";
const string COMMAND = "command";
const string OP = "op";
const string FIELDLIST = "fieldlist";
const string TABLENAMEE = "tablename";
const string COLUMN = "column";
const string VALUE = "value";
const string ALL = "all";
const string WHEREE = "where";
const string YES = "yes";
const string NO = "no";
enum commands {
    SYMBOL = 66,
    SELECT = 1,
    FIELDNAME = 2,
    FROM = 3,
    TABLENAME = 4,
    DROP = 5,
    LOAD = 6,
    PRINT = 7,
    STORE = 8,
    WHERE = 9,
    AND = 10,
    COMPAREVALUE = 11,
    INSERT = 12,
    INTO = 13,
    CREATE = 14,
    VALUES = 15,
    LITERAL = 16,
    TABLE = 17,
    INTT = 18,
    FLOATT = 19,
    STRINGG = 20,
    OR = 21,
    ASTERISK = 42,
    COMMA = 44,
    YESS = 40,
    CLS = 254,
    HELP = 255,
};

enum PARSERSUCCESSROWS{
    tablenamestate = 4,
    fieldnamestate = 9,
    fieldnamestate2 = 30,
    columnstate = 12,
    valuestate = 14,
    createtablenamestate = 28,
    inserttablenamestate = 20,
    insertvaluestate = 22,
};

#endif // CONSTANTS_H
