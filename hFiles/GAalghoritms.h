#ifndef GA_OPERATORS_H
#define GA_OPERATORS_H

int*** rouletteSelection(int ***population, int *fitnesses, int populationSize, int boardN);
int*** tournamentSelection(int ***population, int *fitnesses, int populationSize, int boardN);
int*** rankSelection(int ***population, int *fitnesses, int populationSize, int boardN);

int*** singlePointCrossover(int ***population, int populationSize, int boardN, double crossoverRate);
int*** multiPointCrossover(int ***population, int populationSize, int boardN, double crossoverRate);
int*** uniformCrossover(int ***population, int populationSize, int boardN, double crossoverRate);

int*** mutatePopulation(int ***population, int populationSize, int boardN, double mutationRate);

#endif