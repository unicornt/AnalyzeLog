#include <bits/stdc++.h>
using namespace std;
void AnalyzeCode(string s) {

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
            // get sfi of the function & save it if never meet it before
            // mark it as "marking"
            // deoptimization: get last optimized
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