#include <iostream>
#include <string>
#include <vector>
#include <regex>

using namespace std;

// Token types
enum TokenType {
    PHONENUM, PATH, MAILDOMAIN, DOMAIN, SCHEME, COLON, AT, DOT, SLASH, END, INVALID_INPUT
};
string TokenTypeNames[] = {
    "PHONENUM", "PATH", "MAILDOMAIN", "DOMAIN", "SCHEME", "COLON", "AT","DOT", "SLASH", "END", "invalid input"
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

    bool checkMailDomain(string s){
        return s == "gmail" || s == "yahoo" || s == "iCloud" || s == "outlook";
    }

    bool checkDomain(string s){
        return s == "org" || s == "com";
    }

    bool checkScheme(string s){
        return s == "https" || s == "tel" || s == "mailto";
    }

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
            else if(str[pos] == ':'){
                addToken(COLON, ":");
                pos++;
            }
            else if(str[pos] == '@'){
                addToken(AT, "@");
                pos++;
            }
            else if(str[pos] == '.'){
                addToken(DOT, ".");
                pos++;
            }
            else if(str[pos] == '.'){
                addToken(DOT, ".");
                pos++;
            }
            else if(str[pos] == '/'){
                addToken(SLASH, "/");
                pos++;
            }
            else if(str[pos] == '0'){
                pos += 1;
                string lexeme = "0";
                while(pos < n && isdigit(str[pos]))
                    lexeme += str[pos++];
                if(lexeme[1] == '9' && lexeme.size() == 10)
                    addToken(PHONENUM, lexeme);
                else 
                    addToken(PATH, lexeme);
            }
            else if(isalpha(str[pos]) || isdigit(str[pos])){
                bool flag = true;
                string lexeme = string(1, str[pos++]);
                while(pos < n && (isalpha(str[pos]) || isdigit(str[pos]))){
                    lexeme += str[pos++];
                    if(checkScheme(lexeme)){
                        addToken(SCHEME, lexeme);
                        flag = false;
                        break;
                    } else if(checkMailDomain(lexeme)){
                        addToken(MAILDOMAIN, lexeme);
                        flag = false;
                        break;
                    } else if(checkDomain(lexeme)){
                        addToken(DOMAIN, lexeme);
                        flag = false;
                        break;
                    }
                }
                if(flag)
                    addToken(PATH, lexeme);
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
    stmt();
    if(lexer.peek().type == END)
        return;
    else if(lexer.peek().type == INVALID_INPUT){
        error = true;
        lexer.clear();
        return;
    }
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
    else if(token.type == PHONENUM){
        lexer.next();
        ans.push_back(token);
    }
    else if(token.type == PATH)
        mail();
    else if(token.type == SCHEME)
        uri();

}   

vector<TokenType>mailPattern = {PATH, AT, MAILDOMAIN, DOT, DOMAIN};
void mail(){
    Token token = lexer.next();
    for(int i = 0;i < mailPattern.size();i++){
        if(token.type == mailPattern[i]){
            ans.push_back(token);
            if(i == mailPattern.size() - 1)
                return;
            token = lexer.next();
        }
        else{
            error = true;
            lexer.clear();
            return;
        }
    }
}

vector<TokenType> uriPattern1 = {SCHEME, COLON, SLASH, SLASH, PATH, DOT, DOMAIN};
vector<TokenType> uriPattern2 = {SCHEME, COLON, PATH, AT, MAILDOMAIN, DOT, DOMAIN};
vector<TokenType> uriPattern3 = {SCHEME, COLON, PHONENUM};
bool matchPattern(vector<TokenType> pattern) {
    int startIdx = lexer.currenctIdx;
    for (TokenType type : pattern) {
        if (lexer.next().type != type) {
            lexer.currenctIdx = startIdx;
            return false;
        }
    }
    lexer.currenctIdx = startIdx;
    return true;
}

void uri(){
    if(matchPattern(uriPattern1)) {
        for(int i = 0;i < uriPattern1.size();i++)
            ans.push_back(lexer.next());
    } else if (matchPattern(uriPattern2)) {
        for(int i = 0;i < uriPattern2.size();i++)
            ans.push_back(lexer.next());
    } else if (matchPattern(uriPattern3)) {
        for(int i = 0;i < uriPattern3.size();i++)
            ans.push_back(lexer.next());
    } else {
        error = true;
        lexer.clear();
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
            cout << "Invalid input\n";
        else 
            for(auto token : parser.ans){
                cout << token.lexeme << " " << TokenTypeNames[token.type] << "\n";
            }

    }
}
// tel:0912123459