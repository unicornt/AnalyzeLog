#include <bits/stdc++.h>
using namespace std;
const string null_str = "";
const int func_num = 10000;
enum {init, marked, demarked, compiling, optimizing, optimized};
map<string, int>sfiTable;
int status[func_num];
vector<int>slist[func_num]; // list of status
vector<string>rlist[func_num]; // list of reason(if have)
vector<int>rtlist[func_num]; // list of times of running

//////////////////////////////////////////////////////////

void AnalyzeCode(string s) {

}

string GetSFI(string s) {
    string sfi = null_str;
    size_t p = s.find("sfi");
    if(p == string::npos) {
        // error
        cout << "error: no sfi" << std::endl;
        exit(1);
    }
    p += 3;
    while(s[p] == ' ' || s[p] == '=') p++;
    while(s[p] != ')') {
        sfi += s[p++];
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
        cout << "error: no reason" << std::endl;
        exit(1);
    }
    p += 8;
    while(s[p] != ')') {
        reason += s[p++];
    }
    return reason;
}


void Analyze(string s) {
    string sfi;
    if(s[0] != '[') AnalyzeCode(s);
    else {
        switch (s[1])
        {
        // after change status, calcute running times (need add function to v8 source code)
        case 'm':
            // marking XXXXXXX for optimized recompilation
            // marking XXXXXXX for deoptimization
            // get sfi of the function & save it if never meet it before
            sfi = GetSFI(s);
            // get sfi of the function & save it if never meet it before init it as "init"
            // mark it as "marking"
            // deoptimization: get last optimized
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
            ChangeStatus(sn, CheckTar(s), GetReason(s));
            break;
        case 'c':
            //compiling method XXXXXXX using TurboFan OSR
            // get sfi of the function & mark it as "compiling"
            sfi = GetSFI(s);
            int sn = sfiTable[sfi];
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
            int sn = sfiTable[sfi];
            ChangeStatus(sn, optimizing);
            break;
        case 'b':
            // bailout 
            // record reason
            sfi = GetSFI(s);
            int sn = sfiTable[sfi];
            ChangeStatus(sn, init, GetReason(s));
            break;
        default:
            cout << "unsupport sentence" << std::endl;
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
    map<string, int>::iterator it;
    for(it = sfiTable.begin(); it != sfiTable.end(); it++) {
        cout << it->first << " : " << std::endl;
        int sn = it->second;
        for(int i = 0; i < slist[sn].size(); i++) {
            cout << slist[sn][i] << ' ';
        }
        cout << std::endl;
    }
    return 0;
}