#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hFiles/board.h"
#include "hFiles/sudokuGenerator.h"

//selectors
int*** rouletteSelection(int ***population, int *fitnesses, int populationSize, int boardN)
{
    int totalFitness = 0;
    for (int i = 0; i < populationSize; i++)
    {
        totalFitness += fitnesses[i];
    }

    // Create a new population
    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++)
    {
        allocateBoard(&newPopulation[i], boardN);
    }

    // Select individuals based on their fitness
    for (int i = 0; i < populationSize; i++)
    {
        double randValue = ((double)rand() / RAND_MAX) * totalFitness; // random number from 0 to totalFitness
        double cumulativeFitness = 0.0;

        for (int j = 0; j < populationSize; j++)
        {
            cumulativeFitness += fitnesses[j];
            if (cumulativeFitness >= randValue)
            {
                copyGrid(population[j], newPopulation[i], boardN);
                break;
            }
        }
    }

    return newPopulation;
}

int*** tournamentSelection(int ***population, int *fitnesses, int populationSize, int boardN)
{
    int tournamentSize = 3;
    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++) {
        allocateBoard(&newPopulation[i], boardN);

        // select the best individual from a random tournament
        int bestIdx = rand() % populationSize;
        int bestFitness = fitnesses[bestIdx];
        for (int j = 1; j < tournamentSize; j++) {
            int idx = rand() % populationSize;
            if (fitnesses[idx] > bestFitness) {
                bestIdx = idx;
                bestFitness = fitnesses[idx];
            }
        }
        copyGrid(population[bestIdx], newPopulation[i], boardN);
    }


    return newPopulation;
}

int*** rankSelection(int ***population, int *fitnesses, int populationSize, int boardN)
{
    // create list of indexes
    int *indices = (int *)malloc(populationSize * sizeof(int));
    for (int i = 0; i < populationSize; i++) {
        indices[i] = i;
    }

    // sorting fitnesses
    for (int i = 0; i < populationSize - 1; i++) {
        for (int j = 0; j < populationSize - i - 1; j++) {
            if (fitnesses[indices[j]] < fitnesses[indices[j + 1]]) {
                int tmp = indices[j];
                indices[j] = indices[j + 1];
                indices[j + 1] = tmp;
            }
        }
    }

    // creating ranks best -> hingest rank
    int *ranks = (int *)malloc(populationSize * sizeof(int));
    for (int i = 0; i < populationSize; i++) {
        ranks[i] = populationSize - i;
    }

    int totalRank = (populationSize * (populationSize + 1)) / 2;

    //new population
    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++) {
        allocateBoard(&newPopulation[i], boardN);
    }

    // select individuals based on their rank
    for (int i = 0; i < populationSize; i++) {
        double randValue = ((double)rand() / RAND_MAX) * totalRank;
        double cumulativeRank = 0.0;

        for (int j = 0; j < populationSize; j++) {
            int individualIdx = indices[j];
            cumulativeRank += ranks[j];

            if (cumulativeRank >= randValue) {
                copyGrid(population[individualIdx], newPopulation[i], boardN);
                break;
            }
        }
    }

    free(indices);
    free(ranks);

    return newPopulation;
}

//crossovers
int*** singlePointCrossover(int ***population, int populationSize, int boardN, double crossoverRate)
{
    int totalCells = boardN * boardN;

    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++)
    {
        allocateBoard(&newPopulation[i], boardN);
    }

    for (int i = 0; i < populationSize; i += 2) {
        if (((double)rand() / RAND_MAX) < crossoverRate && i + 1 < populationSize) {

            int crossoverPoint = rand() % totalCells; // Random crossover point in the range of total cells

            for (int idx = 0; idx < totalCells; idx++) {
                // calculating row and column from the index
                int row = idx / boardN;
                int col = idx % boardN;

                if (idx <= crossoverPoint) {
                    // swap from 0 to crossoverPoint
                    newPopulation[i][row][col]     = population[i][row][col];
                    newPopulation[i+1][row][col]   = population[i+1][row][col];
                } else {
                    //swap the rest
                    newPopulation[i][row][col]     = population[i+1][row][col];
                    newPopulation[i+1][row][col]   = population[i][row][col];
                }
            }
        }
        else {
            // If no crossover, just copy the individuals
            copyGrid(population[i],       newPopulation[i],     boardN);
            if (i + 1 < populationSize) {
                copyGrid(population[i + 1], newPopulation[i + 1], boardN);
            }
        }
    }

    return newPopulation;
}

int*** multiPointCrossover(int ***population, int populationSize, int boardN, double crossoverRate)
{

    int totalCells = boardN * boardN;

    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++)
    {
        allocateBoard(&newPopulation[i], boardN);
    }


    for (int i = 0; i < populationSize; i += 2) {
        if (((double)rand() / RAND_MAX) < crossoverRate && i + 1 < populationSize) {

            int crossoverPoint1 = rand() % totalCells; // First crossover point
            int crossoverPoint2 = rand() % totalCells; // Second crossover point

            // Ensure that crossoverPoint1 is less than crossoverPoint2
            if (crossoverPoint1 > crossoverPoint2) {
                int temp = crossoverPoint1;
                crossoverPoint1 = crossoverPoint2;
                crossoverPoint2 = temp;
            }

            for (int idx = 0; idx < totalCells; idx++) {
                // calculating row and column from the index
                int row = idx / boardN;
                int col = idx % boardN;

                if (idx <= crossoverPoint1 || (idx > crossoverPoint2 && idx < totalCells)) {
                    // swap from 0 to first crossover point and from second to end
                    newPopulation[i][row][col]     = population[i][row][col];
                    newPopulation[i+1][row][col]   = population[i+1][row][col];
                } else {
                    //swap between first and second crossover points
                    newPopulation[i][row][col]     = population[i+1][row][col];
                    newPopulation[i+1][row][col]   = population[i][row][col];
                }
            }
        }
        else {
            // If no crossover, just copy the individuals
            copyGrid(population[i],       newPopulation[i],     boardN);
            if (i + 1 < populationSize) {
                copyGrid(population[i + 1], newPopulation[i + 1], boardN);
            }
        }
    }



    return newPopulation;
}

int*** uniformCrossover(int ***population, int populationSize, int boardN, double crossoverRate)
{
    int totalCells = boardN * boardN;

    int ***newPopulation = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++)
    {
        allocateBoard(&newPopulation[i], boardN);
    }

    for (int i = 0; i < populationSize; i += 2) {
        if (((double)rand() / RAND_MAX) < crossoverRate && i + 1 < populationSize) {
            for (int idx = 0; idx < totalCells; idx++) {
                int row = idx / boardN;
                int col = idx % boardN;
                if (rand() % 2) { // Randomly choose which parent to take from
                    newPopulation[i][row][col]     = population[i][row][col];
                    newPopulation[i+1][row][col]   = population[i+1][row][col];
                } else {
                    newPopulation[i][row][col]     = population[i+1][row][col];
                    newPopulation[i+1][row][col]   = population[i][row][col];
                }
            }
        } else {
            copyGrid(population[i], newPopulation[i], boardN);
            if (i + 1 < populationSize) {
                copyGrid(population[i + 1], newPopulation[i + 1], boardN);
            }
        }
    }

    return newPopulation;
}

// mutation
int*** mutatePopulation(int ***population, int populationSize, int boardN, double mutationRate)
{
    for (int i = 0; i < populationSize; i++)
    {
        for (int j = 0; j < boardN; j++)
        {
            if (((double)rand() / RAND_MAX) < mutationRate) // Randomly mutate with a given probability
            {
                int row = rand() % boardN;
                int col = rand() % boardN;
                int newValue = (rand() % boardN) + 1; // New value between 1 and boardN
                population[i][row][col] = newValue;
            }
        }
    }
    return population;
}