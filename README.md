# AVL vs Skip List project

## Project to show performance comparison of AVL self-balancing tree against Skip List in their basic functionalities.

### Contains:
- VS 2022 solution and project files for easier building the whole project
- Documented headers with implementation of template versions of the two structures
- Documented headers with implementation of non-template versions for integers
- Catch2 tests for both structures
- Console application to run customly-made benchmark tests and print the results in a tables that gives imformation for:
 - average insertion, deletion, searching speed, memory used in bytes and comparison as percentage 
 - the slowest insertion, deletion and searching speed time while tests were running
 - average insertion, deletion, searching speed, memory  when structures are used with many elements (simulates cases that are closed to real usage)


### Updates that have been made for performance enhancement in the structures:
- AVL stops balancing checks towards the root when there is no more balancing need in the children
- Skip List has maximum lvl of 35 considering that no more than 2^35 elements will be inserted
- Function to calculate the most optimum fraction for skip list for expected number of elements
- Skip List keeps update array as static so allocation each time don't happen
- Types alignment considered
