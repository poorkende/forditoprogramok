#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

// Grammatikai szabályok
unordered_set<string> U;  // Halmaz, mely tartalmazza azokat a nemterminálisokat, amelyekből λ levezethető
unordered_set<char> terminals = {'a', 'b', 'c'};  // Terminálisok halmaza
unordered_set<string> productions = {"S→aAc", "A→b", "A→ε"};  // A nyelvtan

// Ellenőrizzük, hogy a karakter terminális-e
bool isTerminal(char c) {
    return terminals.find(c) != terminals.end();
}

// Az üres szó levezetése (λ-mentesítés)
void lambdamentesLevezetés() {
    bool changed = true;
    while (changed) {
        changed = false;
        for (const string& prod : productions) {
            size_t arrow_pos = prod.find("→");
            string left = prod.substr(0, arrow_pos);
            string right = prod.substr(arrow_pos + 1);

            if (right == "ε") {
                // Ha az A → ε szabályt találjuk, hozzáadjuk az A-t az U halmazhoz
                if (U.find(left) == U.end()) {
                    U.insert(left);
                    changed = true;
                }
            } else {
                // Ha az A → α szabály van, akkor az α-ban található nemterminálisokat is ellenőrizzük
                bool canProduceLambda = true;
                for (char c : right) {
                    if (!isTerminal(c) && U.find(string(1, c)) == U.end()) {
                        canProduceLambda = false;
                        break;
                    }
                }
                if (canProduceLambda && U.find(left) == U.end()) {
                    U.insert(left);
                    changed = true;
                }
            }
        }
    }
}

// Nyelvtan levezetés LL(1) szerint
bool parseS(const string& input, int& index);

bool parseA(const string& input, int& index);

bool match(char expected, const string& input, int& index) {
    if (index < input.length() && input[index] == expected) {
        index++;
        return true;
    }
    return false;
}

// S → a A c
bool parseS(const string& input, int& index) {
    if (!match('a', input, index)) return false;
    if (!parseA(input, index)) return false;
    if (!match('c', input, index)) return false;
    return true;
}

// A → b | ε
bool parseA(const string& input, int& index) {
    if (index < input.length() && input[index] == 'b') {
        index++;
        return true;
    }
    // Ha A → ε, akkor sikeres, ha nincs 'b'
    return true;
}

int main() {
    string input;
    cout << "Enter input (valid: abc or ac): ";
    cin >> input;

    // Lambdamentes levezetés végrehajtása
    lambdamentesLevezetés();

    // LL(1) parsing
    int index = 0;
    bool result = parseS(input, index);

    if (result && index == input.length()) {
        cout << "The input is accepted by the LL(1) grammar." << endl;
    } else {
        cout << "Syntax error: input rejected." << endl;
    }

    return 0;
}
