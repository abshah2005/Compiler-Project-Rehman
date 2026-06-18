#include <iostream>
#include <stack>
#include <vector>
#include <cctype>

using namespace std;

int temp = 1;

string newTemp() {
    return "t" + to_string(temp++);
}

struct Token {
    string value;
};

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// -------------------------------
// TOKENIZER (LEVEL 2 UPGRADE)
// -------------------------------
vector<string> tokenize(string expr) {
    vector<string> tokens;
    string current = "";

    for (int i = 0; i < expr.length(); i++) {
        char c = expr[i];

        if (c == ' ') continue;

        if (isalnum(c)) {
            current += c;
        }
        else {
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            tokens.push_back(string(1, c));
        }
    }

    if (!current.empty())
        tokens.push_back(current);

    return tokens;
}

// -------------------------------
// INFIX → POSTFIX
// -------------------------------
vector<string> infixToPostfix(vector<string> tokens) {
    stack<char> st;
    vector<string> postfix;

    for (string tok : tokens) {
        if (isalnum(tok[0]) && tok.size() > 0 && !isOperator(tok[0])) {
            postfix.push_back(tok);
        }
        else if (tok == "(") {
            st.push('(');
        }
        else if (tok == ")") {
            while (!st.empty() && st.top() != '(') {
                postfix.push_back(string(1, st.top()));
                st.pop();
            }
            st.pop();
        }
        else if (tok.size() == 1 && isOperator(tok[0])) {
            char op = tok[0];
            while (!st.empty() && precedence(st.top()) >= precedence(op)) {
                postfix.push_back(string(1, st.top()));
                st.pop();
            }
            st.push(op);
        }
    }

    while (!st.empty()) {
        postfix.push_back(string(1, st.top()));
        st.pop();
    }

    return postfix;
}

// -------------------------------
// POSTFIX → TAC
// -------------------------------
void postfixToTAC(vector<string> postfix) {
    stack<string> st;

    for (string tok : postfix) {
        if (tok.size() > 1 || isalnum(tok[0])) {
            st.push(tok);
        }
        else {
            string op2 = st.top(); st.pop();
            string op1 = st.top(); st.pop();

            string t = newTemp();

            cout << t << " = " << op1 << " " << tok << " " << op2 << endl;

            st.push(t);
        }
    }
}

// -------------------------------
// MAIN
// -------------------------------
int main() {
    string expr;

    cout << "Enter expression: ";
    getline(cin, expr);

    vector<string> tokens = tokenize(expr);

    vector<string> postfix = infixToPostfix(tokens);

    cout << "\nPostfix: ";
    for (auto &t : postfix) cout << t << " ";
    cout << "\n\nThree Address Code:\n";

    postfixToTAC(postfix);

    return 0;
}