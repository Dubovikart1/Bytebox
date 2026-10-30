#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;
using ll = long long;

vector<vector<string>> db;

string dbname = "~/projects/bytebox";

namespace f{
    void init(){
        fs::current_path() = "~";
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

    void write(){
        ofstream outf(dbname);
        for(int i = 0;i < db.size();i++){
            for(int j = 0;j < db[0].size();j++){
                outf << db[i][j] << ' ';
            }
            cout << "\n";
        }
    }
}

namespace it{
    void read(){
        for(int i = 0;i < db.size();i++){
            for(int j = 0;j < db[0].size();j++){
                cout << db[i][j] << ' ';
            }
            cout << "\n";
        }
    }

    void getrow(int id){
        for(int i = 0;i < db.size();i++){
            cout << db[i][id] << "\n";
        }
    }

    void getline(int id){
        for(int i = 0;i < db[id].size();i++){
            cout << db[id][i] << "\n";
        }
    }

    void dbget(int x, int y){
        cout << db[x][y];
    }

    void dbadd(vector<string> arg){
        vector<string> buf;
        for(int i = 1;i < arg.size();i++){
            buf.push_back(arg[i]);
        }
        db.push_back(buf);
    }
}

int main(int argc, char* argv[]){
    vector<string> arg;
    if(argc < 2){
        cerr << "Error: No arguments\n";
        return 1;
    }
    for(int i = 1;i <= argc; i++){
        arg.push_back(argv[i]);
    }
    f::init();
    string cmd = arg[0];
    if(cmd == "read"){
        it::read();
    } else if(cmd == "getrow"){
        int id = stoi(arg[1]);
        it::getrow(id);
    } else if(cmd == "getline"){
        int id = stoi(arg[1]);
        it::getline(id);
    } else if(cmd == "get"){
        int x = stoi(arg[1]);
        int y = stoi(arg[1]);
        it::dbget(x,y);
    } else if(cmd == "addrow"){
        db[0].push_back(arg[1]);
    } else if(cmd == "add"){
        it::dbadd(arg);
    }
}