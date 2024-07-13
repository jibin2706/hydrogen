#include "tokenization.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string tokens_to_asm(vector<Token> tokens) {
    string output = "global _start\n_start:\n";
    for (int i = 0; i < tokens.size(); i++) {
        Token token = tokens.at(i);
        if (token.type == TokenType::exit) {
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

    Tokenizer tokenizer = Tokenizer(file_content);
    vector<Token> tokens = tokenizer.tokenize();

    ofstream out("build/out.asm");
    out << tokens_to_asm(tokens);

    system("nasm -felf64 build/out.asm");
    system("ld build/out.o -o build/out");

    return EXIT_SUCCESS;
}