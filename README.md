This is an genetic algorithmn implementation using the explanation from chapter 1.6 of the book An Introduction to Genetic Algorithms by Melanie Mitchell.
The exact implentation of the algorithm in C++ is the first working solution I came up with. It's in dire need of refinement. But it serves mainly as an hands on education of how the algorithm may work.
A population of chromosomes represented by a bit strings have a fitness from 0 to 8 depending on how many 1's are in the string. 
The goal of this implementation is to evolve the population via selection, crossover and mutation to increase the overall fitness of the population.
