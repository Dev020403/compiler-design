#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class LL1Parser
{
    map<string, vector<vector<string>>> grammar;
    map<string, set<string>> first;
    map<string, set<string>> follow;
    map<pair<string, string>, vector<string>> parseTable;

public:
    LL1Parser(map<string, vector<vector<string>>> grammar) : grammar(grammar) {}

    void calculateFirst()
    {
        for (const auto &rule : grammar)
        {
            first[rule.first] = {};
        }

        // Initialize FIRST for terminals
        for (const auto &rule : grammar)
        {
            for (const auto &production : rule.second)
            {
                for (const auto &symbol : production)
                {
                    if (grammar.find(symbol) == grammar.end())
                    { // It's a terminal
                        first[symbol] = {symbol};
                    }
                }
            }
        }

        // Calculate FIRST sets
        for (const auto &rule : grammar)
        {
            firstOf(rule.first);
        }
    }

    void firstOf(const string &symbol)
    {
        if (!first[symbol].empty())
            return; // Already processed

        if (grammar.find(symbol) == grammar.end())
        { // Terminal symbol
            first[symbol] = {symbol};
            return;
        }

        for (const auto &production : grammar[symbol])
        {
            for (const auto &charSymbol : production)
            {
                if (charSymbol == "ε")
                { // Empty string
                    first[symbol].insert("ε");
                    break;
                }

                firstOf(charSymbol);
                first[symbol].insert(first[charSymbol].begin(), first[charSymbol].end());

                if (first[charSymbol].find("ε") == first[charSymbol].end())
                {
                    break;
                }
            }
            if (find(production.begin(), production.end(), "ε") != production.end())
            {
                first[symbol].insert("ε");
            }
        }
    }

    void calculateFollow()
    {
        for (const auto &rule : grammar)
        {
            follow[rule.first] = {};
        }

        follow[grammar.begin()->first].insert("$"); // Start symbol

        for (const auto &rule : grammar)
        {
            followOf(rule.first);
        }
    }

    void followOf(const string &symbol)
    {
        for (const auto &rule : grammar)
        {
            for (const auto &production : rule.second)
            {
                auto it = find(production.begin(), production.end(), symbol);
                if (it != production.end())
                {
                    auto next = it + 1;
                    if (next != production.end())
                    {
                        const string &nextSymbol = *next;
                        firstOf(nextSymbol);
                        follow[symbol].insert(first[nextSymbol].begin(), first[nextSymbol].end());
                        follow[symbol].erase("ε");
                    }
                    if (next == production.end() || first[*next].find("ε") != first[*next].end())
                    {
                        follow[symbol].insert(follow[rule.first].begin(), follow[rule.first].end());
                    }
                }
            }
        }
    }

    void constructParseTable()
    {
        for (const auto &nonTerminal : grammar)
        {
            for (const auto &production : nonTerminal.second)
            {
                for (const auto &terminal : first[production[0]])
                {
                    if (terminal != "ε")
                    {
                        parseTable[{nonTerminal.first, terminal}] = production;
                    }
                }
                if (first[production[0]].find("ε") != first[production[0]].end())
                {
                    for (const auto &terminal : follow[nonTerminal.first])
                    {
                        parseTable[{nonTerminal.first, terminal}] = production;
                    }
                }
            }
        }
    }

    void printParseTable()
    {
        cout << "Parse Table:" << endl;
        cout << "Non-Terminal\tTerminal\tProduction" << endl;
        for (const auto &entry : parseTable)
        {
            cout << entry.first.first << "\t\t" << entry.first.second << "\t\t";
            for (const auto &sym : entry.second)
            {
                cout << sym << " ";
            }
            cout << endl;
        }
    }

    bool parse(const string &inputString)
    {
        stack<string> stk;
        stk.push("$");
        stk.push(grammar.begin()->first); // Start symbol

        string input = inputString + "$"; // Append $ to denote the end of the input
        size_t index = 0;

        while (!stk.empty())
        {
            string top = stk.top();
            string currentInput = ""; // To store multi-character terminals like "id"

            // If the current character in the input is part of 'id', treat it as a whole
            if (input[index] == 'i' && input.substr(index, 2) == "id")
            {
                currentInput = "id";
            }
            else
            {
                currentInput = string(1, input[index]); // Otherwise treat it as a single character
            }

            stk.pop();

            if (top == currentInput)
            {
                index += currentInput.length(); // Move to the next symbol in the input string
            }
            else if (parseTable.find({top, currentInput}) != parseTable.end())
            {
                vector<string> production = parseTable[{top, currentInput}];
                for (auto it = production.rbegin(); it != production.rend(); ++it)
                {
                    if (*it != "ε")
                    {
                        stk.push(*it);
                    }
                }
            }
            else
            {
                cout << "Error: Unexpected symbol " << currentInput << endl;
                return false;
            }
        }

        return true;
    }
};

// Reading input from a text file
string readInputFromFile(const string &filePath)
{
    ifstream file(filePath);
    string inputString;
    getline(file, inputString);
    return inputString;
}

int main()
{
    // Example grammar definition
    map<string, vector<vector<string>>> grammar = {
        {"E", {{"T", "E1"}}},
        {"E1", {{"+", "T", "E1"}, {"ε"}}},
        {"T", {{"F", "T1"}}},
        {"T1", {{"*", "F", "T1"}, {"ε"}}},
        {"F", {{"(", "E", ")"}, {"id"}}}};

    LL1Parser parser(grammar);
    parser.calculateFirst();
    parser.calculateFollow();
    parser.constructParseTable();
    parser.printParseTable();

    // Taking input string from the user
    cout << "Enter the input string to parse: ";
    string inputString;
    getline(cin, inputString); // Use getline to allow spaces if needed

    if (parser.parse(inputString))
    {
        cout << "The input string is valid." << endl;
    }
    else
    {
        cout << "The input string is invalid." << endl;
    }

    return 0;
}
