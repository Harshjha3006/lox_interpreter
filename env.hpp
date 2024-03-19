#include<map>
#include<string>
using namespace std;
class Env{
    public:
    map<string,string>values;
    Env*enclosing;
    Env(){
        values = map<string,string>();
        enclosing = NULL;
    }
    Env(Env*enclosing){
        this->enclosing = enclosing;
        values = map<string,string>();
    }
    void define(string name,string value){
        values[name] = value;
    }
    string get(string name){
        if(values.find(name) != values.end()){
            return values.at(name);
        }
        if(enclosing != NULL)return enclosing->get(name);
        throw "Undefined Variable";
    }

    void assign(string name,string value){
        if(values.find(name) != values.end()){
            values[name] = value;
            return;
        }
        if(enclosing != NULL){
            enclosing->assign(name,value);
            return;
        }
        throw "Undefined Variable";
    }

};