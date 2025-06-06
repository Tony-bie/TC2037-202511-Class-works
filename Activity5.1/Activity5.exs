defmodule Activity5 do
  def prime_numbers(n) do
    cond do
      n<2 ->
        0
      n==2 ->
        2
      n>2 ->
        sum(n,2,0)
    end
  end

  defp sum(n, number, total) when number > n, do: total
  defp sum(n, number, total), do: is_prime_number(n, number, total, 2)

  defp is_prime_number(n, number, total, begin) do
    sqrt_number = :math.sqrt(number)
    if begin <= sqrt_number do
      if rem(number, begin) == 0 do
        sum(n, number + 1, total)
      else
        is_prime_number(n, number, total, begin + 1)
      end
    else
      sum(n, number + 1, total + number)
    end
  end

  def hex_bin(n), do: hex_bin_recursive(n, 0, 0)

  defp hex_bin_recursive(n, begin, total) do
    limit = trunc(:math.pow(2, n))
    if begin > limit do
      total
    else
      if hex_palindromes(begin, "") && binary_palindromes(begin, "") do
        hex_bin_recursive(n, begin + 1, total + 1)
      else
        hex_bin_recursive(n, begin + 1, total)
      end
    end
  end

  defp hex_palindromes(begin, hex) when begin > 0 do
    cons = rem(begin, 16)
    digit = cond do
      cons < 10 -> Integer.to_string(cons)
      cons == 10 -> "A"
      cons == 11 -> "B"
      cons == 12 -> "C"
      cons == 13 -> "D"
      cons == 14 -> "E"
      cons == 15 -> "F"
    end
    hex_palindromes(div(begin, 16), digit <> hex)
  end
  defp hex_palindromes(0, hex), do: String.reverse(hex) == hex

  defp binary_palindromes(begin, binary) when begin > 0 do
    digit = if rem(begin, 2) == 0, do: "0", else: "1"
    binary_palindromes(div(begin, 2), digit <> binary)
  end
  defp binary_palindromes(0, binary), do: String.reverse(binary) == binary
end
