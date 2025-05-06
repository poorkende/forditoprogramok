#include <iostream>
#include <vector>
#include <stack>
#include <string>

using namespace std;

// Szabaly struktura
struct Rule {
    string lhs;
    string rhs;
    int number;
};

// A nyelvtan szabalyai
vector<Rule> rules = {
    {"S", "S+T"},  // 1
    {"S", "T"},    // 2
    {"T", "a"},    // 3
    {"T", "b"}     // 4
};

// Redukalasi fuggveny
bool tryReduce(stack<string>& alpha, stack<string>& beta, vector<int>& rule_sequence) {
    string top = "";
    vector<string> elems;
    stack<string> temp = alpha;

    while (!temp.empty()) {
        elems.push_back(temp.top());
        top = temp.top() + top;
        temp.pop();

        for (const Rule& rule : rules) {
            if (top == rule.rhs) {
                for (size_t i = 0; i < rule.rhs.length(); ++i)
                    alpha.pop();

                alpha.push(rule.lhs);
                beta.push(to_string(rule.number));
                rule_sequence.push_back(rule.number);
                cout << "Reduce: " << rule.lhs << " -> " << rule.rhs << " (rule " << rule.number << ")\n";
                return true;
            }
        }
    }

    return false;
}

int main() {
    string input;
    cout << "Enter input string (e.g. b+a): ";
    cin >> input;
    input += "#";  // End marker

    stack<string> alpha;
    stack<string> beta;
    vector<int> rule_sequence;

    size_t i = 0;
    while (true) {
        while (tryReduce(alpha, beta, rule_sequence));

        if (i < input.length() - 1) {
            string a(1, input[i]);
            alpha.push(a);
            beta.push("s");
            cout << "Shift: '" << a << "' pushed\n";
            i++;
        } else {
            break;
        }
    }

    while (tryReduce(alpha, beta, rule_sequence));

    if (alpha.size() == 1 && alpha.top() == "S") {
        cout << "\nInput accepted (YES)\n";
        cout << "Rule sequence (rightmost derivation): ";
        for (int num : rule_sequence)
            cout << num;
        cout << endl;
    } else {
        cout << "\nInput rejected (NO)\n";
    }

    // Peldak
    cout << "\nExample valid inputs:\n";
    cout << "  a\n";
    cout << "  b\n";
    cout << "  a+b\n";
    cout << "  b+a\n";
    cout << "  a+b+a\n";
    cout << "  b+a+b\n";

    return 0;
}
