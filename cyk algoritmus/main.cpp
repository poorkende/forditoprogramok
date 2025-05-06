#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

using namespace std;

// A szab�lyokat tartalmaz� strukt�ra
struct Rule {
    string left;
    string right;
};

// A szab�lyok list�ja
vector<Rule> grammar = {
    {"S", "AB"},
    {"A", "a"},
    {"B", "b"}
};

// Termin�lisok �s nemtermin�lisok
unordered_map<string, set<string>> rules;  // Nemtermin�lis -> szab�lyok list�ja

// Inicializ�ljuk a grammatik�t
void initializeGrammar() {
    for (const auto &rule : grammar) {
        rules[rule.left].insert(rule.right);
    }
}

// A CYK algoritmus
bool CYK(const string &input) {
    int n = input.size();
    vector<vector<set<string>>> table(n, vector<set<string>>(n));

    // Kezd� l�p�s: t�lts�k fel az 1. sort a termin�lisokkal
    for (int i = 0; i < n; i++) {
        for (const auto &rule : grammar) {
            if (rule.right == string(1, input[i])) {
                table[i][i].insert(rule.left);
            }
        }
    }

    // T�lts�k fel a t�bl�t a dinamikus programoz�s seg�ts�g�vel
    for (int length = 2; length <= n; length++) {  // length: szubstring hossz
        for (int i = 0; i <= n - length; i++) {  // Kezd� poz�ci�
            int j = i + length - 1;  // V�gs� poz�ci�

            for (int k = i; k < j; k++) {  // Felbont�s a [i, k] �s [k+1, j] r�szekre
                for (const auto &rule : grammar) {
                    if (rule.right.size() == 2) {
                        char X = rule.right[0];  // Els� karakter
                        char Y = rule.right[1];  // M�sodik karakter

                        // Ha tal�lunk k�t megfelel� nemtermin�list
                        if (table[i][k].count(string(1, X)) > 0 &&
                            table[k + 1][j].count(string(1, Y)) > 0) {
                            table[i][j].insert(rule.left);
                        }
                    }
                }
            }
        }
    }

    // Az eg�sz bemeneti sztringet gener�l� nemtermin�lisok
    return table[0][n - 1].count("S") > 0;
}

int main() {
    // Inicializ�ljuk a grammatik�t
    initializeGrammar();

    // Teszt bemenet
    string input;
    cout << "Enter input string: ";
    cin >> input;

    // CYK algoritmus v�grehajt�sa
    if (CYK(input)) {
        cout << "The input string is accepted by the grammar." << endl;
    } else {
        cout << "Syntax error: input rejected." << endl;
    }

    return 0;
}
