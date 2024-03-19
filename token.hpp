#include<iostream>
#include<vector>
#include<string>
using namespace std;


namespace Token{
enum TokenType{
    // Single Character Tokens
    LEFT_PAREN,RIGHT_PAREN,LEFT_BRACE,RIGHT_BRACE,
    PLUS,MINUS,SLASH,STAR,COMMA,DOT,SEMICOLON,

    //One or 2 character tokens
    BANG,BANG_EQUAL,GREATER,LESSER,GREATER_EQUAL,
    LESSER_EQUAL,EQUAL,EQUAL_EQUAL,

    //Literals
    IDENTIFIER,STRING,NUMBER,

    //Keywords
    AND,OR,CLASS,IF,ELSE,TRUE,FALSE,FUN,FOR,NIL,
    PRINT,RETURN,SUPER,THIS,VAR,WHILE,

    _EOF

};
class Token{
    public:
    TokenType type;
    int line;
    string value;
    Token(){
        this->type = TokenType::_EOF;
        this->line = -1;
        this->value = "";
    }
    Token(TokenType type,string value,int line){
        this->type = type;
        this->line = line;
        this->value = value;
    }
    void operator=(const Token &t){
        this->type = t.type;
        this->line = t.line;
        this->value = t.value;
    }
    void printToken(){
        cout<<enumToString(type)<<" "<<value<<endl;
    }
    string enumToString(TokenType type){
        switch(type){
            case TokenType::EQUAL_EQUAL:return "EQUAL_EQUAL";break;
            case TokenType::AND:return "AND";break;
            case TokenType::OR:return "OR";break;
            case TokenType::IF:return "IF";break;
            case TokenType::_EOF:return "_EOF";break;
            case TokenType::CLASS:return "CLASS";break;
            case TokenType::IDENTIFIER:return "IDENTIFIER";break;
            case TokenType::BANG:return "BANG";break;
            case TokenType::BANG_EQUAL:return "BANG_EQUAL";break;
            case TokenType::LESSER:return "LESSER";break;
            case TokenType::LESSER_EQUAL:return "LESSER_EQUAL";break;
            case TokenType::GREATER:return "GREATER";break;
            case TokenType::GREATER_EQUAL:return "GREATER_EQUAL";break;
            case TokenType::EQUAL:return "EQUAL";break;
            case TokenType::COMMA:return "COMMA";break;
            case TokenType::DOT:return "DOT";break;
            case TokenType::ELSE:return "ELSE";break;
            case TokenType::FUN:return "FUN";break;
            case TokenType::FALSE:return "FALSE";break;
            case TokenType::TRUE:return "TRUE";break;
            case TokenType::FOR:return "FOR";break;
            case TokenType::LEFT_BRACE:return "LEFT_BRACE";break;
            case TokenType::RIGHT_BRACE:return "RIGHT_BRACE";break;
            case TokenType::LEFT_PAREN:return "LEFT_PAREN";break;
            case TokenType::RIGHT_PAREN:return "RIGHT_PAREN";break;
            case TokenType::MINUS:return "MINUS";break;
            case TokenType::PLUS:return "PLUS";break;
            case TokenType::STAR:return "STAR";break;
            case TokenType::SLASH:return "SLASH";break;
            case TokenType::NIL:return "NIL";break;
            case TokenType::NUMBER:return "NUMBER";break;
            case TokenType::RETURN:return "RETURN";break;
            case TokenType::SEMICOLON:return "SEMICOLON";break;
            case TokenType::STRING:return "STRING";break;
            case TokenType::SUPER:return "SUPER";break;
            case TokenType::THIS:return "THIS";break;
            case TokenType::VAR:return "VAR";break;
            case TokenType::WHILE:return "WHILE";break;
            case TokenType::PRINT:return "PRINT";break;
            default: return "";
        }
    }
};

};