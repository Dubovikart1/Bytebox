#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;
using ll = long long;

vector<vector<string>> db;
string dbname;

void init(){
    db[0] = {};
    ifstream initf(dbname);
    string buf;
    if(initf.is_open()){
        while(getline(initf, buf)){
            vector<string> v;
            string bv;
            for(int i = 0;i < buf.size();i++){
                if(buf[i]!=' '){
                    bv += buf[i];
                } else {
                    v.push_back(bv);
                    bv = "";
                }
            }
            db.push_back(v);
        }
    }
    initf.close();
}

void commands(string c){
    if(c == "$getall"){
        
    }
}

int main(int argc, char* argv[]){
    if(argc < 2){
        cerr << "Error: No arguments\n";
        return 1;
    }
    init();
    for(int i = 1; i <= argc;i++){
        string cmd = argv[i];
        if(cmd[0] = '$'){

        } else {

        }
    }
}