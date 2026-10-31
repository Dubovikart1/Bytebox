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
            string buf;
            if(getline(dbnf, buf) && !buf.empty()){
                dbname = buf;
            }
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

    namespace dbv
    {
        bool setdb(string path){
            if(path.empty()) return false;
            ofstream cf("env/dbname");
            if(!cf.is_open()) return false;
            cf << path << "\n";
            cf.close();
            if(!cf) return false;
            return true;
        }
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
    
    namespace sortv
    {
        bool tonum(string s, long long &x){
            try{
                size_t p = 0;
                x = stoll(s, &p);
                return p == s.size();
            } catch(...){
                return false;
            }
        }

        string lower(string s){
            for(int i = 0; i < (int)s.size(); i++){
                if(s[i] >= 'A' && s[i] <= 'Z'){
                    s[i] = s[i] - 'A' + 'a';
                }
            }
            return s;
        }

        int cmp(string a, string b, bool num, bool ci){
            if(num){
                long long x, y;
                bool ok1 = tonum(a, x);
                bool ok2 = tonum(b, y);
                if(ok1 && ok2){
                    if(x < y) return -1;
                    if(x > y) return 1;
                    return 0;
                }
                if(ok1 != ok2){
                    return ok1 ? -1 : 1;
                }
            }
            if(ci){
                a = lower(a);
                b = lower(b);
            }
            if(a < b) return -1;
            if(a > b) return 1;
            return 0;
        }

        bool emptyc(const vector<string> &r, int c){
            return c >= (int)r.size() || r[c].empty();
        }

        int cmpkey(const vector<string> &a, const vector<string> &b, const vector<int> &cols, bool desc, bool num, bool ci){
            for(int i = 0; i < (int)cols.size(); i++){
                int c = cols[i];
                bool ea = emptyc(a, c);
                bool eb = emptyc(b, c);
                if(ea && eb) continue;
                if(ea) return 1;
                if(eb) return -1;
                int r = cmp(a[c], b[c], num, ci);
                if(r != 0){
                    return desc ? -r : r;
                }
            }
            return 0;
        }

        bool sortrows(string col, bool desc, bool num, bool ci){
            if(db.size() < 2) return true;
            int w = 0;
            for(int i = 0; i < (int)db.size(); i++){
                w = max(w, (int)db[i].size());
            }
            vector<int> cols;
            if(col == "ALL"){
                for(int j = 0; j < w; j++){
                    cols.push_back(j);
                }
            } else {
                long long x;
                if(!tonum(col, x) || x < 0 || x >= w) return false;
                cols.push_back((int)x);
            }
            stable_sort(db.begin() + 1, db.end(), [&](const vector<string> &a, const vector<string> &b){
                return cmpkey(a, b, cols, desc, num, ci) < 0;
            });
            return true;
        }
    } 
    
    namespace uniqv
    {
        string keyval(string s, bool num, bool ci){
            if(num){
                long long x;
                if(sortv::tonum(s, x)){
                    return to_string(x);
                }
            }
            if(ci){
                s = sortv::lower(s);
            }
            return s;
        }

        string keyrow(const vector<string> &r, int w, bool num, bool ci){
            string k;
            for(int i = 0; i < w; i++){
                k += keyval(i < (int)r.size() ? r[i] : "", num, ci);
                k += '\x1f';
            }
            return k;
        }

        bool uniquevals(string col, bool ci, bool num, bool count){
            int w = 0;
            for(int i = 0; i < (int)db.size(); i++){
                w = max(w, (int)db[i].size());
            }
            int c = 0;
            if(col != "ALL"){
                long long x;
                if(!sortv::tonum(col, x) || x < 0 || x >= w) return false;
                c = (int)x;
            }
            vector<string> vals;
            vector<int> cnt;
            map<string, int> pos;
            for(int i = 1; i < (int)db.size(); i++){
                string key, shown;
                if(col == "ALL"){
                    key = keyrow(db[i], w, num, ci);
                    for(int j = 0; j < (int)db[i].size(); j++){
                        if(j != 0) shown += ' ';
                        shown += db[i][j];
                    }
                } else {
                    bool has = c < (int)db[i].size();
                    key = keyval(has ? db[i][c] : "", num, ci);
                    shown = has ? db[i][c] : "";
                }
                if(pos.find(key) == pos.end()){
                    pos[key] = (int)vals.size();
                    vals.push_back(shown);
                    cnt.push_back(1);
                } else {
                    cnt[pos[key]]++;
                }
            }
            for(int i = 0; i < (int)vals.size(); i++){
                if(count){
                    cout << vals[i] << " " << cnt[i] << "\n";
                } else {
                    cout << vals[i] << "\n";
                }
            }
            return true;
        }
    } 
    
    namespace renamev
    {
        bool renamecol(string col, string name){
            if(db.empty() || db[0].empty() || name.empty()) return false;
            int w = (int)db[0].size();
            int c = -1;
            long long x;
            if(sortv::tonum(col, x)){
                if(x < 0 || x >= w) return false;
                c = (int)x;
            } else {
                for(int i = 0; i < w; i++){
                    if(db[0][i] == col){
                        c = i;
                        break;
                    }
                }
                if(c < 0) return false;
            }
            for(int i = 0; i < w; i++){
                if(i != c && db[0][i] == name) return false;
            }
            db[0][c] = name;
            return true;
        }
    } 
    
    namespace replacev
    {
        int replacevals(string col, string oldv, string newv, bool ci, bool num){
            int w = 0;
            for(int i = 0; i < (int)db.size(); i++){
                w = max(w, (int)db[i].size());
            }
            int c = -1;
            if(col != "ALL"){
                long long x;
                if(!sortv::tonum(col, x) || x < 0 || x >= w) return -1;
                c = (int)x;
            }
            string key = uniqv::keyval(oldv, num, ci);
            int cnt = 0;
            for(int i = 1; i < (int)db.size(); i++){
                if(c < 0){
                    for(int j = 0; j < (int)db[i].size(); j++){
                        if(uniqv::keyval(db[i][j], num, ci) == key && db[i][j] != newv){
                            db[i][j] = newv;
                            cnt++;
                        }
                    }
                } else if(c < (int)db[i].size()){
                    if(uniqv::keyval(db[i][c], num, ci) == key && db[i][c] != newv){
                        db[i][c] = newv;
                        cnt++;
                    }
                }
            }
            return cnt;
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
    } else if(cmd == "renamecol" || cmd == "rename"){
        if(args.size() != 3){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        if(!it::renamev::renamecol(args[1], args[2])){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        changed = true;
    } else if(cmd == "replace"){
        if(args.size() < 4){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        bool ci = false, num = false;
        for(int i = 4; i < (int)args.size(); i++){
            if(args[i] == "ci"){
                ci = true;
            } else if(args[i] == "num"){
                num = true;
            } else if(args[i] == "str"){
                num = false;
            } else {
                cerr << "Error: wrong arguments\n";
                return false;
            }
        }
        int n = it::replacev::replacevals(args[1], args[2], args[3], ci, num);
        if(n < 0){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        if(n > 0){
            changed = true;
        }
    } else if(cmd == "sort"){
        if(args.size() < 2){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        bool desc = false, num = false, ci = false, view = false;
        for(int i = 2; i < (int)args.size(); i++){
            if(args[i] == "desc"){
                desc = true;
            } else if(args[i] == "asc"){
                desc = false;
            } else if(args[i] == "num"){
                num = true;
            } else if(args[i] == "str"){
                num = false;
            } else if(args[i] == "ci"){
                ci = true;
            } else if(args[i] == "view"){
                view = true;
            } else {
                cerr << "Error: wrong arguments\n";
                return false;
            }
        }
        if(!it::sortv::sortrows(args[1], desc, num, ci)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        if(view){
            it::read();
        } else {
            changed = true;
        }
    } else if(cmd == "unique" || cmd == "uniq"){
        if(args.size() < 2){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        bool ci = false, num = false, count = false;
        for(int i = 2; i < (int)args.size(); i++){
            if(args[i] == "ci"){
                ci = true;
            } else if(args[i] == "num"){
                num = true;
            } else if(args[i] == "str"){
                num = false;
            } else if(args[i] == "count"){
                count = true;
            } else {
                cerr << "Error: wrong arguments\n";
                return false;
            }
        }
        if(!it::uniqv::uniquevals(args[1], ci, num, count)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
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
    } else if(cmd == "chdb"){
        if(args.size() != 2 || !it::dbv::setdb(args[1])){
            cerr << "Error: wrong arguments\n";
            return false;
        }
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
