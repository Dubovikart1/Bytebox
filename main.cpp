#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;
using ll = long long;

vector<vector<string>> db;
string dbname = "database.db";

namespace f{
    void init(){
        ifstream dbnf("env/dbname");
        if(dbnf.is_open()){
            dbnf >> dbname;
        }
        dbnf.close();
        db.clear();   
        ifstream initf(dbname);
        string buf;
        if(initf.is_open()){
            while(getline(initf, buf)){
                vector<string> v;
                string bv;
                for(int i = 0; i < buf.size(); i++){
                    if(buf[i] != ' '){
                        bv += buf[i];
                    } else {
                        v.push_back(bv);
                        bv = "";
                    }
                }
                if(!bv.empty()){
                    v.push_back(bv);
                }
                db.push_back(v);
            }
        }
        initf.close();
    }

    void write(){
        ofstream outf(dbname);
        if(outf.is_open()){
            outf.clear();
            for(int i = 0; i < db.size(); i++){
                for(int j = 0; j < db[i].size(); j++){
                    outf << db[i][j] << ' ';
                }
                outf << "\n";
            }
        }
    }
}

namespace it
{
    void help(){
        ifstream helpfile("env/help.txt");
        if(helpfile.is_open()){
            string buf;
            while(getline(helpfile, buf)){
                cout << buf << "\n";
            }
        }
        helpfile.close();
    }

    void read(){
        for(int i = 0; i < db.size(); i++){
            for(int j = 0; j < db[i].size(); j++){
                cout << db[i][j] << ' ';
            }
            cout << "\n";
        }
    }

    namespace add
    {
        void dbadd(vector<string> arg){
            vector<string> buf;
            for(int i = 1; i < arg.size(); i++){
                buf.push_back(arg[i]);
            }
            db.push_back(buf);
        }
        void addcol(string a){
            if (db.empty()) {
                db.push_back({});
            }
            db[0].push_back(a);
        }
    } 
    namespace get
    {
        void getcol(int id){
            for(int i = 0; i < db.size(); i++){
                if (id >= 0 && id < db[i].size()) {
                    cout << db[i][id] << "\n";
                }
            }
        }

        void getrow(int id){
            if (id >= 0 && id < db.size()) {
                for(int i = 0; i < db[id].size(); i++){
                    cout << db[id][i] << "\n";
                }
            }
        }

        void dbget(int x, int y){
            if (x >= 0 && x < db.size() && y >= 0 && y < db[x].size()) {
                cout << db[x][y] << "\n";
            }
        }
    } 
    
    namespace del{
        void del(int x, int y){
        if (x >= 0 && x < db.size() && y >= 0 && y < db[x].size()) {
            db[x].erase(db[x].begin() + y);
        }
    }

        void delrow(int x){
            for(int i = 0;i < db[x].size();i++){
                db[x].erase(db[x].begin() + i);
            }
        }

        void delcol(int x){
            for(int i = 0;i < db.size();i++){
                db[i].erase(db[i].begin() + x);
            }
        }
    } 
}

int main(int argc, char* argv[]){
    vector<string> arg;
    if(argc < 2){
        cerr << "Error: No arguments\n";
        return 1;
    }
    for(int i = 1; i < argc; i++){
        arg.push_back(argv[i]);
    }
    f::init();
    string cmd = arg[0];
    if(cmd == "read"){
        it::read();
    } else if(cmd == "getcol"){
        int id = stoi(arg[1]);
        it::get::getcol(id);
    } else if(cmd == "getrow"){
        int id = stoi(arg[1]);
        it::get::getrow(id);
    } else if(cmd == "get"){
        int x = stoi(arg[1]);
        int y = stoi(arg[2]);
        it::get::dbget(x, y);
    } else if(cmd == "addcol"){
        it::add::addcol(arg[1]);
    } else if(cmd == "add"){
        it::add::dbadd(arg);
    } else if(cmd == "delete"){
        int x = stoi(arg[1]);
        int y = stoi(arg[2]);
        it::del::del(x,y);
    } else if(cmd == "delrow"){
        int x = stoi(arg[1]);
        it::del::delrow(x);
    } else if(cmd == "delcol"){
        int x = stoi(arg[1]);
        it::del::delcol(x);
    } else if(cmd == "help"){
        it::help();
    } else {
        cerr << "Error: wrong command\n";
    }
    if(db.size() != 0){
        f::write();
    }
    return 0;
}
