#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Token types
enum TokenType {
    ID, STRLIT, LBR, RBR, DOT, END, INVALID_INPUT
};
string TokenTypeNames[] = {
    "ID", "STRLIT", "LBR", "RBR", "DOT", "END", "invalid input"
};

// Token structure
struct Token {
public:
    TokenType type;
    string lexeme;
    Token(TokenType type, string lexeme) : type(type), lexeme(lexeme) {}
};

class Lexer{
public:
    string str;
    vector<Token> tokens;
    int currenctIdx = 0;

    Lexer(string str) : str(str){
        tokenize(str);
    }

    Token peek(){// get the current token
        if(currenctIdx < tokens.size())
            return tokens[currenctIdx];
        return Token(END, "");
    }

    Token next(){// get the current token and move index to next one
        if(currenctIdx < tokens.size())
            return tokens[currenctIdx++];
        return Token(END, "");
    }

    void clear(){
        tokens.clear();
        currenctIdx = 0;
    }

private:

    bool validIdChar(char c){
        return isalpha(c) || isdigit(c) || c == '_';
    }

    void tokenize(string str){
        size_t pos = 0, n = str.size();
        smatch match;
        while (pos < n) {
            if(str[pos] == ' '){
                pos++;
            }
            else if(str[pos] == '('){
                addToken(LBR, "(");
                pos++;
            }
            else if(str[pos] == ')'){
                addToken(RBR, ")");
                pos++;
            }
            else if(str[pos] == '.'){
                addToken(DOT, ".");
                pos++;
            }
            else if(str[pos] == '"'){
                string lexeme = "\"";
                pos++;
                while (str[pos] != '"' && pos < n)
                    lexeme += str[pos++];
                if(str[pos] != '"'){
                    addToken(INVALID_INPUT, lexeme);
                    return;
                }
                lexeme += str[pos++];
                addToken(STRLIT, lexeme);
            }
            else if(validIdChar(str[pos])){
                string tmp = string(1, str[pos++]);
                while(pos < n && validIdChar(str[pos]))
                    tmp += str[pos++];
                addToken(ID, tmp);
            }
            else {
                addToken(INVALID_INPUT, string(1, str[pos++]));
                return;
            }
        }
    }

    void addToken(TokenType type, string lexeme) {
        tokens.push_back(Token(type, lexeme));
    }
};

class Parser{
private: 

Lexer &lexer;

void program(){
    stmts();
}

void stmts() {
    if(lexer.peek().type == END)
        return;
    stmt();
    stmts();
}

void stmt(){
    Token token = lexer.peek();
    if(token.type == END)
        return;
    else if(token.type == INVALID_INPUT){
        error = true;
        lexer.clear();
        return;
    }
    else if(token.type == ID){
        primary();
    }
    else if(token.type == STRLIT){
        lexer.next();
        ans.push_back(token);
        return;
    }
}

void primary(){
    Token token = lexer.next();
    if(token.type == INVALID_INPUT){
        error = true;
        lexer.clear();
        return;
    }
    else if(token.type == ID){
        ans.push_back(token);
        primary_tail();
    } 
}

void primary_tail() {
    Token token = lexer.next();
    if(token.type == END){
        return;
    }
    else if(token.type == INVALID_INPUT){
        error = true;
        lexer.clear();
        return;
    }
    else if(token.type == DOT && lexer.peek().type == ID){
        ans.push_back(token);
        ans.push_back(lexer.next());
        primary_tail();
    }
    else if(token.type == LBR){
        ans.push_back(token);
        stmt();
        if(lexer.peek().type == RBR){
            ans.push_back(lexer.next());
            primary_tail();
        }
        else{
            error = true;
            lexer.clear();
            return;
        }
    }
}

public:
    vector<Token> ans;
    bool error = false;
    Parser(Lexer &lexer): lexer(lexer){
        program();
    }
};

int main(){
    string s;
    while(getline(cin, s)){
        Lexer lexer(s);

        Parser parser(lexer);
        if(parser.error)
            cout << "invalid input\n";
        else 
            for(auto token : parser.ans){
                cout << TokenTypeNames[token.type] << " " << token.lexeme << "\n";
            }

    }
}