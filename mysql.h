#ifndef MYSQL_H
#define MYSQL_H
#include "mmap.h"
#include "sql_parser.h"
#include "table.h"
#include <cstring>
#include <iostream>
#include <vector>
#include <stdio.h>
using namespace std;


class MySQL
{
public:
    MySQL();
    void run();
    string selectname();
    bool handle_errors(vector<string> tn, vector<string> fl, vector<string> cl,
                       string command1, string command2);
    bool getLine();


private:
    string line;
    MMap<string,string> mymap;
    Map<string,Table> tables;
    bool tablecontainsallfields(string tablename, vector<string> fl, int &j, string command1, string command2);
};

#endif // MYSQL_H
