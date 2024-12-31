#include<bits/stdc++.h>
#pragma GCC optimize("Ofast")
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
    NUM, PLUS, MINUS, MUL, DIV, LPR, RPR, END
};
string TokenTypeNames[] = {
    "NUM", "PLUS", "MINUS", "MUL", "DIV", "LPR", "RPR"
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


    void tokenize(string str){
        size_t pos = 0, n = str.size();
        smatch match;
        while (pos < n) {
            if(str[pos] == ' '){
                pos++;
            }
            else if(str[pos] == '+'){
                addToken(PLUS, "+");
                pos++;
            }
            else if(str[pos] == '-'){
                addToken(MINUS, "-");
                pos++;
            }
            else if(str[pos] == '*'){
                addToken(MUL, "*");
                pos++;
            }
            else if(str[pos] == '/'){
                addToken(DIV, "/");
                pos++;
            }
            else if(str[pos] == '('){
                addToken(LPR, "(");
                pos++;
            }
            else if(str[pos] == ')'){
                addToken(RPR, ")");
                pos++;
            }
            else if(isdigit(str[pos])){
                string tmp = string(1, str[pos++]);
                while(pos < n && isdigit(str[pos]))
                    tmp += str[pos++];
                addToken(NUM, tmp);
            }
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
            cout << TokenTypeNames[token.type] << " ";
            if(token.type == NUM)
                cout << token.lexeme;
            cout << '\n';
        }

    }
}