#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include "TBGetData.h"
#include "WriteData.h"
using namespace std;

int main(int argc, char** argv){
    // Entrada de argumentos
    cout << "********************** Testbench Generator **********************\n";
    string output_File = "testbench.sv";
    string conf = "conf.ini";
    string File = " ";
    smatch sv;

    if(argc < 2){
        cout << "\n[ERROR]: No argument has been declared\n";
        cout << "Use the following syntax: TB.exe [file.sv] [output.sv] [conf.ini]";
        return(EXIT_FAILURE);
    }
    else{
        File = argv[1];
        if (argc == 3)
            output_File = argv[2];
        else if (argc == 4){
            output_File = argv[2];
            conf = argv[3];
        }
    }
    if (!regex_search(File,sv,regex("^\\w+(.sv)$"))){
        cout << "[ERROR] InputFileName = " << File << endl;
        cout << "\tThe input file must have a .sv extension" <<endl;
        return(EXIT_FAILURE);
    }
    if (!regex_search(output_File,sv,regex("^\\w+(.sv)$"))){
        cout << "[ERROR] OuputFileName = " << output_File << endl;
        cout << "\tThe output file must have a .sv extension" <<endl;
        return(EXIT_FAILURE);
    }
    if (!regex_search(conf,sv,regex("^\\w+(.ini)$"))){
        cout << "[ERROR] ConfFileName = " << conf << endl;
        cout << "\tThe conf file must have a .ini extension" <<endl;
        return(EXIT_FAILURE);
    }
    //Objects construction
    TBGetData data(File, conf);
    WriteData writer(output_File);
    //Getting data from design
    vector<string> getHeader = data.get_header(); //timescale,module_name,dumpvars,delay
    vector<string> inst_mod = data.get_instmods();
    vector<string> inputs = data.get_lin(), outputs = data.get_lout();
    vector<int> in_s = data.get_lin_size(), out_s = data.get_lout_size();
    int num_cases = data.get_numcases();
    //Using data from design to write testbench
    writer.header(getHeader[0],inst_mod,getHeader[1]);
    writer.in_out(in_s,inputs,out_s,outputs);
    writer.dut();
    writer.initialbegin(getHeader[2],num_cases,getHeader[3]);
    //End Program
    cout << "\nPress a key to continue ...";
    getchar();
    cout << "....................................." << endl;
    cout << "Created on November 2021" << endl;
    cout << "Authors:" << endl;
    cout << "\tReyes Sanchez Luis Angel" << endl;
    cout << "\tRivera Orozco David" << endl;
    cout << "\tRosales Galindo Elias" << endl;
    cout << "\tTacuapan Moctezuma Edgar" << endl;
    return EXIT_SUCCESS;
}