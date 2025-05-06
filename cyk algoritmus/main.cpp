#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

// A szabályokat tartalmazó struktúra
struct Rule {
    string left;
    string right;
};

// A szabályok listája
vector<Rule> grammar = {
    {"S", "AB"},
    {"A", "a"},
    {"B", "b"}
};

// Terminálisok és nemterminálisok
unordered_map<string, set<string>> rules;  // Nemterminális -> szabályok listája

// Inicializáljuk a grammatikát
void initializeGrammar() {
    for (const auto &rule : grammar) {
        rules[rule.left].insert(rule.right);
    }
}

// A CYK algoritmus
bool CYK(const string &input) {
    int n = input.size();
    vector<vector<set<string>>> table(n, vector<set<string>>(n));

    // Kezdõ lépés: töltsük fel az 1. sort a terminálisokkal
    for (int i = 0; i < n; i++) {
        for (const auto &rule : grammar) {
            if (rule.right == string(1, input[i])) {
                table[i][i].insert(rule.left);
            }
        }
    }

    // Töltsük fel a táblát a dinamikus programozás segítségével
    for (int length = 2; length <= n; length++) {  // length: szubstring hossz
        for (int i = 0; i <= n - length; i++) {  // Kezdõ pozíció
            int j = i + length - 1;  // Végsõ pozíció

            for (int k = i; k < j; k++) {  // Felbontás a [i, k] és [k+1, j] részekre
                for (const auto &rule : grammar) {
                    if (rule.right.size() == 2) {
                        char X = rule.right[0];  // Elsõ karakter
                        char Y = rule.right[1];  // Második karakter

                        // Ha találunk két megfelelõ nemterminálist
                        if (table[i][k].count(string(1, X)) > 0 &&
                            table[k + 1][j].count(string(1, Y)) > 0) {
                            table[i][j].insert(rule.left);
                        }
                    }
                }
            }
        }
    }

    // Az egész bemeneti sztringet generáló nemterminálisok
    return table[0][n - 1].count("S") > 0;
}

int main() {
    // Inicializáljuk a grammatikát
    initializeGrammar();

    // Teszt bemenet
    string input;
    cout << "Enter input string: ";
    cin >> input;

    // CYK algoritmus végrehajtása
    if (CYK(input)) {
        cout << "The input string is accepted by the grammar." << endl;
    } else {
        cout << "Syntax error: input rejected." << endl;
    }

    return 0;
}
