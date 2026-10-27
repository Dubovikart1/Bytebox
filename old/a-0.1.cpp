#include <bits/stdc++.h>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

vector<pair<string, string>> db;
string dbname = "database.db";

string geti(int i){
    string a = db[i].first, b = db[i].second;
    string s = to_string(i) + " " + a + " " + b;
    return s;
}

string getl(string l){
    for(int i = 0;i < db.size();i++){
        if(db[i].first == l){
            return to_string(i) + " " + l + " " + db[i].second;
        }
    }
    return "";
}

string getp(string p){
    for(int i = 0;i < db.size();i++){
        if(db[i].second == p){
            return to_string(i) + " " + db[i].first + " " + p;
        }
    }
    return "";
}

void dbread(){
    for(int i = 0;i < db.size();i++){
        cout << geti(i) << "\n";
    }
}

void dbdel(int i){
    db.erase(db.begin() + i);
}

pair<string, string> reader(string s){
    string a,b,buf;
    for(int i = 0;i < s.size();i++){
        if(s[i] == ' '){
            a = buf;
            buf = "";
        } else {
            buf += s[i];
        }
    }
    b = buf;
    buf = "";
    return {a,b};
}

string writer(pair<string, string> a){
    string s = a.first + " " + a.second;
    return s;
}

int main(int argc, char* argv[]){
    // INIT
    if(argc < 2){
        cerr << "Error: No arguments!\n";
        return 1;
    }
    ifstream file(dbname, ios::binary);
    string buf_1;
    if(file.is_open()){
        while(getline(file, buf_1)){
            db.push_back(reader(buf_1));
        }
    }
    file.close();
    // Program
    string command = argv[1];
    for(int i = 0;i < command.size();i++){
        if(command[i] >= 'A' && command[i] <= 'Z'){
            command[i] = tolower(command[i]);
        }
    }
    if(command == "geti"){
        int i = stoi(argv[2]);
        cout << geti(i) << "\n";
    } else if(command == "getl"){
        string l = argv[2];
        cout << getl(l) << "\n";
    } else if(command == "getp"){
        string p = argv[2];
        cout << getp(p) << "\n";
    } else if(command == "add"){
        string l = argv[2], p = argv[3];
        db.push_back({l,p});
    } else if(command == "read"){
        dbread();
    } else if(command == "delete"){
        int i = stoi(argv[2]);
        dbdel(i);
    } else {
        cerr << "Error: Wrong command!\n";
        return 1;
    }
    // End
    fs::remove(dbname);
    ofstream out(dbname, ios::binary);
    if(out.is_open()){
        for(int i = 0;i < db.size();i++){
            string line = writer(db[i]); 
            out << line << "\n";
        }
    }
    out.close();
    return 0;
}