#ifndef TBGETDATA_H
#define TBGETDATA_H
//GET DATA CLASS
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
using namespace std;

class TBGetData{
    private:
        string File;
        string conf;
        string linea;
        smatch sm;
        smatch nums;
        smatch sm_simple;
        string tempo;
        string dim;
        vector<string> list_input;
        vector<int> list_input_size;
        vector<string> list_output;
        vector<int> list_output_size;
        vector<string> module_instances;
        vector<string> module_name_v;
        //vector<string> gethead;
        string timescale;
        string delay;
        string dumpvars;
        string module_name;
        int num_cases;
    void config();
    void inputs();
    void outputs();
    void module();
    void inst_modules();
    
    public:
    /*  Constructor */
    TBGetData(string _File, string _conf);
    /*  Destructor */
    ~TBGetData();
    /*  Member Functions */
    vector<string> get_header(void);
    vector<string> get_instmods(void);
    vector<string> get_lin(void);
    vector<string> get_lout(void);
    vector<int> get_lin_size(void);
    vector<int> get_lout_size(void);
    int get_numcases();
};

#endif