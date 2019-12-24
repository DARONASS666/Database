#include "mysql.h"

MySQL::MySQL()
{

}
bool MySQL::getLine(){
    cout<<"Command: ";
    getline(cin,line);
    if(line == "cls") system("cls");
    return (line != "exit");
}
bool MySQL::tablecontainsallfields(string tablename, vector<string> fl ,
                                   int & j,string command1 , string command2){
    int errors = 0;
    for(int i = 0 ; i < fl.size() ; i ++){
        if(!tables[tablename].get_field_list().contains(fl[ i ])){
            j = i;
            cout<<command2<<fl[i]<<" does not exist in "<<command1<<" "<<tablename<<endl;
            errors++;
        }
    }
    return (errors == 0);
}

bool MySQL::handle_errors(vector<string> tn,vector<string> fl,vector<string> cl,
                          string command1 , string command2){

    const bool debug = false;
    int errorcount  = 0;
    int j = 0;
    for(int i = 0 ; i <tn.size() ; i++){
        if(!file_exists((tn[i]+".bin").c_str())){
            cout<<command1<<tn[i]<<" does not exist in directory."<<endl;
            errorcount++;
        }
        else
        {
            if(!tablecontainsallfields(tn[i],fl,j,command1,command2))  errorcount++;
            if(!tablecontainsallfields(tn[i],cl,j,command1,command2))  errorcount++;
        }
    }
    if (debug) cout<<"err"<<errorcount<<endl;
    return ( errorcount == 0 );
}

//this runs and takes in a sentence parses into commands and sends them into table
void MySQL::run(){
    while(getLine()){
        sql_parser parse(line.c_str());
        if( parse.parse_tree(mymap))
        {
            //cout<<"==================="<<endl;
            // mymap.print_iterator();
            // cout<<"==================="<<endl;
            //if command is insert do insert on table
            if(mymap[COMMAND][0] == "insert")
            {
                string filename = mymap[TABLENAMEE ][ 0 ];
                if(tables.contains(filename))
                {
                    if(tables[filename].get_field_size() == mymap[VALUE].size())
                        tables[filename].insert(mymap[VALUE]);
                    else
                        cout<<"Table: "<<filename<<" does not have "<<mymap[VALUE].size()<<" fields"<<endl;
                }
                else
                    cout<<"Table: "<<mymap[TABLENAMEE][0]<<" does not exist in directory"<<endl;
            }
            if(mymap[COMMAND][0] == "create")
            {
                tables[mymap[TABLENAMEE ][ 0 ] ] =
                        Table(mymap[TABLENAMEE][0] , mymap[FIELDLIST]);
            }
            if(mymap[COMMAND][0] == "select")
            {
                if(handle_errors(mymap[TABLENAMEE] , mymap[FIELDLIST],mymap[COLUMN]
                                 ,"Table: " ,"Fieldname: ")){
                    if(mymap[WHEREE][0] == YES){
                        for( int i  = 0 ; i < mymap[TABLENAMEE].size() ; i++){
                            string filename = selectname();
                            tables[filename] = tables[mymap[TABLENAMEE][i]].
                                    select(filename,mymap[FIELDLIST],mymap[COLUMN][0],
                                    mymap[OP][0],mymap[VALUE][0]);
                        }
                    }
                    else if (mymap[WHEREE][0] == NO){
                        for( int i  = 0 ; i < mymap[TABLENAMEE].size() ; i++){
                            string filename = selectname();
                            tables[filename] = tables[mymap[TABLENAMEE][i]].select(filename,mymap[FIELDLIST]);
                        }
                    }
                }
                else{
                    cout<<"Mismatched fields/tables..."<<endl;
                }
            }
            if(mymap[COMMAND][0] == "store")
            {

            }
            if(mymap[COMMAND][0] == "load")
            {
                string filename = mymap[TABLENAMEE][0];
                if(handle_errors(mymap[TABLENAMEE] ,mymap[FIELDLIST],
                                 mymap[COLUMN],"Table: " , "Fieldname: "))
                {
                    //tables.insert(filename,Table(filename));
                    // if(!tables.contains(filename))
                    tables[filename] =Table(filename);
                }
                else cout<<"File: "<<filename<<" does not exist in directory."<<endl;
            }
            if(mymap[COMMAND][0] == "print")
            {
                if(tables.contains(mymap[TABLENAMEE][0]))
                    tables[mymap[TABLENAMEE][0]].show();
                else
                    cout<<"Table: "<<mymap[TABLENAMEE][0]<<" does not exist in directory"<<endl;
            }
            if(mymap[COMMAND][0] == "drop")
            {
                //then delete the table
                if(handle_errors(mymap[TABLENAMEE] , mymap[FIELDLIST] ,
                                 mymap[COLUMN],"Table: " ,"Fieldname: ")){
                    string command;
                    string filename = mymap[TABLENAMEE][0];
                    cout<<"Are you sure you want to "
                          "drop file: \""<<filename<<"\" from database?"<<endl;
                    cout<<"All data written in file wil be lost..."<<endl;
                    cout<<"Input yes to continue.."<<endl;
                    cout<<">"; cin>>command;
                    if(command == "yes")
                    {
                        tables.erase(filename);
                        if( remove( (filename+".bin").c_str() ) != 0 )
                            perror( "Error deleting file: " );
                        else
                        {
                            puts( "File successfully deleted" );
                        }
                        if( remove( (filename+".txt").c_str() ) != 0 )
                            perror( "Error deleting file: " );
                        else
                        {
                            puts( "File successfully deleted" );
                        }
                    }
                    else cout<<"File: "<<filename<<" was not dropped from the database."<<endl;
                }
            }
            if(mymap[COMMAND][0] == "help"){
                cout<<"================================================================================"<<endl;
                cout<<"List of commands and how to use them:"<<endl<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"cls.  description: clears terminal screen."<<endl<<endl;
                cout<<"exit. description: exits the terminal"<<endl<<endl;
                cout<<"help. description: displays information about existing commands"<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"1. create table <tablename> <fieldname01> <type01> , <fieldname02> <type02> ..."<<endl<<endl;
                cout<<"description: creates a table with multiple fields."<<endl<<endl;
                cout<<"available types: integer/int , string , float"<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"2. insert into <tablename> values <value01> , <value02> ..."<<endl<<endl;
                cout<<"inserts values into an existing table. values must be in order of fields."<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"3. select <fieldname01> , <fieldname02> ... from <tablename01> , <tablename02> ..."<<endl<<endl
                   <<"where <fieldname01> <condition(>,<,=,>=,<=)> <value01> and <fieldname02>... "<<endl<<endl;
                cout<<"description: select values from multiple table with multiple conditions"<<endl<<endl
                   <<"and store into a new table."<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"4. store <tablename>"<<endl<<endl;
                cout<<"description: saves a table into file directory."<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"5. load <tablename>"<<endl<<endl;
                cout<<"description: load an existing tablename from file directory."<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"6. print <tablename>"<<endl<<endl;
                cout<<"description: prints an existing table."<<endl;
                cout<<"================================================================================"<<endl;
                cout<<"7. drop <tablename>"<<endl<<endl;
                cout<<"description: deletes an existing table from file directory and memory."<<endl;
                cout<<"================================================================================"<<endl;

            }
        }
        else if(line != "")
            cout<<"INVALID COMMAND"<<endl;
    }
}


string MySQL::selectname(){
    string out;
    cout<<"Enter new table name:"<<endl;
    cin>>out;
    while(tables.contains(out)){
        cout<<"Table: \""<<out<<"\"  already exists in the database."<<endl;
        cout<<"Please enter a different table name:"<<endl;
        cin>>out;
    }
    return out;
}
