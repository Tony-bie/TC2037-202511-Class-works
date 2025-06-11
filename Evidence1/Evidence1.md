# Evidence 1: C++ Syntax Highlighter
**Enrique Antonio Pires Rodríguez - A01424547**

## Reflection about the Program
The program analyzes C++ source files by reading the content as a string and processing it line-by-line using regular expressions for tokenization. The process concludes by creating an HTML file with syntax highlighting that visually differentiates code elements through CSS styling.

## Algorithms

### Tokenization Algorithm
The program processes each line character by character using a greedy pattern matching approach:

1. **Pattern Matching**: Tests 17 predefined regex patterns in sequential order
2. **Token Assignment**: When a match is found, creates a tuple (text, token_type) 
3. **Line Processing**: If there's a match, splits the length of the matched text from the line and recursively calls the function, adding the match with its token to the accumulator
4. **Fallback Handling**: If no match is found, the program splits only one character, scrolls through the line, and continues processing
5. **Termination**: The tokenizer stops when the line is empty, then continues with the next line

### HTML Generation Algorithm
After tokenization is complete:
1. **Mapping Process**: Maps the list of token lists to predetermined HTML code lines
2. **Class Assignment**: Assigns each token a CSS class based on its type
3. **Span Creation**: Wraps each match in HTML spans with appropriate styling
4. **CSS Integration**: Uses CSS to assign colors and formatting to each token class for syntax highlighting

## Execution Time Analysis

| File | Execution Time |
|------|----------------|
| **vertex.hpp** | 16.179 milliseconds |
| **main.cpp** | 21.401 milliseconds |
| **Graph.hpp** | 25.6 milliseconds |
| **Edge.hpp** | 6.114 milliseconds |

The program execution time is consistently under one second, which is acceptable for a program that processes over 100 lines of code and creates a new HTML file with complete syntax highlighting.

## Time Complexity Analysis

### Variables Definition
- `n` = total number of characters in input file
- `m` = number of lines  
- `r` = number of regex patterns

### Tokenization Phase
- **Worst case**: O(r × n) -> when every character requires testing all regex patterns
- **Average case**: O(k × n) where k < r -> when patterns match early in the sequence

### HTML Generation Phase  
- **Complexity**: O(t) where t = total tokens ≈ O(n)

### Overall Complexity
- **Total Time Complexity**: O(r × n) = O(n)  (r is constant)

## Empirical Analysis

Using vertex.hpp as the test case:

- **File size**: 2331 characters
- **Execution time**: 16.179 ms
- **Processing rate**: 144074 characters/second

### Validation
The empirical results align with the theoretical O(n) complexity, showing linear scaling with input size. The consistent execution times across different files confirm the algorithm's predictable performance characteristics.

## Ethical Implications

### Positive Impacts

#### Educational Benefits
- **Enhanced Learning**: Syntax highlighting improves code readability for programming students
- **Accessibility**: Helps developers with visual processing differences distinguish code elements
- **Error Prevention**: Visual cues help identify syntax errors and improve code quality

#### Professional Development
- **Increased Productivity**: Developers can read, debug, and maintain code more efficiently
- **Better Collaboration**: Standardized highlighting improves team communication and code reviews

### Potential Concerns

#### Digital Divide
- **Tool Dependency**: Over-reliance on visual aids might weaken fundamental coding skills
- **Resource Requirements**: Advanced highlighting requires computational resources, potentially excluding users with limited hardware

#### Cognitive Considerations
- **Learning Patterns**: Students might focus on visual cues rather than understanding underlying code logic
- **Attention Bias**: Color coding might create unconscious biases about code importance

### Responsible Implementation

#### Accessibility Standards
- Support for colorblind users through alternative highlighting methods
- Customizable color schemes and contrast options
- Multiple highlighting approaches (colors, patterns, fonts)

#### Educational Balance
- Encourage understanding of code structure beyond visual cues
- Provide options to disable highlighting for fundamental learning exercises
- Maintain focus on logical understanding over aesthetic presentation

## Conclusion

The C++ syntax highlighter demonstrates efficient O(n) linear complexity with practical execution times suitable for typical development workflows. The algorithm processes the file in under one second for token recognition and HTML generation.

From a technical perspective, the implementation shows consistent performance depending on the file size and maintains predictable scaling characteristics.

Regarding ethical implications, this technology improves developer productivity. The tool helps to identify what type of code element each token represents, helping to understand the code faster. Also, changing colors helps people who have color blindness by providing visual differentiation.

The linear time complexity and sub-second execution times validate the algorithm's efficiency for practical use in development environments, making it valuable for both educational and professional contexts.

## Reference
- Expresiones regulares - JavaScript | MDN. (s. f.). MDN Web Docs. https://developer.mozilla.org/es/docs/Web/JavaScript/Guide/Regular_expressions 