# Rex

`Rex` is a `Regular Expression (Regex) engine` that written in `C++`. It supports basic features like matching literal characters, the wildcard (.) character, the Kleene star (\*) for zero or more repetitions, grouping using parentheses (()), lookahead/lookbehind ,and alternation using the pipe (|) operator.

## Features

- **Literal Matching**: Match specific characters in the input string.
- **Wildcard (.)**: Matches any single character.
- **Kleene Star (\_)**: Matches zero or more occurrences of the preceding element.
- **Grouping (())**: Groups expressions to apply operators like \_ or to create sub-expressions.
- **Alternation (|)**: Provides a choice between alternatives, matching either expression.
- **lookahead/lookbehind**: Lookaround assertions are non-capturing groups that return matches only if the target string is followed or preceded by a particular character.

## Usage

### Prerequisites

- C++ compiler (e.g., g++, clang++).

### Building the Project

To compile the project, run the following command:

```bash
clang++ -std=c++20 main.cpp -o rex
# Or
g++ -std=c++20 main.cpp -o rex
```

### Running the Program

You can run the compiled executable with a pattern and text input:

```bash
./regex_engine -p <pattern> -t <text>
```

#### Example

To match the text "bbc" against the pattern "a|b\*":

```bash
./regex_engine -p "a|b\*" -t "bbc"
```

This will output:

```bash
Match!
```

### Command-Line Arguments

- `-p <pattern>`: The regular expression pattern to match.
- `-t <text>`: The text to be matched against the pattern.

### Example Patterns

1. `a*b` matches `"ab"`, `"aab"`, `"b"`.
2. `a|b` matches `"a"` or `"b"`.
3. `(ab)*` matches `"ab"`, `"abab"`, `""` (empty string).
4. `a.b` matches `"acb`", `"aab"`, `"a*b"` (where \* represents any character).

## Project Structure

- `main.cpp`: The main implementation file that includes the Lexer, Parser, AST nodes, and Matcher.

### Limitations

- The engine currently supports basic regular expression features and does not handle advanced features like character classes, quantifiers other than \*, or non-greedy matching.

## Future Enhancements

- Add support for character classes (e.g., [a-z]).
- Implement additional quantifiers (+, ?, {min,max}).
- Support for anchors (^, $).
- Enhance the engine to support non-greedy matching.
- Add error handling and more informative messages for invalid patterns.

## License

This project is licensed under the [MIT License](/LICENSE). See the LICENSE file for more details.
