# SMT Model for Differential Cryptanalysis of BORON
Instructions for generate-cvc-boron-cluster-8r.c:
1. Set "num_characteristics" based on the number of characteristics that need to be excluded from the search.
2. All characteristics that have been found so far is listed in the "inter_diff" array. 
3. Set "total_probability" based on the target weight.
4. Execute the script to generate the .cvc file that has all the constraints.
5. Run an SMT solver to find the solution for the .cvc file. A compatible SMT solver is STP (https://stp.readthedocs.io/).
6. Output the results into a file and uses the file_parser.cpp to read (remember to change the file name and number of rounds).


