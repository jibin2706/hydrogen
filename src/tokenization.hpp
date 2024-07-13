#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <vector>

using namespace std;

enum class TokenType {
    exit,
    int_lit,
    semi
};

struct Token {
    TokenType type;
    optional<string> value;
};

class Tokenizer {
private:
    int m_index = 0;
    string m_src;

    optional<char> peek(int ahead = 1) {
        if (m_index + ahead > m_src.length()) {
            return {};
        }
        return m_src.at(m_index);
    }

    char consume() {
        return m_src.at(m_index++);
    }

public:
    Tokenizer(string src) {
        this->m_src = src;
    }

    vector<Token> tokenize() {
        vector<Token> tokens;
        string buffer = "";

        while (peek().has_value()) {
            if (isalpha(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && isalnum(peek().value())) {
                    buffer.push_back(consume());
                }
                if (buffer == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buffer.clear();
                } else {
                    cerr << "You messed up" << endl;
                    cerr << "Invalid token: " << buffer << endl;
                    exit(EXIT_FAILURE);
                }
            } else if (isdigit(peek().value())) {
                buffer.push_back(consume());
                while (peek().has_value() && isdigit(peek().value())) {
                    buffer.push_back(consume());
                }
                tokens.push_back({.type = TokenType::int_lit, .value = buffer});
                buffer.clear();
            } else if (peek().value() == ';') {
                tokens.push_back({.type = TokenType::semi});
                consume();
            } else if (isspace(peek().value())) {
                consume();
            } else {
                cerr << "You messed up" << endl;
                cerr << "Invalid token: " << peek().value() << endl;
                exit(EXIT_FAILURE);
            }
        }
        m_index = 0;
        return tokens;
    }
};