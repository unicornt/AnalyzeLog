#include <bits/stdc++.h>
using namespace std;
const string null_str = "";
const int func_num = 10000;
enum {init, marked, demarked, compiling, optimizing};
map<string, int>sfiTable;
int status[func_num];
vector<int>slist[func_num]; // list of status
vector<string>rlist[func_num]; // list of reason(if have)
vector<int>rtlist[func_num]; // list of times of running

//////////////////////////////////////////////////////////

void AnalyzeCode(string s) {

}

string GetSFI(string s) {
    return null_str;
}

void NewSFI(string sfi) {
    sfiTable[sfi] = sfiTable.size();
    status[sfiTable[sfi]] = init;
}

void ChangeStatus(int sn, int news) {
    status[sn] = news;

}

int CheckTar(string s) {
    return optimizing;
}


void Analyze(string s) {
    if(s[0] != '[') AnalyzeCode(s);
    else {
        switch (s[1])
        {
        // after change status, calcute running times (need add function to v8 source code)
        case 'm':
            // marking XXXXXXX for optimized recompilation
            // marking XXXXXXX for deoptimization
            // get sfi of the function & save it if never meet it before init it as "init"
            // mark it as "marking"
            // deoptimization: get last optimized
            string sfi = GetSFI(s);
            if(sfi == null_str) {
                // error
                cout << "error: no sfi" << std::endl;
                exit(1);
            }
            if(sfiTable.find(sfi) == sfiTable.end()) {
                // new sfi
                NewSFI(sfi);
            }
            int sn = sfiTable[sfi];
            // mark it as "marked/demarked"
            ChangeStatus(sn, CheckTar(s));
            break;
        case 'c':
            //compiling method XXXXXXX using TurboFan OSR
            // get sfi of the function & mark it as "compiling"
            // completed optimizing XXXX
            // mark it as "optimized"
            break;
        case 'o':
            // optimizing 
            // mark it as "optimizing"
            break;
        case 'b':
            // bailout 
            // get reason & record it
            // mark it as "init"
            break;
        default:
            break;
        }
    }
}
int main() {
    ifstream fp;
    fp.open("log/log.txt", ios::in);
    string ls;
    while(!fp.eof() && fp.good()) {
        getline(fp, ls);
        Analyze(ls);
    }
    fp.close();
    return 0;
}