#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>

using namespace std;

// A szabályokat tartalmazó struktúra
struct Rule {
    string left;    // Nemterminális bal oldal
    string right;   // Terminális vagy nemterminális jobb oldal
};

// Az LR(0) táblázat
unordered_map<int, unordered_map<char, int>> actionTable;  // {állapot} -> {terminális} -> {állapot vagy mûvelet}
unordered_map<int, unordered_map<char, int>> gotoTable;    // {állapot} -> {nemterminális} -> {állapot}

// A szabályok listája
vector<Rule> grammar = {
    {"S", "AB"},
    {"A", "aA"},
    {"A", "b"},
    {"B", "cB"},
    {"B", "d"}
};

// Terminálisok és nemterminálisok
unordered_map<char, bool> terminals = {{'a', true}, {'b', true}, {'c', true}, {'d', true}};
unordered_map<string, bool> nonTerminals = {{"S", true}, {"A", true}, {"B", true}};

// Segédfüggvények
bool isTerminal(char ch) {
    return terminals.find(ch) != terminals.end();
}

bool isNonTerminal(string s) {
    return nonTerminals.find(s) != nonTerminals.end();
}

// Az LR(0) elemzõ
bool parse(const string &input) {
    stack<int> stateStack;
    stack<string> symbolStack;  // Symbol stack for keeping track of non-terminals and terminals
    stateStack.push(0);  // Kezdõ állapot
    symbolStack.push("$");  // A bemenet vége jelzése

    int index = 0;  // Bemnet karakterének indexe

    while (index < input.length()) {
        int state = stateStack.top();
        char currentChar = input[index];

        if (actionTable[state].find(currentChar) != actionTable[state].end()) {
            int action = actionTable[state][currentChar];

            if (action > 0) {
                // Shift mûvelet: lépjünk tovább az új állapotba
                stateStack.push(action);
                symbolStack.push(string(1, currentChar));
                index++;
            } else if (action < 0) {
                // Reduce mûvelet: alkalmazzuk a szabályt
                Rule rule = grammar[-action - 1];  // Az akció negatív, így le kell vonni 1-et
                cout << "Reduce by rule: " << rule.left << " -> " << rule.right << endl;

                // Töröljük a megfelelõ számú elemet a verembõl
                for (int i = 0; i < rule.right.length(); i++) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                // Az új állapotot a GOTO táblázat segítségével találjuk meg
                int newState = gotoTable[stateStack.top()][rule.left[0]];
                stateStack.push(newState);
                symbolStack.push(rule.left);
            }
        } else {
            return false;  // Hibás input
        }
    }

    // Az összes szükséges reduce mûvelet után próbáljuk meg befejezni az elemzést
    while (stateStack.top() != 0 || symbolStack.top() != "$") {
        int state = stateStack.top();
        string topSymbol = symbolStack.top();

        if (isNonTerminal(topSymbol)) {
            // Ha a tetején nemterminális van, akkor próbáljuk meg alkalmazni a szabályokat
            bool reduced = false;
            for (int i = 0; i < grammar.size(); i++) {
                if (grammar[i].left == topSymbol) {
                    Rule rule = grammar[i];
                    cout << "Reduce by rule: " << rule.left << " -> " << rule.right << endl;

                    // Töröljük a megfelelõ számú elemet a verembõl
                    for (int j = 0; j < rule.right.length(); j++) {
                        stateStack.pop();
                        symbolStack.pop();
                    }

                    // Az új állapotot a GOTO táblázat segítségével találjuk meg
                    int newState = gotoTable[stateStack.top()][rule.left[0]];
                    stateStack.push(newState);
                    symbolStack.push(rule.left);
                    reduced = true;
                    break;
                }
            }

            if (!reduced) {
                return false;  // Ha nem találtunk szabályt, hiba
            }
        } else {
            return false;  // Hibás input
        }
    }

    // Elfogadott input esetén a végén a verem tartalma megfelel a kezdõ nemterminálisnak
    return stateStack.top() == 0 && symbolStack.top() == "$";
}

int main() {
    // Az LR(0) táblázat feltöltése
    actionTable[0]['a'] = 1;  // Állapot 0, 'a' -> shift 1
    actionTable[0]['b'] = 2;  // Állapot 0, 'b' -> shift 2
    actionTable[1]['a'] = -1; // Állapot 1, 'a' -> reduce S -> AB
    actionTable[1]['b'] = -1; // Állapot 1, 'b' -> reduce S -> AB
    actionTable[2]['c'] = 3;  // Állapot 2, 'c' -> shift 3
    actionTable[2]['d'] = 4;  // Állapot 2, 'd' -> shift 4
    actionTable[3]['c'] = -3; // Állapot 3, 'c' -> reduce A -> aA
    actionTable[4]['d'] = -4; // Állapot 4, 'd' -> reduce A -> b

    gotoTable[0]['A'] = 5; // Állapot 0, 'A' -> goto 5
    gotoTable[0]['B'] = 6; // Állapot 0, 'B' -> goto 6
    gotoTable[5]['A'] = 7; // Állapot 5, 'A' -> goto 7
    gotoTable[6]['B'] = 8; // Állapot 6, 'B' -> goto 8

    // Teszt bemenetek
    string input;
    cout << "Enter input string: ";
    cin >> input;

    if (parse(input)) {
        cout << "The input is accepted by the LR(0) grammar." << endl;
    } else {
        cout << "Syntax error: input rejected." << endl;
    }

    return 0;
}
