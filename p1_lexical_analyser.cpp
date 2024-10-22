#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <unordered_set>

using namespace std;

const int MAX_IDENTIFIER_LENGTH = 20;

unordered_set<string> keywords = {"int", "float", "if", "else", "while", "return"};

unordered_set<string> operators = {"+", "-", "*", "/", "=", "=="};

struct Token {
    string type;
    string value;
};

bool isKeyword(const string& str) {
    return keywords.find(str) != keywords.end();
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '>';
}

bool isDelimiter(char c) {
    return isspace(c) || c == '(' || c == ')' || c == ';' || c == '{' || c == '}';
}
vector<Token> lexicalAnalyzer(const string& input) {
    vector<Token> tokens;
    string current = "";
    bool inComment = false;
    int i = 0;

    while (i < input.length()) {
        char c = input[i];

        if (c == '/' && input[i + 1] == '/') {
            while (i < input.length() && input[i] != '\n') {
                i++;
            }
        }
        else if (c == '/' && input[i + 1] == '*') {
            i += 2;
            while (i < input.length() && !(input[i] == '*' && input[i + 1] == '/')) {
                i++;
            }
            i += 2;  
        }
        else if (isspace(c)) {
            i++;
        }
        else if (isOperator(c)) {
            string op(1, c);
            if (c == '=' && input[i + 1] == '=') {
                op += "=";
                i++;
            }
            tokens.push_back({"Operator", op});
            i++;
        }
        else if (isalpha(c)) {
            current = "";
            while (i < input.length() && (isalnum(input[i]) || input[i] == '_')) {
                current += input[i];
                i++;
            }
            if (current.length() > MAX_IDENTIFIER_LENGTH) {
                cout << "Error: Identifier " << current << " exceeds maximum length of "
                     << MAX_IDENTIFIER_LENGTH << " characters.\n";
            } else if (isKeyword(current)) {
                tokens.push_back({"Keyword", current});
            } else {
                tokens.push_back({"Identifier", current});
            }
        }
        else if (isdigit(c)) {
            current = "";
            while (i < input.length() && isdigit(input[i])) {
                current += input[i];
                i++;
            }
            tokens.push_back({"Literal", current});
        }
        else if (isDelimiter(c)) {
            if (c != ' ') {
                string delim(1, c);
                tokens.push_back({"Delimiter", delim});
            }
            i++;
        } else {
            cout << "Error: Unrecognized character '" << c << "'\n";
            i++;
        }
    }

    return tokens;
}

int main() {
    string code = R"(
    int main() {
        int x = 10;
        float y = 20.5;
        // This is a single-line comment
        /* This is a
        multi-line comment */
        if (x == 10) {
            y = y + x;
        }
        return 0;
    }
    )";

    vector<Token> tokens = lexicalAnalyzer(code);
    for (const Token& token : tokens) {
        cout << "Token Type: " << token.type << ", Value: " << token.value << endl;
    }

    return 0;
}
