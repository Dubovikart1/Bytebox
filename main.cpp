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

bool interpretator(string cmd);

namespace it
{
    namespace view
    {
        void printrows(const vector<int>& rows, bool alwaysheader);
    }

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
        vector<int> rows;
        for (int i = 1; i < (int)db.size(); i++) {
            rows.push_back(i);
        }
        view::printrows(rows, true);
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

    int maxwidth(){
        int w = 0;
        for(int i = 0; i < (int)db.size(); i++){
            w = max(w, (int)db[i].size());
        }
        return w;
    }

    bool resolvecol(const string& tok, int& col, string& err){
        if(tok == "ALL"){
            col = -1;
            return true;
        }
        long long x;
        if(sortv::tonum(tok, x)){
            int w = maxwidth();
            if(x < 0 || x >= w){
                err = "column out of range";
                return false;
            }
            col = (int)x;
            return true;
        }
        if(db.empty() || db[0].empty()){
            err = "unknown column: " + tok;
            return false;
        }
        int found = -1;
        for(int i = 0; i < (int)db[0].size(); i++){
            if(db[0][i] == tok){
                if(found >= 0){
                    err = "duplicate column name: " + tok;
                    return false;
                }
                found = i;
            }
        }
        if(found < 0){
            err = "unknown column: " + tok;
            return false;
        }
        col = found;
        return true;
    }

    namespace view
    {
        void printrows(const vector<int>& rows, bool alwaysheader){
            if(db.empty()) return;
            if(rows.empty() && !alwaysheader) return;
            int maxid = 0;
            for(int i = 0; i < (int)rows.size(); i++){
                maxid = max(maxid, rows[i]);
            }
            int wx = max(1, (int)to_string(maxid).size());
            int mc = (int)db[0].size();
            for(int i = 0; i < (int)rows.size(); i++){
                mc = max(mc, (int)db[rows[i]].size());
            }
            vector<int> cw(mc, 0);
            for(int j = 0; j < (int)db[0].size(); j++){
                cw[j] = max(cw[j], (int)db[0][j].size());
            }
            for(int i = 0; i < (int)rows.size(); i++){
                for(int j = 0; j < (int)db[rows[i]].size(); j++){
                    cw[j] = max(cw[j], (int)db[rows[i]][j].size());
                }
            }
            cout << left << setw(wx + 2) << "ID";
            for(int j = 0; j < (int)db[0].size(); j++){
                cout << left << setw(cw[j] + 2) << db[0][j];
            }
            cout << "\n";
            for(int i = 0; i < (int)rows.size(); i++){
                int r = rows[i];
                cout << right << setw(wx) << r << "  ";
                for(int j = 0; j < (int)db[r].size(); j++){
                    cout << left << setw(cw[j] + 2) << db[r][j];
                }
                cout << "\n";
            }
        }
    }

    namespace cond
    {
        enum class kind { cmp, and_, or_, not_ };
        enum class op { eq, ne, lt, le, gt, ge, contains, starts, ends, empty_, notempty };

        struct node {
            kind k = kind::cmp;
            op o = op::eq;
            int col = -1;
            string value;
            vector<node> kids;
        };

        bool parseop(const string& s, op& o){
            if(s == "=" || s == "==") o = op::eq;
            else if(s == "!=") o = op::ne;
            else if(s == "<") o = op::lt;
            else if(s == "<=") o = op::le;
            else if(s == ">") o = op::gt;
            else if(s == ">=") o = op::ge;
            else if(s == "contains") o = op::contains;
            else if(s == "starts") o = op::starts;
            else if(s == "ends") o = op::ends;
            else if(s == "empty") o = op::empty_;
            else if(s == "notempty") o = op::notempty;
            else return false;
            return true;
        }

        struct parser {
            const vector<string>& tok;
            size_t pos = 0;
            string err;

            parser(const vector<string>& t) : tok(t) {}

            bool end() const { return pos >= tok.size(); }
            const string& peek() const { return tok[pos]; }

            bool parsecmp(node& out){
                if(end()){
                    err = "expected column";
                    return false;
                }
                string ct = tok[pos++];
                int col;
                if(!resolvecol(ct, col, err)) return false;
                if(end()){
                    err = "expected operator";
                    return false;
                }
                string ot = tok[pos++];
                op o;
                if(!parseop(ot, o)){
                    err = "unknown operator: " + ot;
                    return false;
                }
                out.k = kind::cmp;
                out.o = o;
                out.col = col;
                if(o == op::empty_ || o == op::notempty) return true;
                if(end()){
                    err = "expected value";
                    return false;
                }
                if(peek() == "and" || peek() == "or" || peek() == ")"){
                    err = "expected value";
                    return false;
                }
                out.value = tok[pos++];
                return true;
            }

            bool parseunary(node& out){
                if(!end() && peek() == "not"){
                    pos++;
                    node child;
                    if(!parseunary(child)) return false;
                    out.k = kind::not_;
                    out.kids.clear();
                    out.kids.push_back(child);
                    return true;
                }
                if(!end() && peek() == "("){
                    pos++;
                    if(!parseor(out)) return false;
                    if(end() || peek() != ")"){
                        err = "expected )";
                        return false;
                    }
                    pos++;
                    return true;
                }
                return parsecmp(out);
            }

            bool parseand(node& out){
                node first;
                if(!parseunary(first)) return false;
                vector<node> kids;
                kids.push_back(first);
                while(!end() && peek() == "and"){
                    pos++;
                    node next;
                    if(!parseunary(next)) return false;
                    kids.push_back(next);
                }
                if(kids.size() == 1){
                    out = kids[0];
                } else {
                    out.k = kind::and_;
                    out.kids = kids;
                }
                return true;
            }

            bool parseor(node& out){
                node first;
                if(!parseand(first)) return false;
                vector<node> kids;
                kids.push_back(first);
                while(!end() && peek() == "or"){
                    pos++;
                    node next;
                    if(!parseand(next)) return false;
                    kids.push_back(next);
                }
                if(kids.size() == 1){
                    out = kids[0];
                } else {
                    out.k = kind::or_;
                    out.kids = kids;
                }
                return true;
            }

            bool parse(node& out){
                if(!parseor(out)) return false;
                if(!end()){
                    err = "unexpected token: " + peek();
                    return false;
                }
                return true;
            }
        };

        bool hastextop(const node& n){
            if(n.k == kind::cmp){
                return n.o == op::contains || n.o == op::starts || n.o == op::ends;
            }
            for(int i = 0; i < (int)n.kids.size(); i++){
                if(hastextop(n.kids[i])) return true;
            }
            return false;
        }

        bool textcmp(const string& a, const string& b, bool ci){
            return sortv::cmp(a, b, false, ci) == 0;
        }

        bool evalcell(const node& n, const string& cell, bool ci, bool num){
            if(n.o == op::empty_) return cell.empty();
            if(n.o == op::notempty) return !cell.empty();

            if(n.o == op::contains || n.o == op::starts || n.o == op::ends){
                string a = cell;
                string b = n.value;
                if(ci){
                    a = sortv::lower(a);
                    b = sortv::lower(b);
                }
                if(n.o == op::contains) return a.find(b) != string::npos;
                if(n.o == op::starts) return a.rfind(b, 0) == 0;
                if(b.size() > a.size()) return false;
                return a.compare(a.size() - b.size(), b.size(), b) == 0;
            }

            long long x = 0, y = 0;
            bool nx = sortv::tonum(cell, x);
            bool ny = sortv::tonum(n.value, y);
            if(num){
                if(n.o == op::eq){
                    if(nx && ny) return x == y;
                    return textcmp(cell, n.value, ci);
                }
                if(n.o == op::ne){
                    if(nx && ny) return x != y;
                    return !textcmp(cell, n.value, ci);
                }
                if(!nx || !ny) return false;
            } else {
                int c = sortv::cmp(cell, n.value, false, ci);
                if(n.o == op::eq) return c == 0;
                if(n.o == op::ne) return c != 0;
                if(n.o == op::lt) return c < 0;
                if(n.o == op::le) return c <= 0;
                if(n.o == op::gt) return c > 0;
                if(n.o == op::ge) return c >= 0;
                return false;
            }

            if(n.o == op::lt) return x < y;
            if(n.o == op::le) return x <= y;
            if(n.o == op::gt) return x > y;
            if(n.o == op::ge) return x >= y;
            return false;
        }

        bool eval(const node& n, const vector<string>& row, int width, bool ci, bool num){
            if(n.k == kind::and_){
                for(int i = 0; i < (int)n.kids.size(); i++){
                    if(!eval(n.kids[i], row, width, ci, num)) return false;
                }
                return true;
            }
            if(n.k == kind::or_){
                for(int i = 0; i < (int)n.kids.size(); i++){
                    if(eval(n.kids[i], row, width, ci, num)) return true;
                }
                return false;
            }
            if(n.k == kind::not_){
                return !eval(n.kids[0], row, width, ci, num);
            }

            if(n.col == -1){
                for(int j = 0; j < width; j++){
                    string cell = j < (int)row.size() ? row[j] : "";
                    if(evalcell(n, cell, ci, num)) return true;
                }
                return false;
            }
            string cell = n.col < (int)row.size() ? row[n.col] : "";
            return evalcell(n, cell, ci, num);
        }
    }

    namespace query
    {
        bool findcmd(const vector<string>& arg){
            if(arg.size() < 2){
                cerr << "Error: wrong arguments\n";
                return false;
            }
            string value = arg[1];
            int col = -1;
            bool in = false, ci = false, num = false, str = false, substr = false, ids = false, count = false;
            for(int i = 2; i < (int)arg.size(); i++){
                if(arg[i] == "in"){
                    if(in || i + 1 >= (int)arg.size()){
                        cerr << "Error: wrong arguments\n";
                        return false;
                    }
                    in = true;
                    string err;
                    if(!resolvecol(arg[++i], col, err)){
                        cerr << "Error: " << err << "\n";
                        return false;
                    }
                } else if(arg[i] == "ci"){
                    ci = true;
                } else if(arg[i] == "num"){
                    num = true;
                } else if(arg[i] == "str"){
                    str = true;
                } else if(arg[i] == "substr"){
                    substr = true;
                } else if(arg[i] == "ids"){
                    ids = true;
                } else if(arg[i] == "count"){
                    count = true;
                } else {
                    cerr << "Error: wrong arguments\n";
                    return false;
                }
            }
            if(num && str){
                cerr << "Error: num and str cannot be used together\n";
                return false;
            }
            if(ids && count){
                cerr << "Error: ids and count cannot be used together\n";
                return false;
            }
            if(num && substr){
                cerr << "Error: num cannot be used with substr\n";
                return false;
            }
            int w = maxwidth();
            string needle = value;
            if(ci && substr) needle = sortv::lower(needle);
            vector<int> rows;
            for(int i = 1; i < (int)db.size(); i++){
                bool ok = false;
                int from = col == -1 ? 0 : col;
                int to = col == -1 ? w : col + 1;
                for(int j = from; j < to; j++){
                    string cell = j < (int)db[i].size() ? db[i][j] : "";
                    if(substr){
                        if(ci) cell = sortv::lower(cell);
                        if(cell.find(needle) != string::npos){
                            ok = true;
                            break;
                        }
                    } else {
                        long long x = 0, y = 0;
                        bool nx = sortv::tonum(cell, x);
                        bool ny = sortv::tonum(value, y);
                        bool eq;
                        if(num && nx && ny) eq = x == y;
                        else eq = sortv::cmp(cell, value, false, ci) == 0;
                        if(eq){
                            ok = true;
                            break;
                        }
                    }
                }
                if(ok) rows.push_back(i);
            }
            if(count){
                cout << rows.size() << "\n";
            } else if(ids){
                for(int i = 0; i < (int)rows.size(); i++){
                    cout << rows[i] << "\n";
                }
            } else {
                view::printrows(rows, false);
            }
            return true;
        }

        bool wherecmd(const vector<string>& arg){
            if(arg.size() < 2){
                cerr << "Error: wrong arguments\n";
                return false;
            }
            bool ci = false, num = false, str = false, ids = false, count = false;
            size_t end = arg.size();
            while(end > 1){
                string t = arg[end - 1];
                if(t == "ci"){
                    ci = true;
                    end--;
                } else if(t == "num"){
                    num = true;
                    end--;
                } else if(t == "str"){
                    str = true;
                    end--;
                } else if(t == "ids"){
                    ids = true;
                    end--;
                } else if(t == "count"){
                    count = true;
                    end--;
                } else {
                    break;
                }
            }
            if(num && str){
                cerr << "Error: num and str cannot be used together\n";
                return false;
            }
            if(ids && count){
                cerr << "Error: ids and count cannot be used together\n";
                return false;
            }
            if(end <= 1){
                cerr << "Error: wrong arguments\n";
                return false;
            }
            vector<string> tokens(arg.begin() + 1, arg.begin() + end);
            int w = maxwidth();
            cond::node root;
            cond::parser p(tokens);
            if(!p.parse(root)){
                cerr << "Error: " << p.err << "\n";
                return false;
            }
            if(num && cond::hastextop(root)){
                cerr << "Error: num cannot be used with contains/starts/ends\n";
                return false;
            }
            vector<int> rows;
            for(int i = 1; i < (int)db.size(); i++){
                if(cond::eval(root, db[i], w, ci, num)){
                    rows.push_back(i);
                }
            }
            if(count){
                cout << rows.size() << "\n";
            } else if(ids){
                for(int i = 0; i < (int)rows.size(); i++){
                    cout << rows[i] << "\n";
                }
            } else {
                view::printrows(rows, false);
            }
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

    vector<string> splitline(const string& line){
        vector<string> out;
        string cur;
        for(int i = 0; i < (int)line.size(); i++){
            char c = line[i];
            if(c == ' ' || c == '\t' || c == '\r' || c == '\n'){
                if(!cur.empty()){
                    if(cur == "\"\"" || cur == "''") out.push_back("");
                    else out.push_back(cur);
                    cur.clear();
                }
            } else {
                cur += c;
            }
        }
        if(!cur.empty()){
            if(cur == "\"\"" || cur == "''") out.push_back("");
            else out.push_back(cur);
        }
        return out;
    }

    struct scriptflags {
        bool ci = false;
        bool num = false;
        bool str = false;
    };

    bool scriptcond(const vector<string>& tok, size_t start, cond::node& root, scriptflags& flags, string& err){
        size_t end = tok.size();
        while(end > start){
            string t = tok[end - 1];
            if(t == "ci"){
                flags.ci = true;
                end--;
            } else if(t == "num"){
                flags.num = true;
                end--;
            } else if(t == "str"){
                flags.str = true;
                end--;
            } else {
                break;
            }
        }
        if(flags.num && flags.str){
            err = "num and str cannot be used together";
            return false;
        }
        if(end <= start){
            err = "expected condition";
            return false;
        }
        vector<string> ct(tok.begin() + start, tok.begin() + end);
        cond::parser p(ct);
        if(!p.parse(root)){
            err = p.err;
            return false;
        }
        if(flags.num && cond::hastextop(root)){
            err = "num cannot be used with contains/starts/ends";
            return false;
        }
        return true;
    }

    bool hasmatch(const cond::node& root, bool ci, bool num){
        int w = maxwidth();
        for(int i = 1; i < (int)db.size(); i++){
            if(cond::eval(root, db[i], w, ci, num)) return true;
        }
        return false;
    }

    bool validatescript(const vector<string>& lines, string& err){
        vector<int> stack;
        for(int l = 0; l < (int)lines.size(); l++){
            vector<string> tok = splitline(lines[l]);
            if(tok.empty() || tok[0].empty() || tok[0][0] == '#') continue;
            if(tok[0] == "if"){
                if(tok.size() < 2){
                    err = "expected condition";
                    return false;
                }
                stack.push_back(0);
            } else if(tok[0] == "elif"){
                if(tok.size() < 2){
                    err = "expected condition";
                    return false;
                }
                if(stack.empty()){
                    err = "unexpected elif";
                    return false;
                }
                if(stack.back() == 2){
                    err = "elif after else";
                    return false;
                }
            } else if(tok[0] == "else"){
                if(tok.size() != 1){
                    err = "wrong else";
                    return false;
                }
                if(stack.empty()){
                    err = "unexpected else";
                    return false;
                }
                if(stack.back() == 2){
                    err = "duplicate else";
                    return false;
                }
                stack.back() = 2;
            } else if(tok[0] == "end"){
                if(tok.size() != 1){
                    err = "wrong end";
                    return false;
                }
                if(stack.empty()){
                    err = "unexpected end";
                    return false;
                }
                stack.pop_back();
            }
        }
        if(!stack.empty()){
            err = "missing end";
            return false;
        }
        return true;
    }

    enum class blockend { eof, end_, elif_, else_ };
    bool runscript(const string& path, int depth = 0);
    bool execscriptcmd(const vector<string>& tok, int depth, bool& ok);

    blockend execblock(const vector<string>& lines, size_t& i, bool execute, int depth, bool& ok){
        while(i < lines.size()){
            vector<string> tok = splitline(lines[i]);
            if(tok.empty() || tok[0].empty() || tok[0][0] == '#'){
                i++;
                continue;
            }
            if(tok[0] == "end"){
                if(tok.size() != 1){
                    cerr << "Error: wrong end\n";
                    ok = false;
                    return blockend::eof;
                }
                return blockend::end_;
            }
            if(tok[0] == "elif"){
                return blockend::elif_;
            }
            if(tok[0] == "else"){
                if(tok.size() != 1){
                    cerr << "Error: wrong else\n";
                    ok = false;
                    return blockend::eof;
                }
                return blockend::else_;
            }
            if(tok[0] == "if"){
                bool taken = false;
                bool done = false;
                if(execute){
                    scriptflags flags;
                    string err;
                    cond::node root;
                    if(!scriptcond(tok, 1, root, flags, err)){
                        cerr << "Error: " << err << "\n";
                        ok = false;
                        return blockend::eof;
                    }
                    taken = hasmatch(root, flags.ci, flags.num);
                    done = taken;
                }
                i++;
                blockend be = execblock(lines, i, taken, depth, ok);
                if(!ok) return blockend::eof;
                while(be == blockend::elif_){
                    bool etaken = false;
                    if(execute && !done){
                        vector<string> et = splitline(lines[i]);
                        scriptflags ef;
                        string eerr;
                        cond::node eroot;
                        if(!scriptcond(et, 1, eroot, ef, eerr)){
                            cerr << "Error: " << eerr << "\n";
                            ok = false;
                            return blockend::eof;
                        }
                        etaken = hasmatch(eroot, ef.ci, ef.num);
                    }
                    done = done || etaken;
                    i++;
                    be = execblock(lines, i, etaken, depth, ok);
                    if(!ok) return blockend::eof;
                }
                if(be == blockend::else_){
                    i++;
                    be = execblock(lines, i, execute && !done, depth, ok);
                    if(!ok) return blockend::eof;
                }
                if(be == blockend::eof){
                    cerr << "Error: missing end\n";
                    ok = false;
                    return blockend::eof;
                }
                if(be != blockend::end_){
                    cerr << "Error: unexpected elif/else\n";
                    ok = false;
                    return blockend::eof;
                }
                i++;
                continue;
            }
            if(execute && !execscriptcmd(tok, depth, ok)){
                return blockend::eof;
            }
            i++;
        }
        return blockend::eof;
    }

    bool execscriptcmd(const vector<string>& tok, int depth, bool& ok){
        if(tok[0] == "run"){
            if(tok.size() != 2){
                cerr << "Error: wrong arguments\n";
                ok = false;
                return false;
            }
            if(!runscript(tok[1], depth + 1)){
                ok = false;
                return false;
            }
            return true;
        }
        vector<string> saved = args;
        args = tok;
        bool changed = interpretator(tok[0]);
        args = saved;
        if(changed) f::write();
        return true;
    }

    bool runscript(const string& path, int depth){
        if(depth > 16){
            cerr << "Error: script nesting too deep\n";
            return false;
        }
        ifstream script(path);
        if(!script.is_open()){
            cerr << "Error: cannot open script: " << path << "\n";
            return false;
        }
        vector<string> lines;
        string line;
        while(getline(script, line)){
            if(!line.empty() && line.back() == '\r') line.pop_back();
            lines.push_back(line);
        }
        string err;
        if(!validatescript(lines, err)){
            cerr << "Error: " << err << "\n";
            return false;
        }
        size_t i = 0;
        bool ok = true;
        blockend be = execblock(lines, i, true, depth, ok);
        if(!ok) return false;
        if(be != blockend::eof){
            if(be == blockend::end_) cerr << "Error: unexpected end\n";
            else if(be == blockend::elif_) cerr << "Error: unexpected elif\n";
            else cerr << "Error: unexpected else\n";
            return false;
        }
        return true;
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
        vector<vector<string>> saved = db;
        if(!it::sortv::sortrows(args[1], desc, num, ci)){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        if(view){
            it::read();
            db = saved;
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
    } else if(cmd == "find"){
        it::query::findcmd(args);
    } else if(cmd == "where"){
        it::query::wherecmd(args);
    } else if(cmd == "run"){
        if(args.size() != 2){
            cerr << "Error: wrong arguments\n";
            return false;
        }
        it::runscript(args[1], 0);
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
        f::init();
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
