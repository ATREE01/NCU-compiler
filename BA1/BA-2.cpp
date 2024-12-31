#include<bits/stdc++.h>
// #pragma GCC optimize("Ofast")
using namespace std;

#define F first
#define S second
#define pb push_back
#define all(x) x.begin(),x.end()
#define FIO ios::sync_with_stdio(0),cin.tie(0);
#define debug(x) cerr<< #x << " = " << x << '\n';
#define line() cerr << "\n----------------------------------\n";

using ll = long long;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
const int MAXN = 2e5 + 5;
const int MOD = 1e9 + 7;

// Token types
enum TokenType {
    NUM, IDENTIFIER, SYMBOL, KEYWORD, INVALID, END
};
string TokenTypeNames[] = {
    "NUM", "IDENTIFIER", "SYMBOL", "KEYWORD", "Invalid"
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

    bool isIdChar(char ch){
        return isalpha(ch) || isdigit(ch) || ch == '_';
    }

    bool checkKeyWork(string s){
        return s == "if" || s == "while";
    }

    void tokenize(string str){
        size_t pos = 0, n = str.size();
        smatch match;

        while (pos < n) {
            if(str[pos] == ' ' || str[pos] == '\r' || str[pos] == '\n'){
                pos++;
            }
            else if(str[pos] == '+' || str[pos] == '-' || str[pos] == '*' || str[pos] == '/' || str[pos] == '=' ||
                str[pos] == '(' || str[pos] == ')' || str[pos] == '<' || str[pos] == '>' || str[pos] == '{' || str[pos] == '}' ||
                str[pos] == ';'){
                addToken(SYMBOL, string(1, str[pos]));
                pos++;
            }
            else if(isdigit(str[pos])){
                if(str[pos] == '0'){
                    addToken(NUM, string(1, str[pos++]));
                    continue;
                }
                string tmp = string(1, str[pos++]);
                while(pos < n && isdigit(str[pos]))
                    tmp += str[pos++];
                addToken(NUM, tmp);
            }
            else if(isIdChar(str[pos])){
                bool flag = true;
                string tmp = string(1, str[pos++]);
                while(pos < n && isIdChar(str[pos])){
                    tmp += str[pos++];
                    if(checkKeyWork(tmp)){
                        addToken(KEYWORD, tmp);
                        flag = false;
                        break;
                    }
                }
                if(flag)
                    addToken(IDENTIFIER, tmp);
            }
            else 
                addToken(INVALID, string(1, str[pos++]));
            
        }
    }

    void addToken(TokenType type, string lexeme) {
        tokens.push_back(Token(type, lexeme));
    }
};

int main(){
    FIO;
    string s;
    while(getline(cin, s)){
        Lexer lexer(s);
        for(Token token: lexer.tokens){
            cout << TokenTypeNames[token.type];
            if(token.type != INVALID)
                cout << " \"" << token.lexeme << "\"";
            cout << '\n';
        }

    }
}