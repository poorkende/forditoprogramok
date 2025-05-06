#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>

using namespace std;

// A szab�lyokat tartalmaz� strukt�ra
struct Rule {
    string left;    // Nemtermin�lis bal oldal
    string right;   // Termin�lis vagy nemtermin�lis jobb oldal
};

// Az LR(0) t�bl�zat
unordered_map<int, unordered_map<char, int>> actionTable;  // {�llapot} -> {termin�lis} -> {�llapot vagy m�velet}
unordered_map<int, unordered_map<char, int>> gotoTable;    // {�llapot} -> {nemtermin�lis} -> {�llapot}

// A szab�lyok list�ja
vector<Rule> grammar = {
    {"S", "AB"},
    {"A", "aA"},
    {"A", "b"},
    {"B", "cB"},
    {"B", "d"}
};

// Termin�lisok �s nemtermin�lisok
unordered_map<char, bool> terminals = {{'a', true}, {'b', true}, {'c', true}, {'d', true}};
unordered_map<string, bool> nonTerminals = {{"S", true}, {"A", true}, {"B", true}};

// Seg�df�ggv�nyek
bool isTerminal(char ch) {
    return terminals.find(ch) != terminals.end();
}

bool isNonTerminal(string s) {
    return nonTerminals.find(s) != nonTerminals.end();
}

// Az LR(0) elemz�
bool parse(const string &input) {
    stack<int> stateStack;
    stack<string> symbolStack;  // Symbol stack for keeping track of non-terminals and terminals
    stateStack.push(0);  // Kezd� �llapot
    symbolStack.push("$");  // A bemenet v�ge jelz�se

    int index = 0;  // Bemnet karakter�nek indexe

    while (index < input.length()) {
        int state = stateStack.top();
        char currentChar = input[index];

        if (actionTable[state].find(currentChar) != actionTable[state].end()) {
            int action = actionTable[state][currentChar];

            if (action > 0) {
                // Shift m�velet: l�pj�nk tov�bb az �j �llapotba
                stateStack.push(action);
                symbolStack.push(string(1, currentChar));
                index++;
            } else if (action < 0) {
                // Reduce m�velet: alkalmazzuk a szab�lyt
                Rule rule = grammar[-action - 1];  // Az akci� negat�v, �gy le kell vonni 1-et
                cout << "Reduce by rule: " << rule.left << " -> " << rule.right << endl;

                // T�r�lj�k a megfelel� sz�m� elemet a veremb�l
                for (int i = 0; i < rule.right.length(); i++) {
                    stateStack.pop();
                    symbolStack.pop();
                }

                // Az �j �llapotot a GOTO t�bl�zat seg�ts�g�vel tal�ljuk meg
                int newState = gotoTable[stateStack.top()][rule.left[0]];
                stateStack.push(newState);
                symbolStack.push(rule.left);
            }
        } else {
            return false;  // Hib�s input
        }
    }

    // Az �sszes sz�ks�ges reduce m�velet ut�n pr�b�ljuk meg befejezni az elemz�st
    while (stateStack.top() != 0 || symbolStack.top() != "$") {
        int state = stateStack.top();
        string topSymbol = symbolStack.top();

        if (isNonTerminal(topSymbol)) {
            // Ha a tetej�n nemtermin�lis van, akkor pr�b�ljuk meg alkalmazni a szab�lyokat
            bool reduced = false;
            for (int i = 0; i < grammar.size(); i++) {
                if (grammar[i].left == topSymbol) {
                    Rule rule = grammar[i];
                    cout << "Reduce by rule: " << rule.left << " -> " << rule.right << endl;

                    // T�r�lj�k a megfelel� sz�m� elemet a veremb�l
                    for (int j = 0; j < rule.right.length(); j++) {
                        stateStack.pop();
                        symbolStack.pop();
                    }

                    // Az �j �llapotot a GOTO t�bl�zat seg�ts�g�vel tal�ljuk meg
                    int newState = gotoTable[stateStack.top()][rule.left[0]];
                    stateStack.push(newState);
                    symbolStack.push(rule.left);
                    reduced = true;
                    break;
                }
            }

            if (!reduced) {
                return false;  // Ha nem tal�ltunk szab�lyt, hiba
            }
        } else {
            return false;  // Hib�s input
        }
    }

    // Elfogadott input eset�n a v�g�n a verem tartalma megfelel a kezd� nemtermin�lisnak
    return stateStack.top() == 0 && symbolStack.top() == "$";
}

int main() {
    // Az LR(0) t�bl�zat felt�lt�se
    actionTable[0]['a'] = 1;  // �llapot 0, 'a' -> shift 1
    actionTable[0]['b'] = 2;  // �llapot 0, 'b' -> shift 2
    actionTable[1]['a'] = -1; // �llapot 1, 'a' -> reduce S -> AB
    actionTable[1]['b'] = -1; // �llapot 1, 'b' -> reduce S -> AB
    actionTable[2]['c'] = 3;  // �llapot 2, 'c' -> shift 3
    actionTable[2]['d'] = 4;  // �llapot 2, 'd' -> shift 4
    actionTable[3]['c'] = -3; // �llapot 3, 'c' -> reduce A -> aA
    actionTable[4]['d'] = -4; // �llapot 4, 'd' -> reduce A -> b

    gotoTable[0]['A'] = 5; // �llapot 0, 'A' -> goto 5
    gotoTable[0]['B'] = 6; // �llapot 0, 'B' -> goto 6
    gotoTable[5]['A'] = 7; // �llapot 5, 'A' -> goto 7
    gotoTable[6]['B'] = 8; // �llapot 6, 'B' -> goto 8

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
