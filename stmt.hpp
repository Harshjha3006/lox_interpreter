#include "expr.hpp"


class Stmt{
    public:
    virtual ~Stmt() {}
    virtual string accept(Visitor* visitor) const = 0;

};

class Expression : public Stmt{
    public:
    Expr*expression;
    Expression(Expr *expression){
        this->expression = expression;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitExpressionStmt(this);
    }
};

class Print : public Stmt{
    public:
    Expr *expression;
    Print(Expr *expression){
        this->expression = expression;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitPrintStmt(this);
    }
};


class Var : public Stmt{
    public:
    Token::Token name;
    Expr*expression;
    Var(Token::Token name,Expr*expression){
        this->name = name;
        this->expression = expression;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitVar(this);
    }
};

class Block : public Stmt{
    public:
    vector<Stmt*>statements;
    Block(vector<Stmt*>statements){
        this->statements = statements;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitBlock(this);
    }
};

class IfStmt : public Stmt{
    public:
    Expr*condition;
    Stmt*thenBranch,*elseBranch;
    IfStmt(Expr *condition,Stmt *thenBranch,Stmt *elseBranch){
        this->condition = condition;
        this->thenBranch = thenBranch;
        this->elseBranch = elseBranch;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitIf(this);
    }
};

class WhileStmt : public Stmt{
    public:
    Expr*conditon;
    Stmt*body;
    WhileStmt(Expr*condition,Stmt*body){
        this->conditon = condition;
        this->body = body;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitWhile(this);
    }
};