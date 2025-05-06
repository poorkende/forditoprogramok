#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <stack>
#include <unordered_set>

using namespace std;

// Grammatikai szabályok (előre meghatározott)
unordered_map<string, vector<string>> parsingTable = {
    {"S", {"aA", "bB"}}, // S → aA | bB
    {"A", {"c", "d"}},   // A → c | d
    {"B", {"e", "f"}}    // B → e | f
};

// Terminálisok és nemterminálisok
unordered_set<char> terminals = {'a', 'b', 'c', 'd', 'e', 'f'};
unordered_set<string> nonTerminals = {"S", "A", "B"};

// Segédfüggvények
bool isTerminal(char ch) {
    return terminals.find(ch) != terminals.end();
}

bool isNonTerminal(const string& s) {
    return nonTerminals.find(s) != nonTerminals.end();
}

// LL(1) elemző
bool parse(const string &input) {
    stack<string> parseStack;
    parseStack.push("S");  // Kezdő nemterminális
    int index = 0;

    while (!parseStack.empty() && index < input.length()) {
        string top = parseStack.top();
        parseStack.pop();

        if (isNonTerminal(top)) {
            // Ha a tetején nemterminális van, akkor a táblázat alapján dolgozunk
            if (parsingTable.find(top) != parsingTable.end()) {
                vector<string> rules = parsingTable[top];
                string rule = "";

                // Kiválasztjuk a megfelelő szabályt
                for (const string& r : rules) {
                    if (r[0] == input[index]) {
                        rule = r;
                        break;
                    }
                }

                // Ha van érvényes szabály, hozzáadjuk a veremhez
                if (!rule.empty()) {
                    // A szabály karaktereinek megfelelően toljuk a verembe
                    for (int i = rule.length() - 1; i >= 0; --i) {
                        string curr(1, rule[i]);
                        parseStack.push(curr);
                    }
                } else {
                    return false;  // Syntax error: nincs megfelelő szabály
                }
            }
        } else if (isTerminal(top[0])) {
            // Ha a terminális megegyezik az aktuális karakterrel, akkor léphetünk tovább
            if (top[0] == input[index]) {
                index++;
            } else {
                return false;  // Syntax error
            }
        }
    }

    // Sikeres elemzés, ha a bemenetet elfogadtuk és a verem üres
    return index == input.length() && parseStack.empty();
}

int main() {
    string input;

    // Elfogadott és nem elfogadott bemenetek példái
    vector<string> accepted_inputs = {"ac", "ad", "be", "bf"};
    vector<string> rejected_inputs = {"abc", "ae", "ab", "ca"};

    // Elfogadott bemenetek tesztelése
    cout << "Testing accepted inputs:\n";
    for (const string& test_input : accepted_inputs) {
        cout << "Input: " << test_input << " -> ";
        if (parse(test_input)) {
            cout << "The input is accepted by the LL(1) grammar." << endl;
        } else {
            cout << "Syntax error: input rejected." << endl;
        }
    }

    // Nem elfogadott bemenetek tesztelése
    cout << "\nTesting rejected inputs:\n";
    for (const string& test_input : rejected_inputs) {
        cout << "Input: " << test_input << " -> ";
        if (parse(test_input)) {
            cout << "The input is accepted by the LL(1) grammar." << endl;
        } else {
            cout << "Syntax error: input rejected." << endl;
        }
    }

    return 0;
}
