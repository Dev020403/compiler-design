#include <iostream>
#include <string>
#include <cctype>

using namespace std;

class RecursiveDescentParser {
public:
    RecursiveDescentParser(const string& input) : input(input), pos(0) {
        currentToken = input[pos];
    }

    void parse() {
        E();
        if (currentToken != '$') {
            cout << "Error: Unexpected token at the end." << endl;
        } else {
            cout << "Input string is accepted." << endl;
        }
    }

private:
    string input;
    size_t pos;
    char currentToken;

    void advance() {
        pos++;
        if (pos < input.length()) {
            currentToken = input[pos];
        } else {
            currentToken = '$'; // End marker
        }
    }

    void E() {
        T();               // Production 1
        EPrime();         // Production 1
    }

    void EPrime() {
        if (currentToken == '+') {
            cout << "Using production (2): E' -> + T E'" << endl;
            advance();      // Consume '+'
            T();            // Production 3
            EPrime();      // Production 1
        }
        // ε (epsilon) production is implicit as nothing needs to be done
    }

    void T() {
        cout << "Using production (3): T -> F T'" << endl;
        F();               // Production 5
        TPrime();         // Production 4
    }

    void TPrime() {
        if (currentToken == '*') {
            cout << "Using production (4): T' -> * F T'" << endl;
            advance();      // Consume '*'
            F();            // Production 5
            TPrime();       // Production 4
        }
        // ε (epsilon) production is implicit as nothing needs to be done
    }

    void F() {
        if (currentToken == '(') {
            cout << "Using production (5): F -> ( E )" << endl;
            advance();      // Consume '('
            E();            // Production 1
            if (currentToken == ')') {
                advance();  // Consume ')'
            } else {
                cout << "Error: Expected ')'" << endl;
            }
        } else if (isalpha(currentToken)) { // Assuming identifiers are represented by alphabets
            cout << "Using production (5): F -> id" << endl;
            advance();  // Consume 'id'
        } else {
            cout << "Error: Unexpected token '" << currentToken << "'" << endl;
        }
    }
};

int main() {
    string input;

    // Input should be in the form of a mathematical expression
    cout << "Enter a mathematical expression: ";
    getline(cin, input);

    // Append end marker to the input
    input += '$';

    RecursiveDescentParser parser(input);
    parser.parse();

    return 0;
}
