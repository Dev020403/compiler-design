#include <iostream>
#include <stack>
#include <map>
#include <string>

using namespace std;

// Define the precedence table
map<char, map<char, char>> precedenceTable = {
    {'+', {{'+', '>'}, {'-', '>'}, {'*', '<'}, {'/', '<'}, {'(', '<'}, {')', '>'}, {'$', '>'}}},
    {'-', {{'+', '>'}, {'-', '>'}, {'*', '<'}, {'/', '<'}, {'(', '<'}, {')', '>'}, {'$', '>'}}},
    {'*', {{'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, {'(', '<'}, {')', '>'}, {'$', '>'}}},
    {'/', {{'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, {'(', '<'}, {')', '>'}, {'$', '>'}}},
    {'(', {{'+', '<'}, {'-', '<'}, {'*', '<'}, {'/', '<'}, {'(', '<'}, {')', '='}, {'$', '>'}}},
    {')', {{'+', '>'}, {'-', '>'}, {'*', '>'}, {'/', '>'}, {')', '>'}, {'$', '>'}}},
    {'$', {{'+', '<'}, {'-', '<'}, {'*', '<'}, {'/', '<'}, {'(', '<'}, {'$', '='}}}
};

// Check if a character is an operator
bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '$';
}

// Perform a reduction on the stack
void reduce(stack<char>& opStack, stack<int>& valStack) {
    char op = opStack.top();
    opStack.pop();
    
    int val2 = valStack.top();
    valStack.pop();
    
    int val1 = valStack.top();
    valStack.pop();

    int result;
    switch (op) {
        case '+': result = val1 + val2; break;
        case '-': result = val1 - val2; break;
        case '*': result = val1 * val2; break;
        case '/': result = val1 / val2; break;
    }
    valStack.push(result);
}

int operatorPrecedenceParser(string input) {
    stack<char> opStack;
    stack<int> valStack;
    
    // Add the end marker '$' to the input and push it onto the stack
    input += '$';
    opStack.push('$');

    size_t i = 0;
    while (i < input.length()) {
        char symbol = input[i];

        if (isdigit(symbol)) {
            // If it's a digit, push it to the value stack
            valStack.push(symbol - '0');
            i++;
        } else if (isOperator(symbol)) {
            // Get precedence between top of opStack and the current symbol
            char topOp = opStack.top();
            char precedence = precedenceTable[topOp][symbol];

            if (precedence == '<' || precedence == '=') {
                // Shift: Push the operator onto the stack
                opStack.push(symbol);
                i++;
            } else if (precedence == '>') {
                // Reduce: Perform reduction
                reduce(opStack, valStack);
            } else {
                cerr << "Error: Invalid expression!" << endl;
                return -1;
            }
        } else {
            cerr << "Error: Invalid character!" << endl;
            return -1;
        }
    }

    // The final result should be on the value stack
    return valStack.top();
}

int main() {
    string input;
    cout << "Enter an arithmetic expression (e.g., 3+5*2): ";
    cin >> input;

    int result = operatorPrecedenceParser(input);
    if (result != -1) {
        cout << "Result: " << result << endl;
    }
    return 0;
}
