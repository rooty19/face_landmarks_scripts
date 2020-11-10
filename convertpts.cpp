#include <iostream>
#include <fstream> 
#include <string>
#include <iomanip>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::hex;
using std::vector;
using std::istringstream;
using std::__cxx11::to_string;

void showman(){
    cout << "Convert Linear Point data to Binary." << endl;
    cout << "Usage: $./convertpts [filelist] [input folder] [output folder] [32(float)/64(double)]" << endl;
    cout << "<Headers>(Little Endian)" << endl;
    cout << "0x00-0x03: float/double" << endl;
    cout << "0x04-0x07: nr_class" << endl;
    cout << "0x08-0x0B: nr_feature" << endl;
    cout << "0x0C-0x0f: bias" << endl;
    cout << "0x10~    : point data" << endl;
}

bool mkf(const char* foldername){
    struct stat st;
    int ret = stat(foldername, &st);
    if(ret != 0){
        mkdir(foldername, 0755);
        cout << "mkdir " << foldername << ": 0755" << endl; 
        return true;
    } else {
        cout << foldername << " is exist. (aborded)" << endl;
        return false;
    }
}

void fileread(vector<string>& strv, string& infile){
    ifstream fin;
    fin.open(infile);
    strv.resize(0);
    string rawline, spilitword;
    vector<string> spilitline;
    if(fin.fail()){
        cout << "Can't find file" << endl;
        strv.push_back("NOT FOUND");
        return;
    }
    while(fin >> rawline){
        istringstream iss(rawline);
        while(iss >> spilitword){
            strv.push_back(spilitword);
        }
    }
}

int main(int argv, char* argc[]){
    string msg;
    vector<string> flist, plist;
    ofstream fout;
    if(argv != 5){
        showman();
        return -1;
    }
    if(!(atoi(argc[4]) == 32 || atoi(argc[4]) == 64)){
        showman();
        return -1;
    }
    string infile = string(argc[1]);
    string inprefix = string(argc[2]);
    string outprefix = string(argc[3]);
    unsigned int data_bit = atoi(argc[4]);
    if(inprefix.at(inprefix.size()-1) == '/') inprefix.resize(inprefix.size()-1);
    if(outprefix.at(outprefix.size()-1) == '/') outprefix.resize(outprefix.size()-1);
    fileread(flist, infile);
    if(!mkf(outprefix.c_str())) return -1;
    for(auto i=0;i<flist.size();i++){
        //int i=1;
        string intname = inprefix+"/"+flist.at(i);
        flist.at(i).erase(flist.at(i).end()-4, flist.at(i).end());
        string outname = outprefix+"/"+flist.at(i)+".binp";
        fileread(plist, intname); 
        if(plist.at(0) != "NOT FOUND"){
            msg = "found";
        } else msg = "not found";
        cout << intname << " -> " << outname << " : " << msg << endl;
        if(msg == "not found") continue;
        //generate Header
        //unsigned int data_bit = 32;
        unsigned int nr_class = stoi(plist.at(3));
        unsigned int nr_feature = stoi(plist.at(5));
        unsigned int bias = stoi(plist.at(7));
        fout.open(outname, std::ios::out|std::ios::binary|std::ios::trunc);
        fout.write((char*)&data_bit,   sizeof(int));
        fout.write((char*)&nr_class,   sizeof(int));
        fout.write((char*)&nr_feature, sizeof(int));
        fout.write((char*)&bias,       sizeof(int));        
        plist.erase(plist.begin(), plist.begin()+9);
        union {double unid; uint64_t unii;} uniit;
        union {float  unif; uint32_t unii;} uniit32;
        for(auto j=0; j<plist.size(); j++) {
            if(data_bit == 64){
                uniit.unid = stod(plist.at(j));
                fout.write((char*)&uniit.unii, sizeof(uint64_t));
            } else {
                uniit32.unif = stof(plist.at(j));
                fout.write((char*)&uniit32.unii, sizeof(uint32_t));
            }
        }
        fout.close();
    }
}
