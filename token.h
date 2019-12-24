#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
using namespace std;

class Token
{
public:
    Token(string str = "", int type = 4):_token(str), _type(type){}
    friend ostream& operator <<(ostream& outs, const Token& t){return outs<<"|"<<t._token<<"|";}

    int get_type(){return _type;}
    string get_token(){return _token;}
    string type_string(){
        switch(get_type())
        {
        case 1:  return "DIGIT";
        case 2: return "ALPHA";
        case 3: return "SPACE";
        case 4: return "PUNCT";
        case 5: return "UKNWN";
        case 6: return "COMMA";
        case 7: return "LTE";
        case 8: return "GTE";
        case 9: return "EQUAL";
        }
    }


private:
    string _token;
    int _type;
};
#endif // TOKEN_H
