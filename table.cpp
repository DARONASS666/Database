#include "table.h"
Table::Table(){
    table_name  = " ";
    record_count = 0;
    //cout<<"default Ctor fired.."<<endl;
}
const string fileextension = ".txt";
const string binextension = ".bin";
Table::Table(string tablename){
    table_name = tablename;
    field_size = 0;
    initrecordcount();
    int i = 0 ;
    //LOAD ALL THE FIELDNAMES FROM THE FILE AND PUT IT INTO YOUR MAP OF FIELDLISTS
    ifstream file;
    file.open((table_name + fileextension).c_str());
    if(file.is_open()){
        for(i = 0 ;!file.eof(); i++){
            string s;
            getline(file,s);
            if(file.eof()) break;
            field_list.insert(s,i);
            field_size++;
        }
        file.close();
    }
    else
        cout<<"Unable to Open File: "<<table_name + "fieldlists.txt"<<endl;
    cout<<"number of records:"<<record_count<<endl;
    cout<<"number of fieldnames:" <<field_size<<endl;
    reindex();
}

Table::Table(string tablename, vector<string> fieldlist):table_name(tablename), record_count(0)
{
    field_size = fieldlist.size();
    //open a readable and writable file called file with table_name as the files name with a txt
    //but convert string name into char array first
    for(unsigned int i = 0 ; i < fieldlist.size() ; i++){
        field_list[fieldlist [ i ] ] = i;
    }
    
    //refers to file for fieldlists of this table
    ofstream out;
    out.open((table_name + fileextension).c_str());
    if(out.is_open()){
        for(int i  = 0 ; i <fieldlist.size() ; i ++)
            out<<fieldlist[i]<<endl;
        out.close();
    }
    else
        cout<<"Unable to Open File: "<<table_name + fileextension<<endl;
}

void Table::insert(vector<string> v)
{
    //make a file and a record of vectors
    fstream f;
    Record list(v);
    //open the file and assign name of table
    open_fileW(f,(table_name + binextension).c_str());
    
    f.seekg(0, f.end);
    long index = list.write(f);
    cout<<"index in insert: "<<index<<endl;
    //indices at i becomes a multimap, that multimap at your vector of strings at i gives a string
    //that string becomes a key for your multimap and has a vector of longs and update rc there
    for(int i = 0 ; i < v.size() ; i++){
        indices[ i ][ v [ i ] ] += index;
    }
    //increment record count everytime you insert
    record_count++;
    //close file when you finish
    f.close();
}
void Table::show() const{
    fstream f;
    open_fileRW(f,(table_name + binextension).c_str());
    Record r(field_size);
    Map<string,long> tempmap = field_list;
    Map<long,string> temptemp;
    printline();
    cout<<"| Fields:";
    //int count = 0;
    for(Map<string,long>::Iterator it = tempmap.begin() ; it != tempmap.end() ; it++)
    {
        temptemp[ (*it).value ] = (*it).key;
        //count++;
    }
    for( int i = 0 ; i < temptemp.size() ; i++){
        cout << setw(12) << setfill(' ')<<temptemp[i];
    }
    //cout<<"You have iterated "<<count<<" times"<<endl;
    cout<<endl; printline();
    for(int record_pos  = 0 ; record_pos <record_count ; record_pos++){
        r.read(f , record_pos );
        cout << setw(17) << setfill(' ')<<r<<endl;
    }
    printline();
}

Table Table::select(string newtable,const vector<string> & fieldlist,string fieldname , string operation , string value){
    Table tbl(newtable,fieldlist);
    vector<long> recpos;
    fstream file;

    long index =  field_list[fieldname];
    MMap<string,long> tempmap = indices[index];
    open_fileRW(file ,(table_name + binextension).c_str());
    
    //if theyre equal then get the ones that are
    //exactly equal to the value that you passed in
    if(operation == "=" || operation == "=="){
        recpos = indices[index][value];
    }
    
    if(operation == ">"){
        //get the upper bound, go from upper bound to end
        for(MMap<string,long>::Iterator it = tempmap.upper_bound(value); it != tempmap.end() ; it++){
            // cout<<endl<<"iterator now pointing to [["<<(*it).key<<"]]"<<endl;
            recpos += (*it).value_list;
        }
        cout<<"passed iterator shit for >"<<endl;
    }
    if(operation == "<"){
        //start from zero and go to lower bound
        for(MMap<string,long>::Iterator it = tempmap.begin(); it != tempmap.lower_bound(value) ; it++){
            // cout<<endl<<"iterator now pointing to [["<<(*it).key<<"]]"<<endl;
            recpos += (*it).value_list;
        }
    }
    if(operation == ">="){
        //go from equal range to end
        for(MMap<string,long>::Iterator it = tempmap.lower_bound(value); it != tempmap.end() ; it++){
            //  cout<<endl<<"iterator now pointing to [["<<(*it).key<<"]]"<<endl;
            recpos += (*it).value_list;
        }
    }
    if(operation == "<="){
        //go from begin to equal range
        for(MMap<string,long>::Iterator it = tempmap.begin(); it != tempmap.upper_bound(value) ; it++){
            //  cout<<endl<<"iterator now pointing to [["<<(*it).key<<"]]"<<endl;
            recpos += (*it).value_list;
        }
    }
    
    //get the proper fields and proper bounds and read the records and store them into a string,
    //push that string into a vector and insert that vector into your temporary table
    for(int i = 0 ; i < recpos.size() ; i++)
    {
        Record r;
        r.read(file,recpos[ i ] );
        vector<string> temp;
        
        for(int j = 0 ; j <fieldlist.size() ; j++){
            string  in = r.get_row(field_list[fieldlist[ j ] ]);
            temp.push_back( in );
        }
        tbl.insert(temp);
    }
    
    //select function should create a new table class and have the same field list as th og one
    //select returns a table, stuff gets stored in a file. table deals wit files.
    //opening the file, going through the file, select whatever records u need one by one , and insert into new
    //table using the insert function.
    file.close();
    cout<<"----- select results: -------"<<endl;
    tbl.show();
    cout<<"-----------------------------"<<endl;
    return tbl;
}

Table Table::select(string newtable,vector<string> fieldlist){
    Table tbl(newtable,fieldlist);
    fstream file;
    open_fileRW(file ,(table_name+binextension).c_str());
    
    int i = 0;
    while (true){
        Record r;
        r.read(file, i );
        if(file.eof()) break;
        vector<string> temp;
        for(int j = 0 ; j <fieldlist.size() ; j++ ){
            string  in = r.get_row(field_list[fieldlist[ j ] ]);
            temp.push_back( in );
        }
        tbl.insert(temp);
        i++;
    }
    file.close();
    cout<<"----- select results: -------"<<endl;
    tbl.show();
    cout<<"-----------------------------"<<endl;
    return tbl;
}

//from database you alrady have the name, all you need to do on top of that is save fieldnames
void Table::store(vector<string> fieldlist){
    //must store every record, basically every thing indices
    
    
    //this stores the fieldnames of the file
    ofstream out;
    out.open((table_name + fileextension).c_str());
    if(out.is_open()){
        for(int i  = 0 ; i <fieldlist.size() ; i ++)
            out<<fieldlist[i]<<endl;
        out.close();
    }
    else
        cout<<"Unable to Open File: "<<table_name + "fieldlists.txt"<<endl;
}

//because you have to specify from database which table u wanna load all u need is name
void Table::load(string tablename){
    //must also load all the shit from indices and allocate space for indices
    if(file_exists((tablename + ".bin").c_str()))
        reindex();
    else
        cout<<"Unable to Open File: "<<table_name<<endl;
    
    //LOAD ALL THE FIELDNAMES FROM THE FILE AND PUT IT INTO YOUR MAP OF FIELDLISTS
    ifstream file;
    file.open((table_name + fileextension).c_str());
    if(file.is_open()){
        for(int i = 0 ;!file.eof(); i++){
            string s;
            getline(file,s);
            if(file.eof()) break;
            field_list.insert(s,i);
        }
        file.close();
    }
    else
        cout<<"Unable to Open File: "<<table_name + fileextension<<endl;
}
void Table::reindex(){
    fstream iff;
    // iff.open((table_name+binextension).c_str());
    open_fileRW(iff,(table_name+binextension).c_str());
    int i = 0 ;
    while(true){
        Record r;
        r.read(iff,i);
        if(iff.eof()) break;
        //cout<<"FIELDLIST MAP SIZE: "<<field_size<<endl;
        for(int j = 0 ; j < field_size; j++)
        {
            indices[ j ][ r.get_row( j ) ] += i ;
        }
        i++;
    }
    //cout<<"ENTERING PRINT MULTIMAP VALUES IN REINDEX"<<endl;
    //show_multimap_values();
    iff.close();
}
void Table::show_multimap_values(){
    cout<<"indices size:"<<field_size<<endl;
    for(int i = 0 ; i < field_size ; i++){
        cout<<"indices at: "<<i<<endl;
        MMap<string,long> temp = indices[i];
        temp.print_iterator();
        cout<<endl;
    }
}
void Table::initrecordcount(){
    fstream f(table_name + binextension); Record r;
    if(f.fail()) open_fileR( f, (table_name +binextension).c_str());
    f.seekg( 0 , f.end );
    
    record_count = f.tellp() / r.recordsize() ;
}
void Table::printline() const{
    for( int i = 0 ; i < (field_size)*16; i++){
        cout<<"=";
    }
    cout<<endl;
}

//everytime i exit a file and reload it after i recompile , indices mmap will s
