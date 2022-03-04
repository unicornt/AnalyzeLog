#include <bits/stdc++.h>
using namespace std;
const string null_str = "";
const int func_num = 10000;
enum {init, marked, demarked, compiling, optimizing, optimized};
string stats[10] = {"init", "marked", "demarked", "compiling", "optimizing", "optimized"};
map<string, int>sfiTable;
int status[func_num];
vector<int>slist[func_num]; // list of status
vector<string>rlist[func_num]; // list of reason(if have)
vector<int>rtlist[func_num]; // list of times of running

//////////////////////////////////////////////////////////

void throw_error(string reason, string lines) {
    cout << "error: " << reason << std::endl;
    cout << "input string is " << lines << std::endl;
    exit(1);
}

void AnalyzeCode(string s) {

}

string GetSFI(string s) {
    string sfi = null_str;
    size_t p = s.find("sfi");
    if(p == string::npos) {
        // input string is [marking dependent code 0x003c00052481 (0x003c0833c449 <SharedFunctionInfo process>)
        p = s.find("0x");
        if(p == string::npos) return null_str;
        sfi = s.substr(p, 14);
        return sfi;
    }
    p += 3;
    while(s[p] == ' ' || s[p] == '=') p++;
    while(isalnum(s[p]) || s[p] == 'x') {
        sfi += s[p++];
    }
    int delt = 14 - sfi.length();
    if(delt > 0) {
        sfi = "0x" + string(delt, '0') + sfi.substr(2);
    }
    else if(delt < 0) {
        throw_error("delt < 0", s);
    }
    cout << "sfi " << sfi << std::endl;
    return sfi;
}

void NewSFI(string sfi) {
    sfiTable[sfi] = sfiTable.size();
    status[sfiTable[sfi]] = init;
}

void ChangeStatus(int sn, int news) {
    status[sn] = news;
    slist[sn].push_back(news);
}

void ChangeStatus(int sn, int news, string reason) {
    status[sn] = news;
    slist[sn].push_back(news);
    rlist[sn].push_back(reason);
}

int CheckTar(string s) {
    size_t p = s.find("optimized");
    if(p == string::npos) return demarked;
    return marked;
}

string GetReason(string s) {
    string reason = null_str;
    size_t p = s.find("reason");
    if(p == string::npos) {
        if(s.find("ERROR") != string::npos) return "ERROR";
        throw_error("no reason", s);
    }
    cout << p << std::endl;
    p += 8;
    while(isalpha(s[p]) || s[p] == ' ') {
        // cout << s[p] << endl;
        reason += s[p++];
    }
    return reason;
}


void Analyze(string s) {
    string sfi;
    int sn;
    if(s[0] != '[') AnalyzeCode(s);
    else {
        switch (s[1])
        {
        // after change status, calcute running times (need add function to v8 source code)
        case 'm':
            // marking XXXXXXX for optimized recompilation
            // marking XXXXXXX for deoptimization
            sfi = GetSFI(s);
            // get sfi of the function & save it if never meet it before init it as "init"
            // mark it as "marking"
            // deoptimization: get last optimized
            if(sfi == null_str) {
                // error
                throw_error("no sfi code", s);
                exit(1);
            }
            if(sfiTable.find(sfi) == sfiTable.end()) {
                // new sfi
                NewSFI(sfi);
            }
            sn = sfiTable[sfi];
            cout << sfi << ' ' << sn << std::endl;
            // mark it as "marked/demarked"
            ChangeStatus(sn, CheckTar(s), GetReason(s));
            break;
        case 'c':
            //compiling method XXXXXXX using TurboFan OSR
            // get sfi of the function & mark it as "compiling"
            sfi = GetSFI(s);
            if(sfi == null_str) {
                // error
                throw_error("no sfi code", s);
                exit(1);
            }
            sn = sfiTable[sfi];
            if(s[4] == 'i') {
                ChangeStatus(sn, compiling);
            }
            else {
                ChangeStatus(sn, optimized);
            }
            // completed optimizing XXXX
            // mark it as "optimized"
            break;
        case 'o':
            // optimizing 
            // mark it as "optimizing"
            sfi = GetSFI(s);
            if(sfi == null_str) {
                // error
                throw_error("no sfi code", s);
                exit(1);
            }
            sn = sfiTable[sfi];
            ChangeStatus(sn, optimizing);
            break;
        case 'b':
            // bailout 
            // record reason
            sfi = GetSFI(s);
            if(sfi == null_str) {
                // error
                throw_error("no sfi code", s);
                exit(1);
            }
            sn = sfiTable[sfi];
            ChangeStatus(sn, init, GetReason(s));
            break;
        default:
            cout << "unsupport sentence" << std::endl;
            cout << "    " << s.substr(0, 10) << "..." << std::endl;
            break;
        }
    }
}
int main() {
    ifstream fp;
    fp.open("log/log-with-bytecode.txt", ios::in);
    string ls;
    while(!fp.eof() && fp.good()) {
        getline(fp, ls);
        Analyze(ls);
    }
    fp.close();
    map<string, int>::iterator it;
    for(it = sfiTable.begin(); it != sfiTable.end(); it++) {
        cout << it->first << " : " << std::endl;
        int sn = it->second;
        for(int i = 0; i < slist[sn].size(); i++) {
            cout << stats[slist[sn][i]] << ' ';
        }
        cout << std::endl;
    }
    return 0;
}