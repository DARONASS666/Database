#include "ftokenizer.h"
FTokenizer::FTokenizer(char* fname):_pos(0),_blockPos(0),_more(1)
{
    _f.open(fname, fstream::in | fstream::binary | fstream::ate);
    if(_f.fail())
    {
        cout<<"failed to open file"<<endl;
        exit(-666);
    }

    file_size = _f.tellg();
    _f.seekg(0, ios::beg);
    get_new_block();

}

Token FTokenizer::next_token()
{
    Token t;
    t = Token();
    _stk >>t;
    return t;
}
bool FTokenizer::more()  //returns the current value of _more
{
    return _more;
}
int FTokenizer::pos() //returns the value of _pos
{
    return _pos;
}
int FTokenizer::block_pos()     //returns the value of _blockPos
{
    return _blockPos;
}
FTokenizer& operator >> (FTokenizer& f, Token& t)
{
    // extracts tokens from file and returns them
    if(!f._stk.more())
    {
        f.get_new_block();
    }
    t = f.next_token();
    return f;
}
//.............private functions...........//
//gets the new block from the file
bool FTokenizer::get_new_block()
{
    if(_f)
    {
        char buf[MAX_BLOCK];
        _f.read(buf,MAX_BLOCK-1);
        buf[_f.gcount()] = '\0';
        _stk = STokenizer(buf);

        if(_f.gcount() > 0)
        { return  _more = true;}
    }
    else
    {_f.close(); return _more = false;}
}
