#include <bits/stdc++.h>
using namespace std;
bool isOperator(char ch)
{
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' ||
            ch == '=' || ch == '>' || ch == '<' || ch == '!');
}

// Function to validate operators in the input string
void lexicalAnalyzer(const string &input)
{
    vector<string> operators; // Vector to hold operators found

    // Scan through the input string
    for (size_t i = 0; i < input.length(); i++)
    {
        if (isOperator(input[i]))
        {
            // Check for multi-character operators
            if (input[i] == '=' && i + 1 < input.length() && input[i + 1] == '=')
            {
                operators.push_back("==");
                i++; // Skip the next character
            }
            else if (input[i] == '!' && i + 1 < input.length() && input[i + 1] == '=')
            {
                operators.push_back("!=");
                i++; // Skip the next character
            }
            else if (input[i] == '<' && i + 1 < input.length() && input[i + 1] == '=')
            {
                operators.push_back("<=");
                i++; // Skip the next character
            }
            else if (input[i] == '>' && i + 1 < input.length() && input[i + 1] == '=')
            {
                operators.push_back(">=");
                i++; // Skip the next character
            }
            else
            {
                // Single-character operator
                operators.push_back(string(1, input[i]));
            }
        }
    }

    // Output the validated operators
    cout << "Operators found:" << endl;
    for (const string &op : operators)
    {
        cout << op << endl;
    }
}

int main()
{
    string input;

    // Input string to be analyzed
    cout << "Enter the input string: ";
    getline(cin, input);

    // Perform lexical analysis
    lexicalAnalyzer(input);

    return 0;
}
