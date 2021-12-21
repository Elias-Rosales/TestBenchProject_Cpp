#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <regex>
#include "TBGetData.h"

TBGetData::TBGetData(string _File, string _conf){
    TBGetData::File = _File;
    TBGetData::conf = _conf;
    config();
    inputs();
    outputs();
    module();
    inst_modules();
}
TBGetData::~TBGetData(){}
void TBGetData::config(){
    ifstream arc_conf(TBGetData::conf.c_str());
    regex pttrn_timescale ("^\\s*timescale\\s*=\\s*(\\w+\\/\\w+).*$");
    regex pttrn_delay ("^\\s*delay\\s*=\\s*(\\#\\d+).*$");
    regex pttrn_dumpvars ("^\\s*dumpvars\\s*=\\s*(\\d+).*$");
    regex pttrn_cases ("^\\s*num_cases\\s*=\\s*(\\d+).*$");
    while(getline(arc_conf,TBGetData::linea)){
        if(regex_search(TBGetData::linea, TBGetData::sm, pttrn_timescale)){
            TBGetData::timescale = TBGetData::sm[1].str();
        }
        if(regex_search(TBGetData::linea, TBGetData::sm, pttrn_delay)){
            TBGetData::delay = TBGetData::sm[1].str();
        }
        if(regex_search(TBGetData::linea, TBGetData::sm, pttrn_dumpvars)){
            TBGetData::dumpvars = TBGetData::sm[1].str();
        }
        if(regex_search(TBGetData::linea, TBGetData::sm, pttrn_cases)){
            TBGetData::num_cases = stoi(TBGetData::sm[1].str());
        }
    }
    //Variable test
    /*cout << timescale << endl;
    cout << delay << endl;
    cout << dumpvars << endl;
    cout << num_cases << endl;*/
}

void TBGetData::inputs(){
    ifstream archivo(TBGetData::File.c_str());
    regex pttrn_in_bus ("^\\s*input\\s*\\[\\s*(\\d+)\\s*:.*]\\s*(\\w+,?.*)[;,]"); 
    regex pttrn_in_nbus ("^\\s*input\\s*(\\w+,?.*)[;,]");  
    regex simple ("(\\w+)");
    while(getline(archivo,TBGetData::linea)){
        if(regex_search(TBGetData::linea, TBGetData::sm,pttrn_in_nbus)){
            TBGetData::tempo = TBGetData::sm[1].str();
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::list_input.push_back(match_str);
                TBGetData::list_input_size.push_back(1);
            }
        }
        if(regex_search(TBGetData::linea, TBGetData::sm,pttrn_in_bus)){
            TBGetData::tempo = TBGetData::sm[2].str();
            regex_search(linea, nums, regex("\\d"));
            TBGetData::dim = TBGetData::sm[1];
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::list_input.push_back(match_str);
                TBGetData::list_input_size.push_back(stoi(dim)+1);
            }
        }
    }
    /*Variable test
    for (auto it = begin(list_input); it!=end(list_input); ++it)
        cout << ' ' << *it;
    cout << endl;
    for (auto it = begin(list_input_size); it!=end(list_input_size); ++it)
        cout << ' ' << *it;
    cout << endl;*/
}
void TBGetData::outputs(){
    ifstream archivo(TBGetData::File.c_str());
    regex pttrn_bus_no_reg ("^\\s*output\\s*\\[\\s*(\\d+)\\s*:.*]\\s*(\\w+,?.*)[;,]"); // output [1:0] q,
    regex pttrn_reg ("^\\s*output\\s*reg?\\s*(\\[\\s*\\d+\\s*:\\s*\\d+\\s*\\])?\\s+(.*)[;,]"); // output reg out, output reg [1:0] ste,
    regex pttrn_nobus ("^\\s*output\\s*(?!reg)(\\w+.*)[;,]");  // output u, v, w, output b,
    regex simple ("(\\w+)");
    while (getline(archivo,TBGetData::linea)){
        if (regex_search(TBGetData::linea, TBGetData::sm, pttrn_nobus)){
            TBGetData::tempo = TBGetData::sm[1].str();
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::list_output.push_back(match_str);
                TBGetData::list_output_size.push_back(1);
            }
        }
        if (regex_search(TBGetData::linea, TBGetData::sm, pttrn_reg)){
            TBGetData::tempo = TBGetData::sm[2].str();
            regex_search(TBGetData::linea, TBGetData::nums, regex("\\d"));
            TBGetData::dim = TBGetData::nums.str();
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                if (dim.empty()){
                    TBGetData::list_output.push_back(match_str);
                    TBGetData::list_output_size.push_back(1);
                }else{
                    TBGetData::list_output.push_back(match_str);
                    TBGetData::list_output_size.push_back(stoi(dim)+1);
                }
            }
        }   
        if (regex_search(TBGetData::linea, TBGetData::sm, pttrn_bus_no_reg)){
            TBGetData::tempo = TBGetData::sm[2].str();
            regex_search(TBGetData::linea, TBGetData::nums, regex("\\d"));
            TBGetData::dim = TBGetData::sm[1];
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::list_output.push_back(match_str);
                TBGetData::list_output_size.push_back(stoi(dim)+1);
            }
        }   
    }
    /*Variable test
    for (auto it = begin(list_output); it!=end(list_output); ++it)
        cout << ' ' << *it;
        cout << endl;
    for (auto it = begin(list_output_size); it!=end(list_output_size); ++it)
        cout << ' ' << *it;
        cout << endl;*/
}
void TBGetData::module(){
    ifstream archivo(TBGetData::File.c_str());
    regex pattern_module_name ("^module\\s+(\\w+)");
    regex simple ("(\\w+)");
    while (getline(archivo,TBGetData::linea)){    
        if (regex_search(TBGetData::linea, TBGetData::sm, pattern_module_name)){
            TBGetData::tempo = TBGetData::sm[1].str();
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::module_name_v.push_back(match_str);
            }
        }
    }
    TBGetData::module_name = TBGetData::module_name_v[0];
    //Variable test
    //cout << module_name << endl;
}
void TBGetData::inst_modules(){
    ifstream archivo(TBGetData::File.c_str());
    regex pattern_module_instances ("^\\s*(?!module)(\\w+)\\s*\\w+\\s*\\(.+\\);");
    regex simple ("(\\w+)");
    while (getline(archivo,TBGetData::linea)){    
        if (regex_search(TBGetData::linea, TBGetData::sm, pattern_module_instances)){
            TBGetData::tempo = TBGetData::sm[1].str();
            auto words_begin = sregex_iterator(tempo.begin(), tempo.end(), simple);
            auto words_end = sregex_iterator();
            for (sregex_iterator i = words_begin; i != words_end; ++i) {
                smatch match = *i;
                string match_str = match.str();
                TBGetData::module_instances.push_back(match_str);
            }
        }
    }
    sort(module_instances.begin(), module_instances.end() );
    module_instances.erase( unique( module_instances.begin(), module_instances.end() ), module_instances.end() );
    /*Variable test
    for (auto it = begin(module_instances); it!=end(module_instances); ++it)
        cout << ' ' << *it;
    cout << endl;*/
}
//
vector<string> TBGetData::get_header(void){
    //return [timescale,module_name,dumpvars,delay]
    vector<string> gethead;
    gethead.push_back(timescale);
    gethead.push_back(module_name);
    gethead.push_back(dumpvars);
    gethead.push_back(delay);
    return gethead;
}
vector<string> TBGetData::get_instmods(void){
    return TBGetData::module_instances;
}
vector<string> TBGetData::get_lin(void){
    return TBGetData::list_input;
}
vector<string> TBGetData::get_lout(void){
    return TBGetData::list_output;
}
vector<int> TBGetData::get_lin_size(void){
    return TBGetData::list_input_size;
}
vector<int> TBGetData::get_lout_size(void){
    return TBGetData::list_output_size;
}
int TBGetData::get_numcases(){
    return TBGetData::num_cases;
}