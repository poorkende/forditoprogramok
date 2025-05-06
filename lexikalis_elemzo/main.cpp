#include <iostream>
#include <cctype>
#include <string>

// Token típusok
enum TokenType {
    KULCSSZO,
    VALTOZO,
    OPERATOR,
    SZOKOZ,
    ERTEK,
    ISMERETLEN
};

// Token struktúra
struct Token {
    std::string value;
    TokenType type;
};

// Segédfüggvény, hogy a token típusát meghatározzuk
TokenType getTokenType(const std::string &token) {
    if (token == "int" || token == "float" || token == "if" || token == "else") {
        return KULCSSZO;
    } else if (isalpha(token[0])) {  // Ha betûvel kezdõdik, akkor változó
        return VALTOZO;
    } else if (token == "(" || token == ")" || token == "{" || token == "}" || token == ";" || token == "=") {
        return OPERATOR;
    } else if (isspace(token[0])) {  // Ha szóköz, akkor azt kihagyhatjuk
        return SZOKOZ;
    } else if (isdigit(token[0])) { // Ha szám
        return ERTEK;
    }
    return ISMERETLEN;  // Ha nem ismert
}

// Funkció a kommentek eltávolítására
std::string removeComments(const std::string &input) {
    std::string result = "";
    bool inComment = false;

    for (size_t i = 0; i < input.length(); ++i) {
        // Ha találunk egy kommentet, akkor átugorjuk
        if (i < input.length() - 1 && input[i] == '/' && input[i + 1] == '/') {
            inComment = true;  // A kommentek után nem dolgozunk fel karaktereket
        }

        if (!inComment) {
            result += input[i];
        }

        if (input[i] == '\n') {
            inComment = false;  // Új sorban véget érhet a komment
        }
    }

    return result;
}

// Funkció, hogy a forráskódot tokenekre bontsa
void tokenize(const std::string &input) {
    std::string token = "";
    for (size_t i = 0; i < input.size(); ++i) {
        char current = input[i];

        // Ha karakter egy betû vagy szám, akkor hozzáadjuk a tokenhez
        if (isalpha(current) || isdigit(current)) {
            token += current;
        }
        // Ha találkozunk egy operátorral vagy szóközzel, akkor kiértékeljük a tokent
        else if (current == '(' || current == ')' || current == '{' || current == '}' || current == ';' || current == '=' || isspace(current)) {
            if (!token.empty()) {
                // Ha a token nem üres, akkor kiértékeljük
                TokenType type = getTokenType(token);
                std::cout << "Token: " << token << ", Tipus: " << type << std::endl;
                token = "";  // Resetelhetjük a tokent
            }
            if (isspace(current)) continue;  // Ha szóköz, akkor nem csinálunk semmit
            token += current;  // Ha operátor vagy zárójel, adjuk hozzá a tokenhez
            TokenType type = getTokenType(token);
            std::cout << "Token: " << token << ", Tipus: " << type << std::endl;
            token = "";  // Resetelhetjük a tokent
        }
    }
    if (!token.empty()) {
        TokenType type = getTokenType(token);
        std::cout << "Token: " << token << ", Tipus: " << type << std::endl;
    }
}

int main() {
    std::string input;

    std::cout << "Add meg a forraskodot (pl. C++ kod):" << std::endl;
    std::getline(std::cin, input);  // A felhasználó bemeneti kódja

    // Elõször távolítsuk el a kommenteket
    std::string cleanedInput = removeComments(input);

    // Tokenizálás
    tokenize(cleanedInput);

    return 0;
}
