#include<iostream>
#include<fstream>
#include<sstream>
#include "interpreter.hpp"
using namespace std;


int main(int argc, char **argv){
    if(argc != 2){
        cerr<<"Incorrect Usage"<<endl;
        cerr<<"Correct Usage is : lox <file.lx>"<<endl;
        return 1;
    }
    fstream file(argv[1],ios_base::in);
    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();
    file.close();
    Scanner::Scanner *scanner = new Scanner::Scanner(source);
    vector<Token::Token>tokens = scanner->scanTokens();
    // for(auto token :tokens){
    //     token.printToken();
    // }
    // cout<<endl;
    Parser::Parser *parser = new Parser::Parser(tokens);
    vector<Stmt*>statements = parser->parse();
    Interpreter*interpreter = new Interpreter();
    vector<string>evals = interpreter->interpret(statements);
    for(auto &eval : evals){
        if(eval.size() <= 0)continue;
        cout<<eval<<endl;
    }

}