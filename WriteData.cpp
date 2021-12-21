#include "WriteData.h"
#include <algorithm>
#include <math.h>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

WriteData::WriteData(string output_file){
    TB.open(output_file);
}
WriteData::~WriteData(){}
void WriteData::header(string _timescale, vector<string> _instantiated_modules, string _module_name){
    timescale = _timescale;
    instantiated_modules = _instantiated_modules;
    module_name = _module_name;
    num_mod = instantiated_modules.size();

    TB << "`timescale " << timescale <<endl;
    if(num_mod!=0){
        for(int i = 0; i<num_mod;i++){
        TB << "`include \"" << instantiated_modules[i] << ".sv\"\n";
        }
    }
    TB << "\nmodule " << module_name << "_TB;\n";
    TB.flush();
}

void WriteData::in_out(vector<int> _in_s,vector<string> _in,vector<int> _out_s,vector<string> _out){
    list_input_size = _in_s;
    list_input = _in;
    list_output_size = _out_s;
    list_output = _out;
    int maxElement;
    //inputs
    maxElement = *max_element(list_input_size.begin(),list_input_size.end());
    vector<vector<string>> inputs;
    inputs.resize(maxElement);
    for(int i=0;i<list_input.size();i++){
        inputs[list_input_size[i]-1].push_back(list_input[i]);
    }
    
    for(int i=0;i<maxElement;i++){
        if(!inputs[i].empty()){
            if(i+1==1){
                TB << "  reg ";
                for(int j=0; j<inputs[i].size();j++){
                    if(j==inputs[i].size()-1){
                        TB << inputs[i][j] << "_TB;\n";
                    }
                    else TB << inputs[i][j] << "_TB, ";
                }
            }
            else{
                TB << "  reg [" << i << ":0] ";
                for(int j=0; j<inputs[i].size(); j++){
                    if(j==inputs[i].size()-1){
                        TB << inputs[i][j] << "_TB;\n";
                    }
                    else TB << inputs[i][j] << "_TB, ";
                }
            }
        }
    }

    //outputs
    maxElement = *max_element(list_output_size.begin(),list_output_size.end());
    vector<vector<string>> outputs;
    outputs.resize(maxElement);
    for(int i=0;i<list_output.size();i++){
        outputs[list_output_size[i]-1].push_back(list_output[i]);
    }
    for(int k=0;k<maxElement;k++){
        if(!outputs[k].empty()){
            if(k+1==1){
                TB << "  wire ";
                for(int j=0; j<outputs[k].size();j++){
                    if(j==outputs[k].size()-1){
                        TB << outputs[k][j] << "_TB;\n";
                    }
                    else 
                    TB << outputs[k][j] << "_TB, ";
                }
            }
            else{
                TB << "  wire [" << k << ":0] ";
                for(int j=0; j<outputs[k].size(); j++){
                    if(j==outputs[k].size()-1){
                        TB << outputs[k][j] << "_TB;\n";
                    }
                    else
                    TB << outputs[k][j] << "_TB, ";
                }
            }
        }
    }
    TB.flush();
}

void WriteData::dut(){
    num_in = list_input.size();
    num_out = list_output.size();
    flag1 = false;
    bool flag2 = false;

    TB << "\n" << module_name << " DUT(" ;

    if (num_in !=0){
        for (int i = 0; i < num_in; i++){
            if (i == num_in - 1){
                if(num_out!=0){
                    TB << "." << list_input[i] << "(" << list_input[i] << "_TB),\n            ";
                }
                else{
                    TB << "." << list_input[i] << "(" << list_input[i] << "_TB));" << endl;
                }
            }
            else{
                TB << "." << list_input[i] << "(" << list_input[i] << "_TB), " ;
            }
        }
    }
    else{
        flag1 = true;
    }

    if (num_out !=0){
        for (int i = 0; i < num_out; i++){
            if (i == num_out - 1){
                TB << "." << list_output[i] << "(" << list_output[i] << "_TB));" << endl;
            }
            else{
                TB << "." << list_output[i] << "(" << list_output[i] << "_TB), ";
            }
        }
    }
    else{
        flag2 = true;
    }

    if (flag1 && flag2){
        TB << ");" << endl;
    }
    TB.flush();
}

void WriteData::initialbegin(string _dump,int _cases,string _delay){
    dumpvars = _dump;
    num_cases = _cases;
    delay = _delay;
    TB << "\n  initial begin\n    $dumpfile(\"" << module_name << ".vcd\");\n    ";
    TB << "$dumpvars(" << dumpvars << "," << module_name << "_TB);\n\n";

    //number of signals
    int num_bits,cases_used;
    string in_vector;
    for(auto& n : list_input_size){
        num_bits += n;
    }
    
    int num_totcases = pow(2,num_bits);
    if(num_totcases>num_cases){
        cases_used = num_cases;}
    else cases_used = num_totcases; 
    if(!flag1){
        for(int i=0;i<num_in;i++){
            if(i==num_in-1) in_vector = in_vector+list_input[i]+"_TB";
            else in_vector = in_vector+list_input[i]+"_TB,";
        }
        //Case 0
        TB << "    {"+in_vector+"} = " << num_bits << "'b"+dectobin(num_bits,0)+"; " << delay << endl;
        srand(time(NULL)); //Initialize random seed
        int value;
        for(int i=0;i<cases_used-2;i++){
            value = rand() % (num_totcases-2) + 1; //get random number from 1 to num_totcases-2
            TB << "    {"+in_vector+"} = " << num_bits << "'b"+dectobin(num_bits,value)+"; " << delay << endl;
        }
        //Case max
        TB << "    {"+in_vector+"} = " << num_bits << "'b"+dectobin(num_bits,num_totcases-1)+"; " << delay << endl;
    }
    TB << "\n    $finish;\n  end\nendmodule";
    TB.close();
}

string WriteData::dectobin(int _bits,int _num){
    long i;
    string bin_num;
    for (i = 1 << _bits-1; i > 0; i = i / 2) //for 2^(bits-1), while i > 0
    {
      if((_num & i) != 0)   //check if i bit is 1 or 0
      {
        bin_num=bin_num+"1";
      }
      else
      {
        bin_num=bin_num+"0";
      }
    }
    return bin_num;
}