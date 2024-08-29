#include <iostream>
#include <string>
#include <vector>
#include <memory>

// Define token type for lexer
enum class TokenType
{
    LITERAL,
    DOT,
    STAR,
    END
};

// Structure for token with constructor
struct Token
{
    TokenType type; // Type of token
    char value;     // Value

    // (Constructor) initializes a Token with a specified type and an optional value. The default value for value is '\0', which is useful for tokens that do not need a specific character value (like END).
    Token(TokenType type, char value = '\0') : type(type), value(value) {}
};
