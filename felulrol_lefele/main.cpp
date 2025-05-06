#include <iostream>
#include <vector>
#include <string>

using namespace std;

string input;
int pos = 0;

// Függvények prototípusai
bool S();
bool T();

// Helper: aktuális karakter lekérdezése
char current() {
    return pos < input.length() ? input[pos] : '#';
}

// Helper: adott karaktert vár, ha egyezik, lépteti a pozíciót
bool match(char expected) {
    if (current() == expected) {
        pos++;
        return true;
    }
    return false;
}

// S → T + S | T
bool S() {
    int saved_pos = pos;
    if (T() && match('+') && S()) {
        return true;
    }
    pos = saved_pos;
    if (T()) {
        return true;
    }
    pos = saved_pos;
    return false;
}

// T → a | b
bool T() {
    if (match('a') || match('b')) {
        return true;
    }
    return false;
}

int main() {
    cout << "Add meg az input szót (pl. b+a): ";
    cin >> input;

    pos = 0;
    if (S() && pos == input.length()) {
        cout << "Igen, a szó eleme a nyelvnek.\n";
    } else {
        cout << "Nem, a szó NEM eleme a nyelvnek.\n";
    }

    return 0;
}
