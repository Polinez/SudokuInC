#include "hFiles/GA.h"

#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

#include "hFiles/settings.h"
#include "hFiles/sudokuGenerator.h"
#include "hFiles/board.h"
#include "hFiles/game.h"
#include "hFiles/solver.h"
#include "hFiles/GAalghoritms.h"

int fitness(int **board, int **solution, int boardN)
{
    int fitness_value = 0;
    for (int i = 0; i < boardN; i++)
    {
        for (int j = 0; j < boardN; j++)
        {
            if (board[i][j] == solution[i][j])
            {
                fitness_value++;
            }
        }
    }
    return fitness_value;
}

void freePopulation(int ***population, int populationSize, int boardN)
{
    for (int i = 0; i < populationSize; i++)
    {
        freeBoard(population[i], boardN);
    }
    free(population);
}

void initializeGeneration(int** individual,int **board,  int blockSize, int boardN)
{
    copyGrid(board, individual, boardN);

    // Randomly fill empty cells with valid numbers
    insertRandomValues(individual, board, blockSize, boardN);
}


void solve_genetic_algorithm(int **board, int **solution, int blockSize, int boardN, int populationSize, int generations,double crossoverRate,
                             int selection_method,int crossover_method, double mutationRate)
{
    // initialize population
    int ***population = (int ***)malloc(populationSize * sizeof(int **));
    for (int i = 0; i < populationSize; i++)
    {
        allocateBoard(&population[i], boardN);
        initializeGeneration(population[i], board, blockSize, boardN);
    }
    
    // fitness for every individual in population
    int *fitnesses = (int *)malloc(populationSize * sizeof(int));
    int maxFitness = 0;
    int solutionFound = 0;

    // Evaluate initial population
    for (int i = 0; i < populationSize; i++) {
        fitnesses[i] = fitness(population[i], solution, boardN);
        if (fitnesses[i] > maxFitness) {
            maxFitness = fitnesses[i];
        }
    }

    // Check initial population
    if (maxFitness == boardN * boardN) {
        printf("Solution found in initial population:\n");
        for (int i = 0; i < populationSize; i++) {
            if (fitnesses[i] == maxFitness) {
                printBoard(population[i], blockSize, boardN);
                solutionFound = 1;
                break;
            }
        }
    }

    // Main loop for the genetic algorithm
    for (int generation = 0; generation < generations; generation++)
    {
        // Selection in GAalgoritmh.c:
        if (selection_method == 0) //rullet selection
        {
            population = rouletteSelection(population, fitnesses, populationSize, boardN);
        }
        else if (selection_method == 1)//turnament selection
        {
            population = tournamentSelection(population, fitnesses, populationSize, boardN);
        }
        else //rank selection
        {
            population = rankSelection(population, fitnesses, populationSize, boardN);
        }


        // crossovers in GAalgoritmh.c
        if (crossover_method == 0) // Single-Point Crossover
        {
            population = singlePointCrossover(population, populationSize, boardN, crossoverRate);
        }
        else if (crossover_method == 1) //Multi-Point Crossover
        {
            population = multiPointCrossover(population, populationSize, boardN, crossoverRate);
        }
        else //Uniform Crossover
        {
            population = uniformCrossover(population, populationSize, boardN, crossoverRate);
        }

        // Mutation in GAalgoritmh.c
        population = mutatePopulation(population, populationSize, boardN, mutationRate);




        // Reallocate fitness array for new population
        free(fitnesses);
        fitnesses = malloc(populationSize * sizeof(int));
        maxFitness = 0;

        // create new fitnesses list for new population
        for (int i = 0; i < populationSize; i++) {
            fitnesses[i] = fitness(population[i], solution, boardN);
            if (fitnesses[i] > maxFitness) {
                maxFitness = fitnesses[i];
            }
        }

        printf("Generation %d: Max Fitness = %d Serching Fitness= %d\n", generation+1, maxFitness, boardN * boardN);

        // Check for solution
        if (maxFitness == boardN * boardN) {
            printf("Solution found in generation %d:\n", generation+1);
            for (int i = 0; i < populationSize; i++) {
                if (fitnesses[i] == maxFitness) {
                    printBoard(population[i], blockSize, boardN);
                    solutionFound = 1;
                    break;
                }
            }
        }
        if (solutionFound==1) {
            break; // Exit if a solution is found
        }
    }

    if (!solutionFound) {
        printf("No solution found after %d generations\n", generations);
    }

    // Free resources
    free(fitnesses);
    freePopulation(population, populationSize, boardN);
}



void Find_GA_Solution(int* boardN, int* k, int* level)
{
    clear_screen();

    int blockSize = (int)sqrt(*boardN); // √n

    int **board = NULL;
    int **solution = NULL;
    allocateBoard(&board,*boardN);
    allocateBoard(&solution,*boardN);
    initializeGame(board,solution,*boardN, *k, blockSize);

    // GA parametrers
    int populationSize = 1000;
    int generations = 1000;
    double crossoverRate = 0.8;
    double mutationRate = 0.1;
    int selection_method = 1; // 0 - rullet, 1 - turnament, 2 - rank
    int crossover_method = 1; // 0 - single-point, 1 - multi-point, 2 - uniform


    solve_genetic_algorithm(board, solution, blockSize,*boardN,populationSize, generations,crossoverRate , selection_method,crossover_method, mutationRate);

    printf("\n----- How it should look like -----\n");
    printBoard(solution, blockSize, *boardN);

    printf("\nPress Enter to continue...");
    while (getchar() != '\n');
    getchar();
}
