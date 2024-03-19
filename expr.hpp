
#include "token.hpp"
#include <string.h>

class Expr;
class Binary;
class Grouping;
class Unary;
class Literal;
class Expression;
class Print;
class Var;
class Variable;
class Assign;
class Block;
class IfStmt;
class Logical;
class WhileStmt;
class Visitor{
    public:
    virtual string visitBinary(const Binary* binary)   = 0;
    virtual string visitUnary(const Unary* unary) = 0;
    virtual string visitGrouping(const Grouping* grouping)  = 0;
    virtual string visitLiteral(const Literal* literal)  = 0;
    virtual string visitExpressionStmt(const Expression*expression) = 0;
    virtual string visitPrintStmt(const Print *printExpr) = 0;
    virtual string visitVariable(const Variable*varExpr) = 0;
    virtual string visitVar(const Var*var) = 0;
    virtual string visitAssign(const Assign*expr) = 0;
    virtual string visitBlock(const Block*block) = 0;
    virtual string visitIf(const IfStmt*stmt) = 0;
    virtual string visitLogical(const Logical*expr) = 0;
    virtual string visitWhile(const WhileStmt*stmt) = 0;
};
class Expr{
    public:
    virtual ~Expr() {}
    virtual string accept(Visitor* visitor) const = 0;
    
};

class Binary : public Expr{
    public:
    Expr *left,*right;
    Token::Token op;
    Binary(Expr *left,Token::Token op,Expr *right){
        this->left = left;
        this->op = op;
        this->right = right;
    }
   string accept(Visitor *visitor)const override{
        return visitor->visitBinary(this);
   }
};

class Grouping : public Expr{
    public:
    Expr *expression;
    Grouping(Expr *expression){
        this->expression = expression;
    }
    string accept(Visitor *visitor) const override{
        return visitor->visitGrouping(this);
    }
};

class Literal : public Expr{
    public:
    string value;
    Literal(string value){
        this->value = value;
    } 
    string accept(Visitor *visitor) const override{
        return visitor->visitLiteral(this);
    }
};

class Unary : public Expr{
    public:
    Token::Token op;
    Expr *right;
    Unary(Token::Token op,Expr *right){
        this->op = op;
        this->right = right;
    }
    string accept(Visitor *visitor) const override{
        return visitor->visitUnary(this);
    }
};


class Variable : public Expr{
    public:
    Token::Token name;
    Variable(Token::Token name){
        this->name = name;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitVariable(this);
    }
};

class Assign : public Expr{
    public:
    Token::Token name; 
    Expr*value;
    Assign(Token::Token name,Expr*value){
        this->name = name;
        this->value = value;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitAssign(this);
    }
};


class Logical : public Expr{
    public:
    Token::Token op;
    Expr*left,*right;
    Logical(Expr*left,Expr*right,Token::Token op){
        this->left = left;
        this->right = right;
        this->op = op;
    }
    string accept(Visitor *visitor)const override{
        return visitor->visitLogical(this);
    }
};  