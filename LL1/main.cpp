#include <iostream>
#include <string>

using namespace std;

// Prototípusok
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
    if (!match('a', input, index)) return false;  // Ellenőrizzük az 'a' karaktert
    if (!parseA(input, index)) return false;      // Ellenőrizzük az A nemterminális szabályt
    if (!match('c', input, index)) return false;  // Ellenőrizzük a 'c' karaktert
    return true;
}

// A → b | ε
bool parseA(const string& input, int& index) {
    if (index < input.length() && input[index] == 'b') {
        index++;  // Ha találunk 'b'-t, akkor sikeres
        return true;
    }
    // ε - üres sztring, tehát sikeres, ha nincs 'b'
    return true;
}

int main() {
    string input;
    cout << "Enter input (valid: abc or ac): ";
    cin >> input;

    int index = 0;
    bool result = parseS(input, index);

    // Ha sikerült a parsing és a teljes inputot feldolgoztuk
    if (result && index == input.length()) {
        cout << "The input is accepted by the LL(1) grammar." << endl;
    } else {
        cout << "Syntax error: input rejected." << endl;
    }

    return 0;
}
