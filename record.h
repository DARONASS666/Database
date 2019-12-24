#ifndef RECORD_H
#define RECORD_H
#include <iostream>
#include <fstream>
#include <cstring>
#include <vector>
#include <iomanip>
using namespace std;
class Record{
public:
    Record(int _size){
        for(int i = 0 ; i < MAX ; i++)
            record[i][0] = NULL;
        recno = -1;
    size = _size;
    }
    Record(){
        for(int i = 0 ; i < MAX ; i++)
            record[i][0] = NULL;
        recno = -1;
    size = 0;
    }
    Record(const char str[]){
        // strcpy(record, str);
    cout<<"RECORD CONSTRUCTOR FIRED"<<endl;
    }
    Record(vector<string> v){
        for(int i = 0; i <MAX ; i++){
            record[i][0] = NULL;
        }
        for(int i = 0; i < v.size() ; i++){
            strcpy(record[i] , v[i].c_str());
        }
        size = 0;
    }
    long write(fstream& outs);
    long read(fstream& ins, long recno);
    long recordsize();


    string get_row(int index){
        string returner = "";
        for(int i  = 0 ; record[index][i] != NULL ; i++)
        {
            returner += record[index][i];
        }
        return returner;
    }

    friend ostream& operator<<(ostream& outs,
                               const Record& r);
private:
    static const int MAX = 50;
    int size;
    int recno;
    int fieldnumber;
    char record[MAX][MAX];
};
//-------------------------------------------------
#endif // RECORD_H
