#include "scanner.hpp"
#include <string.h>


namespace Parser{

class Parser{
    vector<Token::Token>tokens;
    int current;

    template<class Base,class T>
    inline bool instanceOf(const T *ptr){
        return dynamic_cast<const Base*>(ptr) != NULL;
    }

    bool isAtEnd(){
       return tokens[current].type == TokenType::_EOF;
    }
    Token::Token previous(){
        return tokens.at(current - 1);
    }
    Token::Token advance(){
        if(!isAtEnd())current++;
        return previous();
    }
    Token::Token peek(){
        return tokens.at(current);
    }
    bool check(Token::TokenType type){
        if(isAtEnd())return false;
        return tokens.at(current).type == type;
    }
    
    bool match(vector<Token::TokenType>types){
        for(auto &type : types){
            if(check(type)){
                advance();
                return true;
            }
        }
        return false;
    }

    Token::Token consume(Token::TokenType type,string message){
        if(check(type))return advance();
        throw message;
    }
    Expr* expression(){

        return assignment();
    }
    Expr* assignment(){
        // lvalue = rvalue
        Expr*expr = Or();
        if(match({Token::TokenType::EQUAL})){
            Expr*value = assignment();

            if(instanceOf<Variable>(expr)){
                Token::Token name = (dynamic_cast<const Variable*>(expr))->name;
                return new Assign(name,value);
            }
            string error = "Invalid assignment target.";
            throw error;
        }
        return expr;
    }
    Expr* Or(){
        Expr *expr = And();
        while(match({Token::TokenType::OR})){
            Token::Token op = previous();
            Expr*right = And();
            return new Logical(expr,right,op);
        }
        return expr;
    }
    Expr* And(){
        Expr *expr = equality();
        while(match({Token::TokenType::AND})){
            Token::Token op = previous();
            Expr*right = And();
            return new Logical(expr,right,op);
        }
        return expr;
    }
    Expr* equality(){
        Expr* expr = comparison();
        while(match({Token::TokenType::EQUAL_EQUAL,Token::TokenType::BANG_EQUAL})){
            Token::Token op = previous();
            Expr* right = comparison();
            expr = new Binary(expr,op,right);
        }
        return expr;
    }

    Expr* comparison(){
        Expr* expr = term();
        while(match({Token::TokenType::GREATER,Token::TokenType::GREATER_EQUAL,Token::TokenType::LESSER,Token::TokenType::LESSER_EQUAL})){
            Token::Token op = previous();
            Expr*right = term();
            expr = new Binary(expr,op,right);
        }
        return expr;
    }
    Expr* term(){
        Expr*expr = factor();
        while(match({Token::TokenType::MINUS,Token::TokenType::PLUS})){
            Token::Token op = previous();
            Expr*right = factor();
            expr = new Binary(expr,op,right);
        }
        return expr;
    }
    Expr* factor(){
        Expr* expr = unary();
        while(match({Token::TokenType::STAR,Token::TokenType::SLASH})){
            Token::Token op = previous();
            Expr *right = unary();
            expr = new Binary(expr,op,right);
        }
        return expr;
    }

    Expr* unary(){
        if(match({Token::TokenType::BANG,Token::TokenType::MINUS})){
            Token::Token op = previous();
            Expr *right = unary();
            return new Unary(op,right);
        }
        return primary();
    }
    Expr* primary(){
        if(match({Token::TokenType::FALSE}))return new Literal("false");
        else if(match({Token::TokenType::TRUE}))return new Literal("true");
        else if (match({Token::TokenType::NIL}))return new Literal("");
        else if(match({TokenType::IDENTIFIER}))return new Variable(previous());
        else if(match({Token::TokenType::NUMBER,Token::TokenType::STRING})){
            return new Literal(previous().value);
        }
        else if(match({Token::TokenType::LEFT_PAREN})){
            Expr*expr = expression();
            consume(Token::TokenType::RIGHT_PAREN,"Expect ')' after expression");
            return new Grouping(expr);
        }
        else{
            string error = "Expect Expression!";
            throw error;
        }
    }

    vector<Stmt*> block(){
        vector<Stmt*>statements;
        while(!check(TokenType::RIGHT_BRACE) && !isAtEnd()){
            statements.push_back(declaration());
        }
        consume(TokenType::RIGHT_BRACE,"Expected } at the end");
        return statements;
    }

   
    Stmt*declaration(){
        if(match({TokenType::VAR})){
            return varDeclaration();
        }
        return statement();
    }

    Stmt*varDeclaration(){
        Token::Token name = consume(TokenType::IDENTIFIER,"Expect Variable name");
        Expr*expr = NULL;
        if(match({TokenType::EQUAL})){
            expr = expression();
        }
        consume(TokenType::SEMICOLON,"Expected ';' at the end");
        return new Var(name,expr);
    }
    Stmt*statement(){
        if(match({TokenType::PRINT})){
            return printStatement();
        }
        else if(match({TokenType::FOR})){
            return forStatement();
        }
        else if(match({TokenType::IF})){
           return ifstatement();
        }
        else if(match({TokenType::LEFT_BRACE}))return new Block(block());
        else if(match({TokenType::WHILE}))return whileStatement();
        return expressionStatement();
    }
    Stmt* forStatement(){
        consume(TokenType::LEFT_PAREN,"Expected '(' after 'for'");
        Stmt* initializer;
        if(match({TokenType::SEMICOLON})){
            initializer = NULL;
        }
        else if(match({TokenType::VAR})){
            initializer = varDeclaration();
        }
        else{
            initializer = expressionStatement(); 
        }
        Expr* condition = NULL;
        if(!check(TokenType::SEMICOLON)){
            condition = expression();
        }
        consume(TokenType::SEMICOLON,"Expect ';' after loop condition.");
        Expr*increment = NULL;
        if(!check(TokenType::RIGHT_PAREN)){
            increment = expression();
        }
        consume(TokenType::RIGHT_PAREN,"Expect ')' after increment expression");
        Stmt*body = statement();

        if(increment != NULL){
            body = new Block({body,new Expression(increment)});
        }

        if(condition == NULL)condition = new Literal("true");
        body = new WhileStmt(condition,body);

        if(initializer != NULL){
            body = new Block({initializer,body});
        }
        return body;
    }
    Stmt* ifstatement(){
        consume(TokenType::LEFT_PAREN,"Expected ( ");
        Expr*condition = expression();
        consume(TokenType::RIGHT_PAREN,"Expected ) ");
        Stmt* thenBranch = statement();
        Stmt* elseBranch = NULL;
        if(match({TokenType::ELSE})){
            elseBranch = statement();
        }
        return new IfStmt(condition,thenBranch,elseBranch);

    }
    Stmt *whileStatement(){
        consume(Token::TokenType::LEFT_PAREN,"Expected (");
        Expr*condition = expression();
        consume(Token::TokenType::RIGHT_PAREN,"Expected )");
        Stmt*body = statement();
        return new WhileStmt(condition,body);
    }
    Stmt*printStatement(){
        Expr*value = expression();
        consume(TokenType::SEMICOLON,"Expected ';' after value");
        return new Print(value);
    }

    Stmt*expressionStatement(){
        Expr*value = expression();
        consume(TokenType::SEMICOLON,"Expected ';' after value");
        return new Expression(value);
    }
    public:
    vector<Stmt*> parse(){
        try{
            vector<Stmt*>statements;
            while(!isAtEnd()){
                statements.push_back(declaration());
            }
            return statements;
        }
        catch(string error){
            cout<<error<<endl;
            return vector<Stmt*>();
        }
    }
    Parser(vector<Token::Token>tokens){
        current = 0;
        this->tokens = tokens;
    }
};

};