
#include "parser.hpp"
#include "env.hpp"

class Interpreter : public Visitor{

    string evaluate(Expr *expr){
        return expr->accept(this);
    }
    string execute(Stmt*stmt){
        return stmt->accept(this);
    }

    void executeBlock(vector<Stmt*>statements,Env*env){
        Env*previous = this->env;
        try{
            this->env = env;
            for(auto &stmt : statements){
                evals.push_back(execute(stmt));
            }
        }
        catch(string error){
            cout<<error<<endl;
        }
        this->env = previous;
    }    
    Env*env;
    vector<string>evals;
    public:
    Interpreter(){
        env = new Env();
        evals = vector<string>();
    }
    vector<string> interpret(vector<Stmt*>statements){
         for(auto &stmt : statements){
            evals.push_back(execute(stmt));
         }
         return evals;
    }
    string visitExpressionStmt(const Expression*stmt)override{
        return evaluate(stmt->expression);
    }
    string visitPrintStmt(const Print*stmt)override{
        string value = evaluate(stmt->expression);
        return value;
    }
    string visitLiteral(const Literal *expr) override{
        return expr->value;
    }

    string visitBinary(const Binary *expr)override{
        string left = evaluate(expr->left);
        string right = evaluate(expr->right);

        if(expr->op.type == TokenType::PLUS){
            return to_string(stod(left) + stod(right));
        }
        else if(expr->op.type == TokenType::MINUS){
            return to_string(stod(left) - stod(right));
        }
        else if(expr->op.type == TokenType::STAR){
            return to_string(stod(left) * stod(right));
        }
        else if(expr->op.type == TokenType::SLASH){
            return to_string(stod(left) / stod(right));
        }
        else if(expr->op.type == TokenType::GREATER){
            return to_string(stod(left) > stod(right));
        }
        else if(expr->op.type == TokenType::GREATER_EQUAL){
            return to_string(stod(left) >= stod(right)) == "0"?"False":"True";
        }
        else if(expr->op.type == TokenType::LESSER){
            return to_string(stod(left) < stod(right)) == "0"?"False":"True";
        }
        else if(expr->op.type == TokenType::LESSER_EQUAL){
            return to_string(stod(left) <= stod(right)) == "0"?"False":"True";
        }
        else if(expr->op.type == TokenType::EQUAL_EQUAL){
            return to_string(stod(left) == stod(right)) == "0"?"False":"True";
        }
        else if(expr->op.type == TokenType::BANG_EQUAL){
            return to_string(stod(left) != stod(right)) == "0"?"False":"True";
        }
    }

    string visitUnary(const Unary *unary)override{
        string expr = evaluate(unary->right);
        if(unary->op.type == TokenType::MINUS){
            return to_string(-stod(expr));
        }
        else if(unary->op.type == TokenType::BANG){
            return to_string(!stod(expr));
        }
    }

    string visitGrouping(const Grouping*grouping) override{
        return evaluate(grouping->expression);
    }

    string visitVar(const Var*var) override{
        string value = "";
        if(var->expression != NULL){
            value = evaluate(var->expression);
        }
        env->define(var->name.value,value);
        return "";
    }

    string visitVariable(const Variable *var)override{
        return env->get(var->name.value);
    }

    string visitAssign(const Assign *expr)override{
        string value = evaluate(expr->value);
        env->assign(expr->name.value,value);
        return "";
    }

    string visitBlock(const Block *block)override{
        executeBlock(block->statements,new Env(this->env));
        return "";
    }

    string visitIf(const IfStmt *stmt)override{
        string value = evaluate(stmt->condition);
        if(value != "False"){
            return execute(stmt->thenBranch);
        }
        else{
            return execute(stmt->elseBranch);
        }
    }

    string visitLogical(const Logical*expr)override{
        string left = evaluate(expr->left);

        if(expr->op.type == Token::TokenType::OR){
            if(left != "False") return left;
        }
        else{
            if(left == "False")return left;
        }

        return evaluate(expr->right);
    }

    string visitWhile(const WhileStmt *stmt)override{
        while(evaluate(stmt->conditon) != "False"){
            evals.push_back(execute(stmt->body));
        }
        return "";
    }

};