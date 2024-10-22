#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

class LALRParser
{
    map<string, vector<vector<string>>> grammar;
    string startSymbol;
    map<string, map<string, string>> parsingTable;
    map<pair<string, vector<string>>, set<string>> lr1Items;

public:
    LALRParser(map<string, vector<vector<string>>> grammar, string startSymbol)
        : grammar(grammar), startSymbol(startSymbol)
    {
        parsingTable = buildParsingTable();
        lr1Items = buildLR1Items();
    }

    map<pair<string, vector<string>>, set<string>> buildLR1Items()
    {
        // Generate LR(1) items for the grammar
        map<pair<string, vector<string>>, set<string>> items;
        for (const auto &rule : grammar)
        {
            for (const auto &production : rule.second)
            {
                items[{rule.first, production}] = {"$"}; // Add a lookahead symbol
            }
        }

        cout << "\nLR(1) Items:\n";
        for (const auto &item : items)
        {
            cout << item.first.first << " -> ";
            for (const string &sym : item.first.second)
            {
                cout << sym << " ";
            }
            cout << ", Lookahead: ";
            for (const string &lookahead : item.second)
            {
                cout << lookahead << " ";
            }
            cout << endl;
        }
        return items;
    }

    map<string, map<string, string>> buildParsingTable()
    {
        // Construct the parsing table for the grammar
        map<string, map<string, string>> table = {
            {"E", {{"id", "E -> T"}, {"(", "E -> T"}}},
            {"T", {{"id", "T -> F"}, {"(", "T -> F"}}},
            {"F", {{"id", "F -> id"}, {"(", "F -> ( E )"}}},
            {"+", {{"T", "E -> E + T"}}},
            {"*", {{"F", "T -> T * F"}}},
            {"$", {{"", "Accept"}}}};
        return table;
    }

    void printParsingTable()
    {
        // Print the parsing table in a formatted way
        cout << "\nParsing Table:\n";
        cout << "Non-Terminal\tTerminal\tProduction\n";
        for (const auto &row : parsingTable)
        {
            for (const auto &entry : row.second)
            {
                cout << row.first << "\t\t" << entry.first << "\t\t" << entry.second << endl;
            }
        }
    }

    bool parse(const string &inputString)
    {
        vector<string> tokens = tokenize(inputString);
        tokens.push_back("$"); // Add end of input symbol

        stack<string> stk;
        stk.push("$");
        stk.push(startSymbol); // Stack initialization

        size_t index = 0; // Index for current token

        cout << "\nParsing Process:\n";

        while (!stk.empty())
        {
            string top = stk.top();              // Top of the stack
            string currentToken = tokens[index]; // Current token

            if (grammar.find(top) != grammar.end())
            { // Non-terminal
                string action = parsingTable[top][currentToken];
                if (!action.empty())
                {
                    cout << "Action: " << action << " on '" << currentToken << "'\n";
                    stk.pop(); // Pop the non-terminal
                    if (action.find("->") != string::npos)
                    {
                        string production = action.substr(action.find("->") + 2);
                        if (!production.empty())
                        { // If there's a production
                            vector<string> prodSymbols = split(production);
                            for (auto it = prodSymbols.rbegin(); it != prodSymbols.rend(); ++it)
                            {
                                if (*it != "ε")
                                {
                                    stk.push(*it); // Push the production to the stack
                                }
                            }
                        }
                    }
                }
                else
                {
                    cout << "Error: No action for '" << top << "' with token '" << currentToken << "'.\n";
                    return false;
                }
            }
            else if (top == currentToken)
            { // Terminal match
                cout << "Matched terminal: '" << top << "'\n";
                stk.pop();
                index++; // Move to the next token
            }
            else
            {
                cout << "Error: Stack top '" << top << "' does not match current token '" << currentToken << "'.\n";
                return false;
            }

            if (stk.size() == 1 && stk.top() == "$")
            { // Acceptance condition
                cout << "Input accepted.\n";
                return true;
            }
        }

        return false;
    }

private:
    vector<string> tokenize(const string &inputString)
    {
        // Tokenize the input string
        vector<string> tokens;
        stringstream ss(inputString);
        string token;
        while (ss >> token)
        {
            tokens.push_back(token);
        }
        return tokens;
    }

    vector<string> split(const string &s)
    {
        // Split the string by spaces
        vector<string> result;
        stringstream ss(s);
        string word;
        while (ss >> word)
        {
            result.push_back(word);
        }
        return result;
    }
};

int main()
{
    // Example grammar definition
    map<string, vector<vector<string>>> grammar = {
        {"E", {{"E", "+", "T"}, {"T"}}},
        {"T", {{"T", "*", "F"}, {"F"}}},
        {"F", {{"(", "E", ")"}, {"id"}}}};

    LALRParser parser(grammar, "E");

    // Print the parsing table
    parser.printParsingTable();

    // Example input string
    string inputString = "id +* id"; // Change this input as needed
    parser.parse(inputString);

    return 0;
}
