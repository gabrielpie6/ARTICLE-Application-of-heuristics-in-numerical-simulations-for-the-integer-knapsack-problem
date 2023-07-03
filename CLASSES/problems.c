#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

// #include "sort.h"
// typedef struct StCell
// {
//     double value;
//     double weight;
//     double factor;
// } cell;
// bool compare (Item a, Item b)
// {
//     cell * item_a = (cell *) a;
//     cell * item_b = (cell *) b;
//     return item_a->factor < item_b->factor;
// }

// int random_double (int min, int max) {
//     return rand() / RAND_MAX * (max - min) + min;
// }

// void GenerateProblem (char * filename, int VARS_TOTAL, double capacity, double * VARS_VALUES_RANGE, double * VARS_WEIGHTS_RANGE)
// {
//     srand(time(NULL));
//     FILE * file = fopen(filename, "w");
//     cell * Items[VARS_TOTAL];
    
//     /*
//         Gerar numeros randomicos dentro de um intervalo especificado, cada linha é
//         uma variável do problema
//         Todo arquivo de problema gerado deve seguir o seguinte formato:
        
//     in "filename.txt"
//         <qtd_total_vars> <capacity> [VARS_VALUES_RANGE] [VARS_WEIGHTS_RANGE]
//         <peso_var_1> <qtd_var_1>
//         ...
//         <peso_var_n> <qtd_var_n>
//     */

//     fprintf(file, "%d %lf %lf %lf %lf %lf\n", VARS_TOTAL, capacity, VARS_VALUES_RANGE[0], VARS_VALUES_RANGE[1], VARS_WEIGHTS_RANGE[0], VARS_WEIGHTS_RANGE[1]);
//     for (int i = 0; i < VARS_TOTAL; i++)
//     {
//         Items[i] = malloc(sizeof(cell));
//         Items[i]->value  = random_double(VARS_VALUES_RANGE[0], VARS_VALUES_RANGE[1]);
//         Items[i]->weight = random_double(VARS_WEIGHTS_RANGE[0], VARS_WEIGHTS_RANGE[1]);
//         Items[i]->factor = Items[i]->weight/Items[i]->value;

//     }
//     Array array = (Array) Items;
//     BubbleSort(array, compare, VARS_TOTAL, false);
//     for (int i = 0; i < VARS_TOTAL; i++)
//         fprintf(file, "%lf %lf %lf\n", Items[i]->weight, Items[i]->value, Items[i]->factor);
//     fclose(file);
// }


typedef struct Class {
    int itens;
    int capacity;
    int values_range[2];
    int weights_range[2];
    char * name;
} class;

int randomIntRange (int min, int max)
{
    return min + random() % (max - min + 1);
}

void GenerateIntProblem(char * filename, int VARS_TOTAL, int capacity, int * VARS_WEIGHTS_RANGE, int * VARS_VALUES_RANGE)
{
    FILE * file = fopen(filename, "w");

    fprintf(file, "%d %d %d %d %d %d\n", VARS_TOTAL, capacity, VARS_WEIGHTS_RANGE[0], VARS_WEIGHTS_RANGE[1], VARS_VALUES_RANGE[0], VARS_VALUES_RANGE[1]);
    for (int i = 0; i < VARS_TOTAL; i++)
        fprintf(file, "%d %d\n", randomIntRange(VARS_WEIGHTS_RANGE[0], VARS_WEIGHTS_RANGE[1]), randomIntRange(VARS_VALUES_RANGE[0], VARS_VALUES_RANGE[1]));
    fclose(file);
}

void main()
{
    srandom(time(NULL));
    /*
        Classe P1: Mochilas de 100 itens com capacidade de valor 500, os itens devem ter tamanho entre 10 e 200 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.

        Classe P2: Mochilas de 100 itens com capacidade de valor 500, os itens devem ter tamanho entre 200 e 500 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.

        Classe M1: Mochilas de 1.000 itens com capacidade de valor 500, os itens devem ter tamanho entre 10 e 200 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.

        Classe M2: Mochilas de 1.000 itens com capacidade de valor 500, os itens devem ter tamanho entre 200 e 500 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.

        Classe G1: Mochilas de 10.000 itens com capacidade de valor 500, os itens devem ter tamanho entre 10 e 200 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.

        Classe G2: Mochilas de 10.000 itens com capacidade de valor 500, os itens devem ter tamanho entre 200 e 500 e
        as prioridades valores entre 10 e 100. Deverão ser gerados 100 exemplos.
    */
    class P1 = {100,    500, {10,   200}, {10, 100}, "P1"};
    class P2 = {100,    500, {200,  500}, {10, 100}, "P2"};
    class M1 = {1000,   500, {10,   200}, {10, 100}, "M1"};
    class M2 = {1000,   500, {200,  500}, {10, 100}, "M2"};
    class G1 = {10000,  500, {10,   200}, {10, 100}, "G1"};
    class G2 = {10000,  500, {200,  500}, {10, 100}, "G2"};

    class * classes[] = {&P1, &P2, &M1, &M2, &G1, &G2};
    char filename[] = "CC/problem-CC-000.txt";
    
    for (int i = 0; i < 6; i++)
    {
        filename[0] = classes[i]->name[0];
        filename[1] = classes[i]->name[1];

        filename[11] = classes[i]->name[0];
        filename[12] = classes[i]->name[1];
        
        for (int j = 1; j <= 100; j++)
        {
            filename[14] = '0' + (j/100)%10;
            filename[15] = '0' + (j/10)%10;
            filename[16] = '0' + (j/1)%10;
            GenerateIntProblem (
                filename,
                classes[i]->itens,
                classes[i]->capacity,
                classes[i]->weights_range,
                classes[i]->values_range
            );
        }
    }
}