#include "record.h"

long Record::write(fstream &outs){
    //write to the end of the file.
    long pos = outs.tellp();
    //cout<<"pos in write :record "<<pos<<endl;
    outs.write(&record[0][0], recordsize());
    return (pos/recordsize());
}
long Record::read(fstream &ins, long recno){
    long pos= recno * recordsize();
    ins.seekg(pos, ios_base::beg);
    ins.read(&record[0][0], recordsize());
    return ins.gcount();
}
ostream& operator<<(ostream& outs,
                    const Record& r){
//    for(int i  = 0 ;r.record[i][0] != NULL ; i++){
//        outs<<r.record[i]<<setw(20)<<" ";
//    }
    outs<<"|"; outs << setiosflags(ios::left);
    for( int i = 0 ; i < r.size ; i++){
        outs << setw(12) << setfill(' ') <<r.record[i];
    }
    outs << resetiosflags(ios::left); outs << "|";
    return outs;
}

long Record::recordsize(){
    return MAX*MAX;
}

//if u find mistakje in record count, switch recordsize to sizeof(record);
