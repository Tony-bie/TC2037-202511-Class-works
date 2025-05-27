
#Enrique Antonio Pires Rodríguez
#A01424547
#Lexical analysis of a C++ file and syntax highlighting in HTML.


#Note: To run the code,put this in the terminal: "elixir Evidence1 File.cpp"
defmodule CppLexer do

#Defining regular expressions for different types of tokens with modules
  @comment_single_regex ~r/^\/\/.*/
  @comment_multi_regex ~r/^\/\*.*?\*\//
  @string_regex ~r/^"[^"]*"/
  @char_regex ~r/^'[^']*'/
  @number_regex ~r/^(\d+\.\d+|\d+)/
  @preprocessor_regex ~r/^#\w+/
  @header_regex ~r/^<\w+>/
  @template_regex ~r/^template/
  @keyword_regex ~r/^(class|struct|int|void|char|bool|float|double|string|for|while|if|else|return|new|delete|public|private|protected|static|const|virtual|namespace|using|typedef|auto|long|short|unsigned|signed)/
  @std_regex ~r/^std::/
  @scope_regex ~r/^::/
  @class_name_regex ~r/^[A-Z]\w*/
  @function_regex ~r/^[a-z_]\w*(?=\s*\()/
  @identifier_regex ~r/^[a-zA-Z_]\w*/
  @operator_regex ~r/^(\+\+|--|->|<<|>>|<=|>=|==|!=|&&|\|\||[+\-*\/%<>=!&|^~])/
  @punctuation_regex ~r/^[(){}\[\];:,.<>]/
  @space_regex ~r/^\s+/

#Variables added to a module list with their corresponding token types,
#next to it are your CSS tags, each token type is assigned a different color.
  @regex_list [
    {@comment_single_regex, :comment},
    {@comment_multi_regex, :comment},
    {@string_regex, :string},
    {@char_regex, :character},
    {@number_regex, :number},
    {@preprocessor_regex, :preprocessor},
    {@header_regex, :header},
    {@template_regex, :template_keyword},
    {@keyword_regex, :keyword},
    {@std_regex, :namespace},
    {@scope_regex, :scope},
    {@function_regex, :function},
    {@class_name_regex, :class},
    {@operator_regex, :operator},
    {@punctuation_regex, :punctuation},
    {@identifier_regex, :identifier},
    {@space_regex, :space}
  ]
  #Main function that receives the name of the input file
  def analyze(input_file) do
    name = Path.rootname(input_file)
    html_file = "#{name}.html" #Create the html file name

    content = File.read!(input_file) #Read the file
    tokens = tokenize(content) #Tokenize the file content
    html = generate_html(tokens) #Send the tokens to the function that generates the HTML

    File.write!(html_file, html) #Write the HTML to a file
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
        {_, resto} = String.split_at(line, 1)
        tokenize_line(resto, list)
    end
  end

  #Generation of html
  defp generate_html(tokens) do
    """
    <!Doctype html>
    <html>
    <head>
      <link rel="stylesheet" href="style.css">
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
