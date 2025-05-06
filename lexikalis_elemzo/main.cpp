#include <iostream>
#include <cctype>
#include <string>

// Token t�pusok
enum TokenType {
    KULCSSZO,
    VALTOZO,
    OPERATOR,
    SZOKOZ,
    ERTEK,
    ISMERETLEN
};

// Token strukt�ra
struct Token {
    std::string value;
    TokenType type;
};

// Seg�df�ggv�ny, hogy a token t�pus�t meghat�rozzuk
TokenType getTokenType(const std::string &token) {
    if (token == "int" || token == "float" || token == "if" || token == "else") {
        return KULCSSZO;
    } else if (isalpha(token[0])) {  // Ha bet�vel kezd�dik, akkor v�ltoz�
        return VALTOZO;
    } else if (token == "(" || token == ")" || token == "{" || token == "}" || token == ";" || token == "=") {
        return OPERATOR;
    } else if (isspace(token[0])) {  // Ha sz�k�z, akkor azt kihagyhatjuk
        return SZOKOZ;
    } else if (isdigit(token[0])) { // Ha sz�m
        return ERTEK;
    }
    return ISMERETLEN;  // Ha nem ismert
}

// Funkci� a kommentek elt�vol�t�s�ra
std::string removeComments(const std::string &input) {
    std::string result = "";
    bool inComment = false;

    for (size_t i = 0; i < input.length(); ++i) {
        // Ha tal�lunk egy kommentet, akkor �tugorjuk
        if (i < input.length() - 1 && input[i] == '/' && input[i + 1] == '/') {
            inComment = true;  // A kommentek ut�n nem dolgozunk fel karaktereket
        }

        if (!inComment) {
            result += input[i];
        }

        if (input[i] == '\n') {
            inComment = false;  // �j sorban v�get �rhet a komment
        }
    }

    return result;
}

// Funkci�, hogy a forr�sk�dot tokenekre bontsa
void tokenize(const std::string &input) {
    std::string token = "";
    for (size_t i = 0; i < input.size(); ++i) {
        char current = input[i];

        // Ha karakter egy bet� vagy sz�m, akkor hozz�adjuk a tokenhez
        if (isalpha(current) || isdigit(current)) {
            token += current;
        }
        // Ha tal�lkozunk egy oper�torral vagy sz�k�zzel, akkor ki�rt�kelj�k a tokent
        else if (current == '(' || current == ')' || current == '{' || current == '}' || current == ';' || current == '=' || isspace(current)) {
            if (!token.empty()) {
                // Ha a token nem �res, akkor ki�rt�kelj�k
                TokenType type = getTokenType(token);
                std::cout << "Token: " << token << ", Tipus: " << type << std::endl;
                token = "";  // Resetelhetj�k a tokent
            }
            if (isspace(current)) continue;  // Ha sz�k�z, akkor nem csin�lunk semmit
            token += current;  // Ha oper�tor vagy z�r�jel, adjuk hozz� a tokenhez
            TokenType type = getTokenType(token);
            std::cout << "Token: " << token << ", Tipus: " << type << std::endl;
            token = "";  // Resetelhetj�k a tokent
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
    std::getline(std::cin, input);  // A felhaszn�l� bemeneti k�dja

    // El�sz�r t�vol�tsuk el a kommenteket
    std::string cleanedInput = removeComments(input);

    // Tokeniz�l�s
    tokenize(cleanedInput);

    return 0;
}
