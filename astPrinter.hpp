#include "parser.hpp"
#include<string.h>
#include<vector>

class AstPrinter : public Visitor{
    
   
    string parenthesize(string name,vector<Expr *>exprs){
        string res = "";
        res += "(";
        res += name;
        for(auto expr : exprs){
            res += " ";
            res.append(expr->accept(this));
        }
        res += ")";
        return res;
    }
    public:
    string print(Expr *expr){
        return expr->accept(this);
    }
    string visitBinary(const Binary*binary)override {
        return parenthesize(binary->op.value,{binary->left,binary->right});
    }
    string visitGrouping(const Grouping*grouping) override  {
        return parenthesize("group",{grouping->expression});
    }
    string visitLiteral(const Literal*literal)override {
        if(literal->value == "")return "nil";
        return literal->value;
    }
    string visitUnary(const Unary *unary)override{
        return parenthesize(unary->op.value,{unary->right});
    }
};