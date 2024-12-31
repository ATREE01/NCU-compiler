#include<bits/stdc++.h>

using namespace std;

// Structure to hold grammar productions
struct Production {
    char nonTerminal;
    vector<string> rules; // Each rule is a string of symbols
};

// Function to calculate the First Set
set<char> calculateFirst(char nonTerminal, map<char, Production>& grammar, 
                         map<char, set<char>>& firstSet, bitset<30>& visited) {

    // If already visited, return empty set to avoid infinite recursion
    if (visited[nonTerminal - 'A']) return {};
    visited[nonTerminal - 'A'] = 1;

    set<char> first;
    // Get the production for the non-terminal
    if (grammar.find(nonTerminal) != grammar.end()) {
        for (const auto& rule : grammar[nonTerminal].rules) {
            if (rule.empty()) continue; // Skip empty rules

            // this flag means where to keep account of the first set in the same | seperated rule
            bool takeAccount = true; 
            for (int i = 0 ; i < rule.size(); i++) {
                char symbol = rule[i];

                if(symbol == '|'){
                    takeAccount = true;
                    visited.reset();
                    continue;
                }

                if(!takeAccount) 
                    continue;
                
                if (isalpha(symbol) && islower(symbol)) {
                    first.insert(symbol);
                    takeAccount = false;
                }
                else if (isupper(symbol)) {
                    set<char> firstOfSymbol = calculateFirst(symbol, grammar, firstSet, visited);
                    takeAccount = false;

                    if (firstOfSymbol.count(';')) {
                        takeAccount = true;
                        // for the beta beta_n rule
                        if(!(i == rule.size() - 1 || rule[i+1] == '|')){
                            firstOfSymbol.erase(';');
                        }
                    }
                    first.insert(firstOfSymbol.begin(), firstOfSymbol.end());
                }
                else if(symbol == ';' || symbol == '$')
                    first.insert(symbol); 
            }
        }
    }

    // Store the calculated first set
    firstSet[nonTerminal].insert(first.begin(), first.end());
    return first;
}

int main() {
    // Define the grammar
    map<char, Production> grammar;

    string noneTerminal, rule;
    while(cin >> noneTerminal){
        if(noneTerminal == "END_OF_GRAMMAR") break;
        cin >> rule;
        grammar[noneTerminal[0]].nonTerminal = noneTerminal[0];
        grammar[noneTerminal[0]].rules.push_back(rule);

    }

    map<char, set<char>> firstSet; // To hold the first sets
    bitset<30> visited;
    for (const auto& pair : grammar) {
        char nonTerminal = pair.first;
        visited.reset();
        calculateFirst(nonTerminal, grammar, firstSet, visited);
    }

    // Output the First Sets
    for (const auto& pair : firstSet) {
        cout << pair.first << " ";
        for (char terminal : pair.second) {
            cout << terminal;
        }
        cout << '\n';
    }
    cout << "END_OF_FIRST\n";

    return 0;
}