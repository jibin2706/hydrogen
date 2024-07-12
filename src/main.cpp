#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    _return,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    optional<string> value;
};

vector<Token> tokenize(string str) {
    vector<Token> tokens;
    string buffer = "";

    for (int i = 0; i < str.length(); i++) {
        char c = str.at(i);

        if (isspace(c)) {
            continue;
        } else if (isdigit(c)) {
            buffer.push_back(c);
            i++;
            while (isdigit(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;
            tokens.push_back({.type = TokenType::int_lit, .value = buffer});
            buffer.clear();
        } else if (c == ';') {
            tokens.push_back({.type = TokenType::semi});
        } else if (isalpha(c)) {
            buffer.push_back(c);
            i++;
            while (isalnum(str.at(i))) {
                buffer.push_back(str.at(i));
                i++;
            }
            i--;

            if (buffer == "return") {
                tokens.push_back({.type = TokenType::_return});
                buffer.clear();
            } else {
                cerr << "You message up" << endl;
                exit(EXIT_FAILURE);
            }
        } else {
            cerr << "You message up" << endl;
            exit(EXIT_FAILURE);
        }
    }

    return tokens;
}

string tokens_to_asm(vector<Token> tokens) {
    string output = "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens.at(i);
        if (token.type == TokenType::_return) {
            if (i + 1 < tokens.size() && tokens.at(i + 1).type == TokenType::int_lit) {
                if (i + 2 < tokens.size() && tokens.at(i + 2).type == TokenType::semi) {
                    output += "    mov rax, 60\n";
                    output += "    mov rdi, " + tokens.at(i + 1).value.value() + "\n";
                    output += "    syscall";
                }
            }
        }
    }
    return output;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cerr << "Incorrect usage. Correct usage is..." << endl;
        cerr << "hydrogen <filename>" << endl;
        return EXIT_FAILURE;
    }

    string file_path = "/home/jibin/dev/hydrogen/";
    file_path.append(argv[1]);
    ifstream file(file_path);
    string str;
    string file_content;
    while (getline(file, str)) {
        file_content += str + "\n";
    }

    vector<Token> tokens = tokenize(file_content);

    ofstream out("build/out.asm");
    out << tokens_to_asm(tokens);

    system("nasm -felf64 build/out.asm");
    system("ld build/out.o -o build/out");

    return EXIT_SUCCESS;
}