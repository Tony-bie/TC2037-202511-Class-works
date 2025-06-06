# Activity 5.2
Enrique Antonio Pires Rodríguez

## How to run the programms with timming?

### Prime number 
- Elixir: iex.bat -> c("Activity.5.exs") -> :timer.tc(fn -> Activity5.prime_numbers(100000) end)
- C++: g++ -o Prime_number .\Prime_number.cpp -> Measure-Command { .\prime_number.exe | Out-Host } | Select-Object TotalMilliseconds

### Hex_bin
- Elixir: iex.bat -> c("Activity.5.exs") -> :timer.tc(fn -> Activity5.hex_bin(25) end)
- C++: g++ -o Hex_bin .\Hex_bin.cpp -> Measure-Command { .\Hex_bin.exe | Out-Host } | Select-Object TotalMilliseconds

## How the programms works
### Prime_number 
Takes a number `n` as input and calculates the sum of all prime numbers in the range [2, n]. Each number is tested for primality using trial division with square root optimization. If a number is prime, it's added to the accumulator.

### Hex_bin
Takes a number `n` as input and counts how many numbers in the range [0, 2^n] have both binary and hexadecimal representations that are palindromes. For each number:
1. Converts to binary and hexadecimal strings
2. Checks if both representations are palindromes (by reversing and comparing)
3. Increments counter if both conditions are met

## Test time and print: Elixir vs C++
### Prime Number Tests

| Input | C++ Time | C++ Output | Elixir Time | Elixir Output |
|-------|----------|------------|-------------|---------------|
| 10 | 54.4783 | 17 | 0 | 17 |
| 100 |220.8262 | 1060 | 0 | 1060 |
| 1000 | 171.9304 | 76127 | .204 | 76127 |
| 10000 | 182.1418 | 5736396 | 4.608 | 5736396 |
| 100000 | 420.2944 | 454396537 | 96.972 | 454396537 |
| 1000000 | 486.7952 | 37550402023 | 1556.684 | 37550402023 |

### Hex_Binary Tests

| Input | C++ Time | C++ Output | Elixir Time | Elixir Output |
|-------|----------|------------|-------------|---------------|
| 10 | 301.6487 | 17 | .716 | 17 |
| 20 | 625.4022 | 125 | 940.236 | 125 |
| 25 | 15013.8275 | 205 | 39573.811 | 205 |

## Perfomance
With low iterations, Elixir has an advantage against C++, demonstrating that Elixir is better with programs that don't process too many iterations. With big iterations like 1000000, C++ demonstrates better performance against Elixir, there's a big difference.
