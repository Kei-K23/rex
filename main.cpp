#include <iostream>
#include <string>
#include <vector>
#include <memory>

enum class TokenType
{
    LITERAL,
    DOT,
    STAR,
    END
};

struct Token
{
    TokenType type;
    char value;

    Token(TokenType type, char value = '\0') : type(type), value(value) {}
};

class Lexer
{
public:
    Lexer(const std::string &pattern) : pattern(pattern), index(0) {}

    Token nextToken()
    {
        if (index >= pattern.length())
        {
            return Token(TokenType::END);
        }

        char current = pattern[index++];
        switch (current)
        {
        case '*':
            return Token(TokenType::STAR, '*');
        case '.':
            return Token(TokenType::DOT, '.');
        default:
            return Token(TokenType::LITERAL, current);
        }
    }

private:
    std::string pattern;
    size_t index;
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
        if (index < text.length() && text[index] == value)
        {
            ++index;
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
        if (index < text.length())
        {
            ++index;
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

        // Try to match the rest of the pattern by backtracking
        while (index >= start)
        {
            if (index == text.length() || node->match(text, index))
            {
                return true;
            }
            --index;
        }
        return false;
    }

private:
    std::unique_ptr<ASTNode> node;
};

class ConcatNode : public ASTNode
{
public:
    ConcatNode(std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : left(std::move(left)), right(std::move(right)) {}

    bool match(const std::string &text, size_t &index) const override
    {
        size_t temp = index;
        if (left->match(text, temp) && right->match(text, temp))
        {
            index = temp;
            return true;
        }
        return false;
    }

private:
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

class Parser
{
public:
    Parser(Lexer &lexer) : lexer(lexer), currentToken(lexer.nextToken()) {}

    std::unique_ptr<ASTNode> parseExpression()
    {
        std::unique_ptr<ASTNode> left = parsePrimary();
        while (currentToken.type == TokenType::LITERAL || currentToken.type == TokenType::DOT)
        {
            std::unique_ptr<ASTNode> right = parsePrimary();
            left = std::make_unique<ConcatNode>(std::move(left), std::move(right));
        }
        return left;
    }

private:
    Lexer &lexer;
    Token currentToken;

    void advance()
    {
        currentToken = lexer.nextToken();
    }

    std::unique_ptr<ASTNode> parsePrimary()
    {
        std::unique_ptr<ASTNode> node;
        switch (currentToken.type)
        {
        case TokenType::LITERAL:
            node = std::make_unique<LiteralNode>(currentToken.value);
            advance();
            break;
        case TokenType::DOT:
            node = std::make_unique<DotNode>();
            advance();
            break;
        default:
            break;
        }

        if (currentToken.type == TokenType::STAR)
        {
            node = std::make_unique<StarNode>(std::move(node));
            advance();
        }

        return node;
    }
};

class Matcher
{
public:
    Matcher(std::unique_ptr<ASTNode> root) : root(std::move(root)) {}

    bool match(const std::string &text)
    {
        size_t index = 0;
        return root->match(text, index) && index == text.length();
    }

private:
    std::unique_ptr<ASTNode> root;
};

int main()
{
    std::string pattern = "a.";
    std::string text = "ax";

    Lexer lexer(pattern);
    Parser parser(lexer);

    std::unique_ptr<ASTNode> root = parser.parseExpression();
    Matcher matcher(std::move(root));

    if (matcher.match(text))
    {
        std::cout << "Match!" << std::endl;
    }
    else
    {
        std::cout << "No match." << std::endl;
    }

    return 0;
}
