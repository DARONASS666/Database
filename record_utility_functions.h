#ifndef RECORD_UTILITY_FUNCTIONS_H
#define RECORD_UTILITY_FUNCTIONS_H
#include "record.h"
//-----------------------------------------
//RECORD UTILITY FUNCTIONS SHALL BE
//USED TO DEAL WITH FILES, TO READ
//AND WRITE THEM, CHECK TO SEE
//IF THEY EXIST OR NOT
//-----------------------------------------
bool file_exists(const char filename[]);

void open_fileRW(fstream& f, const char filename[]) throw(char*);
void open_fileW(fstream& f, const char filename[]);
void open_fileR(fstream& f, const char filename[]);

void save_list(Record list[], int count);

//-----------------------------------------
#endif // RECORD_UTILITY_FUNCTIONS_H
