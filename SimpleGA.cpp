#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <array>
#include <cmath>
using namespace std;

const int POP_SIZE = 10; // keep it even stevens
const int LENGTH = 8;
const double CROSSOVER_CHANCE = 0.9;
const double MUTATE_RATE = 0.01;
const string TAB = "\t";

vector<array<char, LENGTH>> population;
vector<array<char, LENGTH>> newPopulation;
vector<int> fitness(POP_SIZE);

// selected chromosomes
array<char, LENGTH> chromoA;
array<char, LENGTH> chromoB;
array<char, LENGTH> tempChromoA;
array<char, LENGTH> tempChromoB;

void initRandom() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = now.time_since_epoch();
    auto seed = std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
    std::srand(seed);
}

char randomOneorZero() {
	return (rand() % 2 == 0) ? '1' : '0';
}

void initPopulation() {
	for (int i = 0 ; i < POP_SIZE; i++) {
		// char tempArr[LENGTH];
		array<char, LENGTH> tempArr;
		for (int j = 0; j < LENGTH; j++) {
			tempArr[j] = randomOneorZero();
		}
		population.push_back(tempArr);
	}
}

void printPopulation() {
	for (const auto& ind : population) {
		for (char gene : ind) {
			cout << gene;
		} cout << endl;
	}
	cout << endl;
}

void calcFitness() {
	for (int popIndex = 0; popIndex < POP_SIZE; popIndex++) {
		int numOfOnes = 0;
		for (int chromoIndex = 0; chromoIndex < LENGTH; chromoIndex++) {
			if (population[popIndex][chromoIndex] == '1') {
				numOfOnes++;
			}
		}
		fitness[popIndex] = numOfOnes;
	}
}

void printFitness() {
	for (int popIndex = 0; popIndex < POP_SIZE; popIndex++) {
		// print chromo
		for (int chromoIndex = 0; chromoIndex < LENGTH; chromoIndex++) {
			cout << population[popIndex][chromoIndex];
		} 
		cout << " ";
		cout << fitness[popIndex] << endl;
	}
	cout << endl;
}

// sort by fitness
void sortPopByFitnessDesc() {
    vector<array<char, LENGTH>> tempPopulation;
    vector<int> tempFitness;
    
    int max = 8;
    while (true) {
        for (int index = 0; index < POP_SIZE; index++) {
            if (fitness[index] == max) {
                tempFitness.push_back(fitness[index]);
                tempPopulation.push_back(population[index]);
            }
        }
        max--;
        if (max < 0) {
            break;
        }
    }
    
    for (int i = 0; i < POP_SIZE; i++) {
        fitness[i] = tempFitness[i];
        population[i] = tempPopulation[i];
    }
}

double determineFitnessChanceLowerLimit(int fitness) {
	double chance = 0;
	switch(fitness) {
		case 8:
			chance = 0.595;
			break;
		case 7:
			chance = 0.356;
			break;
		case 6:
			chance = 0.212;
			break;
		case 5:
			chance = 0.126;
			break;
		case 4:
			chance = 0.0754;
			break;
		case 3:
			chance = 0.0450;
			break;
		case 2:
			chance = 0.0268;
			break;
		case 1:
			chance = 0.016;
			break;
		case 0:
			chance = 0.00953;
			break;
		default:	
			chance = 0;
	}
	return chance;
}

double determineFitnessChanceHigherLimit(int fitness) {
	double chance = 0;
	switch(fitness) {
		case 8:
			chance = 1;
			break;
		case 7:
			chance = 0.595;
			break;
		case 6:
			chance = 0.356;
			break;
		case 5:
			chance = 0.356;
			break;
		case 4:
			chance = 0.126;
			break;
		case 3:
			chance = 0.0754;
			break;
		case 2:
			chance = 0.0450;
			break;
		case 1:
			chance = 0.0268;
			break;
		case 0:
			chance = 0.016;
			break;
		default:	
			chance = 0;
	}
	return chance;
}

double randomValueFunc() {
	double randomValue = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	randomValue = round(randomValue * 100) / 100; 
	// cout << "RANDOMVALUE: " << randomValue << endl;
	return randomValue;
}

// selects an index of an chromosome in the population
int selectionFunction() {
	
	int populationIndex = -1;
	
	bool noSelection = true;
	while(noSelection) {
		for (int index = 0; index < POP_SIZE; index++) {
			double randomValue = randomValueFunc();
			double chanceLowerLimit = determineFitnessChanceLowerLimit(fitness[index]);
			double chanceHigherLimit = determineFitnessChanceHigherLimit(fitness[index]);
			if (randomValue >= chanceLowerLimit && randomValue < chanceHigherLimit) {
				
				// print result
				// cout << index << " " << fitness[index] << " ";
				// for (int i = 0; i < LENGTH; i++) {
					// cout << population[index][i];
				// }
				// cout << endl;
				
				populationIndex = index;
				
				noSelection = false;
				break;
			}
		}
	}
	
	return populationIndex;
}

void crossPoint() {
	
	int indexA = selectionFunction();
	for (int i = 0; i < LENGTH; i++) {
		chromoA[i] = population[indexA][i];
	}
	
	int indexB = selectionFunction();
	while(indexA == indexB) {
		indexB = selectionFunction();
	}
	for (int i = 0; i < LENGTH; i++) {
		chromoB[i] = population[indexB][i];
	}
	
	// cout << "chromosome: " << indexA << " and " << indexB << endl;
	
	double randomCrossover = randomValueFunc();
	
	if (randomCrossover < CROSSOVER_CHANCE) {
		int crossoverPoint = (rand() % 7)+1;
		
		// cout << "crossover index: " << crossoverPoint << endl;
		
		for (int i = 0; i < LENGTH; i++) {
			if (i < crossoverPoint) {
				tempChromoA[i] = chromoA[i];
			}
			if (i >= crossoverPoint) {
				tempChromoB[i] = chromoA[i];
			}
		}
		
		for (int i = 0; i < LENGTH; i++) {
			if (i < crossoverPoint) {
				tempChromoB[i] = chromoB[i];
			}
			if (i >= crossoverPoint) {
				tempChromoA[i] = chromoB[i];
			}
		}
		
		// for (int i = 0; i < LENGTH; i++) {
		// cout << chromoA[i] << TAB << chromoB[i] << TAB << tempChromoA[i] << TAB << tempChromoB[i] << endl;
		// }
		
	} else { // children are clones of the parents
		for (int i = 0; i < LENGTH; i++) {
			tempChromoA[i] = chromoA[i];
			tempChromoB[i] = chromoB[i];
		}
	}
}

double randomDoubleBetween0_01and1() {
    double R = static_cast<double>(rand()) / RAND_MAX; // Generates a number between 0 and 1
    R = 0.01 + R * (1.0 - 0.01); // Scales it to the range 0.01 to 1
    return R;
}

char flipCharBit(char bit) {
	if (bit == '0') {
		return '1';
	} 
	else if (bit == '1') {
		return '0';
	}
	else {return 'E';}
}

void mutate() {
	for (int i = 0; i < LENGTH; i++) {
		if (randomDoubleBetween0_01and1() <= 0.1) {
			tempChromoA[i] = flipCharBit(tempChromoA[i]);
		}
		if (randomDoubleBetween0_01and1() <= 0.1) {
			tempChromoB[i] = flipCharBit(tempChromoB[i]);
		}
	}
}

void addChildrenToPopulation() {
	newPopulation.push_back(tempChromoA);
	newPopulation.push_back(tempChromoB);
}

void replaceOldPopulation() {
    population = newPopulation;
    newPopulation.clear();
}

int main() {
    const int numGenerations = 100; // Number of generations to run
	initRandom();
	initPopulation();	
	
	for (int generation = 0; generation < numGenerations; generation++) {
	    calcFitness();
	    sortPopByFitnessDesc();
	    printFitness();
	    
	    for (int i = 0; i < POP_SIZE / 2; i++) {
		    crossPoint();
		    mutate();
		    addChildrenToPopulation();
	    }
	    
	    replaceOldPopulation();
	}
	
	return 0;
}
