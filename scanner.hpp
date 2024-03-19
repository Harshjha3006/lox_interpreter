#include "stmt.hpp"
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
using namespace std;
using namespace Token;

namespace Scanner{
class Scanner
{
private:
    static unordered_map<string, TokenType> keywords;
    string source;
    vector<Token::Token> tokens;
    int start, current, line;
    static void error(int line, string message)
    {
        cerr << "[Line " << line << "] Error : " << message << endl;
    }

    bool isAtEnd()
    {
        return current >= source.length();
    }

    char advance()
    {
        return source.at(current++);
    }
    void addToken(TokenType type)
    {
        addToken(type, "");
    }
    void addToken(TokenType type, string value)
    {
        string lexeme = source.substr(start, current - start);
        tokens.push_back(Token::Token(type,value, line));
    }
    bool match(char expected)
    {
        if (isAtEnd())
            return false;
        if (source.at(current) != expected)
        {
            return false;
        }
        current++;
        return true;
    }
    char peek()
    {
        if (isAtEnd())
            return '\0';
        return source.at(current);
    }
    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }
    void extractString()
    {
        while (peek() != '"' && !isAtEnd())
        {
            if (peek() == '\n')
                line++;
            advance();
        }
        if (isAtEnd())
        {
            error(line, "Unterminated String");
            return;
        }
        advance();
        string lexeme = source.substr(start + 1, current - start - 2);
        addToken(TokenType::STRING, lexeme);
    }
    char peekNext()
    {
        if (current + 1 >= source.length())
            return '\0';
        return source.at(current + 1);
    }
    void extractNum()
    {
        while (isDigit(peek()))
        {
            advance();
        }
        if (peek() == '.' && isDigit(peekNext()))
        {
            advance();
            while (isDigit(peek()))
                advance();
        }
        addToken(TokenType::NUMBER, source.substr(start, current - start));
    }
    bool isAlpha(char c)
    {
        if ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_')
        {
            return true;
        }
        return false;
    }
    bool isAlphaNum(char c)
    {
        return isAlpha(c) || isDigit(c);
    }
    void extractIdentifier()
    {
        while (isAlphaNum(peek()))
            advance();
        string value = source.substr(start, current - start);
        if (keywords.find(value) != keywords.end())
        {
            TokenType type = keywords.at(value);
            addToken(type);
        }
        else
        {
            addToken(TokenType::IDENTIFIER, value);
        }
    }
    void scanToken()
    {
        char c = advance();
        switch (c)
        {
        case '(':
            addToken(TokenType::LEFT_PAREN,"(");
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN,")");
            break;
        case '{':
            addToken(TokenType::LEFT_BRACE,"{");
            break;
        case '}':
            addToken(TokenType::RIGHT_BRACE,"}");
            break;
        case ',':
            addToken(TokenType::COMMA,",");
            break;
        case '.':
            addToken(TokenType::DOT,".");
            break;
        case '-':
            addToken(TokenType::MINUS,"-");
            break;
        case '+':
            addToken(TokenType::PLUS,"+");
            break;
        case ';':
            addToken(TokenType::SEMICOLON,";");
            break;
        case '*':
            addToken(TokenType::STAR,"*");
            break;
        case '!':
            if(match('=')){
                addToken(Token::TokenType::BANG_EQUAL,"!=");
            }
            else{
                addToken(Token::TokenType::BANG,"!");
            }
            break;
        case '>':
            if(match('=')){
                addToken(Token::TokenType::GREATER_EQUAL,">=");
            }
            else{
                addToken(Token::TokenType::GREATER,">");
            }
            break;
        case '<':
            if(match('=')){
                addToken(Token::TokenType::LESSER_EQUAL,"<=");
            }
            else{
                addToken(Token::TokenType::LESSER,"<");
            }
            break;
        case '=':
            if(match('=')){
                addToken(Token::TokenType::EQUAL_EQUAL,"==");
            }
            else{
                addToken(Token::TokenType::EQUAL,"=");
            }
            break;
        case '/':
            if (match('/'))
            {
                while (peek() != '\n' && !isAtEnd())
                    advance();
            }
            else
            {
                addToken(TokenType::SLASH,"/");
            }
            break;
        case ' ':
            break;
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '\r':
            break;
        case '"':
            extractString();
            break;
        default:
            if (isDigit(c))
            {
                extractNum();
            }
            else if (isAlpha(c))
            {
                extractIdentifier();
            }
            else
            {
                error(line, "Unexpected Character");
            }
        }
    }

public:
    Scanner(string source)
    {
        this->source = source;
        start = 0, current = 0;
        line = 1;
        tokens = vector<Token::Token>();
    }

    vector<Token::Token> scanTokens()
    {
        while (!isAtEnd())
        {
            start = current;
            scanToken();
        }
        tokens.push_back(Token::Token(TokenType::_EOF,(string) "", line));
        return tokens;
    }
   
};

unordered_map<string, TokenType> Scanner::keywords = {{"and", TokenType::AND}, {"class", TokenType::CLASS}, {"else", TokenType::ELSE}, {"false", TokenType::FALSE}, {"for", TokenType::FOR}, {"fun", TokenType::FUN}, {"if", TokenType::IF}, {"nil", TokenType::NIL}, {"or", TokenType::OR}, {"print", TokenType::PRINT}, {"return", TokenType::RETURN}, {"super", TokenType::SUPER}, {"this", TokenType::THIS}, {"true", TokenType::TRUE}, {"var", TokenType::VAR}, {"while", TokenType::WHILE}};
};