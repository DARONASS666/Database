#include "record_utility_functions.h"
bool file_exists(const char filename[]){
    const bool debug = false;
    fstream ff;
    ff.open (filename,
             std::fstream::in | std::fstream::binary );
    if (ff.fail()){
        if (debug) cout<<"File does NOT exist: "<<filename<<endl;
        return false;
    }
        if (debug) cout<<"File DOES exist: "<<filename<<endl;
    return true;
}

void open_fileRW(fstream& f, const char filename[]) throw(char*){
    const bool debug = false;
    //openning a nonexistent file for in|out|app causes a fail()
    //  so, if the file does not exist, create it by openning it for
    //  output:
    if (!file_exists(filename)){
        f.open(filename, std::fstream::out|std::fstream::binary);
        if (f.fail()){
            cout<<"file open (RW) failed: with out|"<<filename<<"]"<<endl;
            throw("file RW failed  ");
        }
        else{
            if (debug) cout<<"open_fileRW: file created successfully: "<<filename<<endl;
        }
    }
    else{
        f.open (filename,
                std::fstream::in | std::fstream::out| std::fstream::binary );
        if (f.fail()){
            cout<<"file open (RW) failed. ["<<filename<<"]"<<endl;
            throw("file failed to open.");
        }
    }

}
//a writeable file that appends to end of file
void open_fileW(fstream& f, const char filename[]){
    f.open (filename, std::ios_base::app | std::fstream::out| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
//a readable file that reads stuff from the file
void open_fileR(fstream& f, const char filename[]){
    f.open (filename, std::fstream::in| std::fstream::binary );
    if (f.fail()){
        cout<<"file open failed: "<<filename<<endl;
        throw("file failed to open.");
    }

}
//do not use this in any fucntions, this is used only to show how shit are written
void save_list(Record list[], int count){
    fstream f;
    open_fileW(f, "record_list.bin");
    for (int i= 0; i<count; i++){
        list[i].write(f);
    }
    f.close();
}
