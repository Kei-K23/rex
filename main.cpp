#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cctype>

/*
The TokenType enumeration defines the types of tokens the lexer can generate.
*/
enum class TokenType
{
    LITERAL,
    DOT,
    STAR,
    END
};

/*
The Token struct encapsulates a token's type and value.
*/
struct Token
{
    TokenType type; // Type of token
    char value;     // Value

    // (Constructor) initializes a Token with a specified type and an optional value. The default value for value is '\0', which is useful for tokens that do not need a specific character value (like END).
    Token(TokenType type, char value = '\0') : type(type), value(value) {}
};

/*
 The Lexer class reads the regular expression pattern and produces tokens corresponding to the different components of the pattern (literals, dot, asterisk, end of input). The nextToken method handles the logic of token generation based on the current character and updates the index accordingly.
*/
class Lexer
{
public:
    Lexer(const std::string &pattern) : pattern(pattern), index(0) {}

    Token nextToken()
    {
        // Reach to the end of pattern string
        if (index >= pattern.length())
        {
            return Token(TokenType::END);
        }

        // Get current character
        char current = pattern[index++];

        switch (current)
        {
        case '*':
            return Token(TokenType::STAR);
        case '.':
            return Token(TokenType::DOT);
        default:
            // alphabetic literal token
            if (std::isalpha(current))
            {
                return Token(TokenType::LITERAL);
            }
            else
            {
                throw std::invalid_argument("Invalid pattern string");
            }
        }
    }

private:
    std::string pattern; // Pattern string
    size_t index;        // Index for pattern string
};

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual bool match(const std::string &text, size_t &index) const = 0;
};

class LiteralNode : public ASTNode
{

public:
    LiteralNode(char value) : value(value) {}

    bool match(const std::string &text, size_t &index) const override
    {
        // Check value are equal
        if (index < text.length() && text[index] == value)
        {
            // Increment the index for text
            index++;
            return true;
        }
        return false;
    }

private:
    char value;
};

class DotNode : public ASTNode
{
public:
    bool match(const std::string &text, size_t &index) const override
    {
        // Check current text index is within range
        if (index < text.length())
        {
            index++;
            return true;
        }
        return false;
    }
};

class StarNode : public ASTNode
{
public:
    StarNode(std::unique_ptr<ASTNode> node) : node(std::move(node)) {}

    bool match(const std::string &text, size_t &index) const override
    {
        size_t start = index;

        // Match as many as possible
        while (node->match(text, index))
            ;

        while (index >= start)
        {
            if (index == text.length() || node->match(text, index))
            {
                return true;
            }
            index--;
        }
        return false;
    }

private:
    std::unique_ptr<ASTNode> node;
};