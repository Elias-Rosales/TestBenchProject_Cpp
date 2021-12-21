#ifndef WRITEDATA_H
#define WRITEDATA_H
//WRITE DATA CLASS
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
using namespace std;

class WriteData{
    private:
        string module_name,timescale,dumpvars,delay;
        vector<string> instantiated_modules,list_input,list_output;
        vector<int> list_input_size,list_output_size;
        int num_mod,num_in,num_out,num_cases;
        ofstream TB;
        bool flag1;
    public:
    /*  Constructor  */
        WriteData(string);
    /*  Destructor  */
        ~WriteData();
    /*  Member Functions  */
        void header(string,vector<string>,string);
        void in_out(vector<int>,vector<string>,vector<int>,vector<string>);
        void dut();
        void initialbegin(string,int,string);
        string dectobin(int,int);
};

#endif