
#Enrique Antonio Pires Rodríguez
#A01424547
#Lexical analysis of a C++ file and syntax highlighting in HTML.


#Note: To run the code,put this in the terminal: "elixir Evidence1 File.cpp"
defmodule CppLexer do


#Defining regular expressions for different types of tokens with modules
  @include_directive_regex ~r/^(#include)/
  @class_regex ~r/^([A-Z][a-z]*)/
  @header_regex ~r/^(<[^>]+>|"[^"]+")/
  @define_regex ~r/^(#define)/
  @define_text_regex ~r/^([A-Z][A-Z0-9_]*)/
  @comment_single_regex ~r/^(\/\/.*)/
  @comment_multi_start_regex ~r/^(\/\*)/
  @comment_multi_end_regex ~r/^(.*?\*\/)/
  @std_regex ~r/^(std::)/
  @vector_class_regex ~r/^(vector|[A-Z][a-z]*)/
  @type_regex ~r/^(void|int|string|float|double|char|auto|bool|long|short|unsigned|signed|wchar_t|size_t)/
  @keyword_regex ~r/^(for|while|do|if|else|switch|case|default|break|continue|return|new|delete|try|catch|throw|class|struct|template|namespace|using|typedef|virtual|friend|static|const|inline|explicit|operator|sizeof|private|public|protected)/
  @function_regex ~r/^([a-zA-Z_][a-zA-Z0-9_]*)\s*\(/
  @identifier_regex ~r/^([a-zA-Z_][a-zA-Z0-9_]*)/
  @number_regex ~r/^(0[xX][0-9a-fA-F]+|0[bB][01]+|0[0-7]+|\d+\.\d+([eE][+-]?\d+)?|\d+)/
  @string_regex ~r/^"([^"\\]|\\.)*"/
  @char_regex ~r/^'([^'\\]|\\.)*'/
  @operator_regex ~r/^(\+\+|--|->|\+=|-=|\*=|\/=|%=|&=|\|=|\^=|<<=|>>=|<<|>>|==|!=|<=|>=|&&|\|\||::|<=>|[+\-*\/%<>=!&|^~.,])/
  @punctuation_regex ~r/^([\[\]\(\)\{\};:])/


# Variables added to a module list with their corresponding token types,
#next to it are your CSS tags, each token type is assigned a different color.
  @regex_list [
    {@include_directive_regex, :preprocessor},
    {@class_regex, :class},
    {@header_regex, :string},
    {@define_regex, :preprocessor},
    {@define_text_regex, :constant},
    {@comment_single_regex, :comment},
    {@comment_multi_start_regex, :comment},
    {@comment_multi_end_regex, :comment},
    {@std_regex, :namespace},
    {@vector_class_regex, :container},
    {@type_regex, :type},
    {@keyword_regex, :keyword},
    {@function_regex, :function},
    {@identifier_regex, :identifier},
    {@number_regex, :number},
    {@string_regex, :string},
    {@char_regex, :character},
    {@operator_regex, :operator},
    {@punctuation_regex, :punctuation}
  ]

  #Main function that receives the name of the input file
  def analyze(input_file) do
    html_file = "SyntaxCPP.html" #Create the html file name
    css_file = "token_colors.css" #Create css file name

    content = File.read!(input_file) #Read the file
    tokens = tokenize(content) #Tokenize the file content
    html = generate_html(tokens) #Send the tokens to the function that generates the HTML

    css = generate_css() #Generates CSS for syntax highlighting

    File.write!(html_file, html) #Write the HTML to a file
    File.write!(css_file, css) #Write the CSS to a file
  end

  #Function that take the files's content to tokenize as paragraphs
  defp tokenize(content) do
    content
    |>String.split("\n")
    |>Enum.map(fn line ->
      tokenize_line(line, [])
    end)
  end

  #If line is empty then return the list revese
  defp tokenize_line("",list), do: Enum.reverse(list)

  #If line is not empty, this function starts
  defp tokenize_line(line,list) do
    #Result equals method that search the coincidence with the tokens and return in a case that
    #if there's a coincidence, is assigned the name that match and its token
    result = Enum.find_value(@regex_list, fn {regex, type} ->
      case Regex.run(regex,line, capture: :first) do
        [match] -> {match, type}
        nil -> nil
      end
    end)
    #We use the result to create two cases, if it's matched, then we split the part of the line
    #that has the word matched and continue
    #if it's nil, we split it too but the length will be only the space, finishing when there is a word
    case result do
      {match, type} ->
        {_,resto} = String.split_at(line, String.length(match))
        tokenize_line(resto, [{match, type} | list])
      nil ->
        {char, resto} = String.split_at(line, 1)
        tokenize_line(resto, [{char, :nil} | list])
    end
  end

  #Generation of html
  defp generate_html(tokens) do
    """
    <!Doctype html>
    <html>
    <head>
      <link rel="stylesheet" href="token_colors.css">
    </head>
    <body>
      <pre>#{generate_spans(tokens)}</pre>
    </body>
    </html>
    """
  end

  #We map the tokens and take as a paragraph
  defp generate_spans(tokens) do
    tokens
    |>Enum.map(fn line->
      generate_line_spans(line)
    end)
    |>Enum.join("\n")
  end
  #We map the line and take only the match with its token
  defp generate_line_spans(line) do
    line
    |> Enum.map(fn {text, type} ->
      "<span class=\"#{type}\">#{escape_html(text)}</span>"
    end)
    |>Enum.join("")
  end

  #Generator of css
  def generate_css do
    """
        body {
        font-family: 'Courier New', monospace;
        line-height: 1.5;
        margin: 20px;
        background-color: #f5f5f5;

    }

    pre {
        background-color: #ffffff;
        padding: 15px;
        border-radius: 5px;
        border: 1px solid #ddd;
        overflow-x: auto;
    }

    /* Estilos para los diferentes tipos de token */
    .keyword {
        color: #0000ff;
        font-weight: bold;
    }

    .preprocessor {
        color: #008000;
    }

    .comment {
        color: #008000;
        font-style: italic;
    }

    .string {
        color: #a31515;
    }

    .character {
        color: #a31515;
    }

    .number {
        color: #098658;
    }

    .identifier {
        color: #000000;
    }

    .operator {
        color: #800000;
    }

    .punctuation {
        color: #000000;
    }

    .type {
        color: #2b91af;
        font-weight: bold;
    }

    .function {
        color: #795e26;
    }

    .namespace {
        color: #2b91af;
    }

    .container {
        color: #2b91af;
    }

    .constant {
        color: #0000ff;
    }

    .include {
        color: #008000;
    }

    .define {
        color: #008000;
        font-weight: bold;
    }

    .class{
      color: #c757ff
    }
    """
  end

  #Changer of character to avoid conflicts with the html
  defp escape_html(text) do
    text
    |> String.replace("&", "&amp;")
    |> String.replace("<", "&lt;")
    |> String.replace(">", "&gt;")
    |> String.replace("\"", "&quot;")
  end
end

input_file = System.argv()
CppLexer.analyze(input_file)
