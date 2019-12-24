#include "state_machine.h"

//Fill all cells of the array with -1
void init_table(int _table[][MAX_COLUMNS])
{
    for (int i = 0; i < MAX_ROWS; i++)
        for(int j=0; j < MAX_COLUMNS ; j++)
            _table[i][j] = -1;
}

//Mark this state (row) with a 1 (success)
void mark_success(int _table[][MAX_COLUMNS], int state)
{
        _table [state][ 0 ] = 1;
        //cout<<"at state:"<<state<<" truth value is:"<<_table[state][0]<<endl<<endl;
}

//Mark this state (row) with a 0 (fail)
void mark_fail(int _table[][MAX_COLUMNS], int state)
{
        _table [state][ 0 ] = 0;
       // cout<<"at state:"<<state<<" truth value is:"<<_table[state][0]<<endl<<endl;

}

//true if state is a success state
bool is_success(int _table[][MAX_COLUMNS], int state)
{
   // cout<<"at state:"<<state<<" truth value is:"<<_table[state][0]<<endl<<endl;
    return (_table[state][0] == 1);
}

//Mark a range of cells in the array.
void mark_cells(int row, int _table[][MAX_COLUMNS], int from, int to, int state)
{
    for(int j = from ; j < to ; j++)
        _table[row][ j ] = state;
}

//Mark columns represented by the string columns[] for this row
void mark_cells(int row, int _table[][MAX_COLUMNS], const char columns[], int state)
{
    for (int i = 0; i < strlen(columns) ; i++)
        _table[row][ columns[i] ] = state;
}

//Mark this row and column
void mark_cell(int row, int _table[][MAX_COLUMNS], int column, int state)
{
    _table[row][column] = state ;
}

//This can realistically be used on a small table
void print_table(int _table[][MAX_COLUMNS])
{
    for(int i = 0 ; i < MAX_ROWS ; i++)
    {
        for(int j = 0 ; j < MAX_COLUMNS ; j++)
        {
            cout<<"["<<setw(2)<<_table[ i ][ j ]<<"] ";
        }
        cout<<endl;
    }
}

//show string s and mark this position on the string:
//hello world   pos: 7
//      ^
void show_string(char s[], int _pos)
{
    cout<<s<<" pos:"<<_pos<<endl;
    if(_pos)
        cout<<setw(_pos)<<"^"<<endl;
}
