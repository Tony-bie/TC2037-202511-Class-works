# Evidence 2
**By: Enrique Antonio Pires Rodríguez - A01424547**

## Algorithm analysis 
This program is an enhancement of Evidence 1, incorporating two main processing approaches for syntax highlighting. The program now accepts either a single file name or a list of file names as input.

**Sequential Processing:** Files are processed one after another in order. The program completes the syntax highlighting of the first file before proceeding to the second, continuing this pattern until all files are processed.

**Parallel Processing:** Multiple files are processed simultaneously using modern multi-core CPU capabilities, distributing the workload across different cores of the computer. The effectiveness comes from Elixir's lightweight processes, where each file processing task runs independently without shared state, eliminating synchronization overhead that often limits parallel performance in other languages.

## How to execute the program with timing and AVG?
To run the program, in the terminal you have to execute `elixir .\Evidence2.exs`

I added a `for` loop to process both algorithms 10 times. When you run it, the `for` loop returns a list of execution times. I store this list in a variable, then sum all the times and calculate the average to get more accurate performance measurements. This approach helps eliminate variations caused by system load and provides more reliable benchmarking results.

## Time sequentially apply vs modern multi-core

The files:
1. BankSystem.hpp
2. EmployeeManager.cpp
3. Library.hpp
4. OnlineStore.hpp
5. LinkedList.hpp
6. TaskManager.hpp

- Total of words: 15252 words
- Time sequentially AVG (s): 0.41444329999999996 seconds
- Modern multi-core AVG (s): 0.0906849 seconds
- Speed up: 4.570146738872734 faster

## Reflective Analysis
The modern multi-core demonstrates faster processing than a sequential method. This occurs because Multi-core processing, instead of processing each file, processes every file in parallel. This helps to save time. With 6 files, the impact doesn't look too significant, but if you process hundreds, thousands or millions of files, the time difference will be substantial.

## Time Complexity Analysis
### Variables
- f: files
- m: numbers of words average
- k: tokens per word
- c: cores
### Complexity
- Sequential apply: O(f * m * k)
- Modern multi-core: O((f * m * k) / c)

## Positive impacts
- Dramatically reduced processing time: The results show 4.57 speedup, transforming 0.414 seconds to 0.091. This demonstrates exponentially more valuable, where programs that take hours of processing can be reduced to minutes.
- Optimal hardware resource utilization: Instead of using only one CPU core while others remain idle, parallel processing maximizes the investment in modern multi-core systems by distributing workload across all available cores.
- Non-linear scalability benefits: Instead of time increasing proportionally with file count, the CPU can handle double the number of files without doubling processing time. This efficiency gain becomes more pronounced with larger workloads.

## The potential about this
For example, my gaming computer has 14 cores, so it can process 14 tasks in parallel. If my computer can do this, imagine computers more powerful or computers from the future. How many cores will they have? This will reduce the time needed for a program to process anything. With this we can solve problems that humans can't. 

## Conclusion
Programs that utilize multiple cores for processing are significantly faster than traditional sequential methods. This helps to reduce processing time - in this case, approximately 4.5 times faster. The parallel implementation demonstrates the effectiveness of modern multi-core architectures for computationally intensive tasks.

## References
- Based on material from the Parallel Programming course, Prof. Gilberto Echeverría
- Chriserin. (2024, 12 mayo). Timing A Function In Elixir. Hashrocket. Recuperado 8 de junio de 2025, de https://til.hashrocket.com/posts/9jxsfxysey-timing-a-function-in-elixir 
- Comprehensions — Elixir v1.18.4. (s. f.). https://hexdocs.pm/elixir/comprehensions.html