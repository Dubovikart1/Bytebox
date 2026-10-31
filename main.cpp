#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;
using ll = long long;

vector<vector<string>> db;
vector<string> args;
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
        string tmp = dbname + ".tmp";
        ofstream outf(tmp);
        if(!outf.is_open()){
            cerr << "Error: cannot open database\n";
            return;
        }
        for(int i = 0; i < db.size(); i++){
            for(int j = 0; j < db[i].size(); j++){
                outf << db[i][j] << ' ';
            }
            outf << "\n";
        }
        outf.close();
        if(!outf){
            cerr << "Error: write failed\n";
            return;
        }
        error_code ec;
        fs::rename(tmp, dbname, ec);
        if(ec){
            cerr << "Error: rename failed\n";
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

    void read() {
        if (db.empty()) return;
        int r = db.size();
        int wX = max(1, (int)to_string(r - 1).size());
        int mc = 0;
        for (int i = 0; i < r; i++) {
            mc = max(mc, (int)db[i].size());
        }
        vector<int> cw(mc, 0);
        for (int i = 0; i < r; i++) {
            for (int j = 0; j < (int)db[i].size(); j++) {
                cw[j] = max(cw[j], (int)db[i][j].size());
            }
        }
        for (int i = 0; i < r; i++) {
            if (i == 0) {
                cout << left << setw(wX + 2) << "ID";
            } else {
                cout << right << setw(wX) << i << "  ";
            }

            for (int j = 0; j < (int)db[i].size(); j++) {
                cout << left << setw(cw[j] + 2) << db[i][j];
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
            if(db.empty()){
                db.push_back({});
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
            for(int i = 1; i < db.size(); i++){
                if (id >= 0 && id < (int)db[i].size()) {
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
    
    namespace setv
    {
        bool setvalue(int x, int y, string val){
            if (x > 0 && x < (int)db.size() && y >= 0 && y < (int)db[x].size()) {
                db[x][y] = val;
                return true;
            }
            return false;
        }

        bool setrow(int x, vector<string> vals){
            if (x >= 0 && x < (int)db.size() && vals.size() == db[x].size()) {
                for(int j = 0; j < (int)vals.size(); j++){
                    db[x][j] = vals[j];
                }
                return true;
            }
            return false;
        }

        bool setcol(int y, vector<string> vals){
            if (y < 0) return false;
            int cnt = 0;
            for(int i = 1; i < db.size(); i++){
                if (y < (int)db[i].size()) {
                    cnt++;
                }
            }
            if (cnt == 0 || (int)vals.size() != cnt) return false;
            int k = 0;
            for(int i = 1; i < db.size(); i++){
                if (y < (int)db[i].size()) {
                    db[i][y] = vals[k];
                    k++;
                }
            }
            return true;
        }
    } 
    
    namespace del{
        void del(int x, int y){
            if (x > 0 && x < (int)db.size() && y >= 0 && y < (int)db[x].size()) {
                db[x][y] = "";
            }
        }

        void delrow(int x){
            if (x > 0 && x < (int)db.size()) {
                db[x].clear();
            }
        }

        void delcol(int x){
            if (x < 0) return;
            for(int i = 0;i < db.size();i++){
                if (x < (int)db[i].size()) {
                    db[i].erase(db[i].begin() + x);
                }
            }
        }
    } 
}


bool toint(int i, int &x){
    if(i >= (int)args.size()) return false;
    try{
        size_t p = 0;
        x = stoi(args[i], &p);
        return p == args[i].size();
    } catch(...){
        return false;
    }
}

bool interpretator(string cmd){
    bool changed = false;
    if(cmd == "read"){
        it::read();
    } else if(cmd == "getcol"){
        int id;
        if(!toint(1, id)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::get::getcol(id);
    } else if(cmd == "getrow"){
        int id;
        if(!toint(1, id)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::get::getrow(id);
    } else if(cmd == "get"){
        int x, y;
        if(!toint(1, x) || !toint(2, y)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::get::dbget(x, y);
    } else if(cmd == "addcol"){
        if(args.size() < 2){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::add::addcol(args[1]);
        changed = true;
    } else if(cmd == "add"){
        if(args.size() < 2){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::add::dbadd(args);
        changed = true;
    } else if(cmd == "set"){
        int x, y;
        if(args.size() != 4 || !toint(1, x) || !toint(2, y)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        if(it::setv::setvalue(x, y, args[3])){
            changed = true;
        }
    } else if(cmd == "setrow"){
        int x;
        if(args.size() < 3 || !toint(1, x)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        vector<string> vals;
        for(int i = 2; i < args.size(); i++){
            vals.push_back(args[i]);
        }
        if(!it::setv::setrow(x, vals)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        changed = true;
    } else if(cmd == "setcol"){
        int y;
        if(args.size() < 3 || !toint(1, y)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        vector<string> vals;
        for(int i = 2; i < args.size(); i++){
            vals.push_back(args[i]);
        }
        if(!it::setv::setcol(y, vals)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        changed = true;
    } else if(cmd == "delete"){
        int x, y;
        if(!toint(1, x) || !toint(2, y)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::del::del(x,y);
        changed = true;
    } else if(cmd == "delrow"){
        int x;
        if(!toint(1, x)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::del::delrow(x);
        changed = true;
    } else if(cmd == "delcol"){
        int x;
        if(!toint(1, x)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::del::delcol(x);
        changed = true;
    } else if(cmd == "help"){
        it::help();
    } else {
        cerr << "Error: wrong command\n";
    }
    return changed;
}

int main(int argc, char* argv[]){
    if(argc < 2){
        cerr << "Error: No arguments\n";
        return 1;
    }
    for(int i = 1; i < argc; i++)
        args.push_back(argv[i]);
    f::init();
    string cmd = args[0];
    if(interpretator(cmd)){
        f::write();
    }
    return 0;
}
