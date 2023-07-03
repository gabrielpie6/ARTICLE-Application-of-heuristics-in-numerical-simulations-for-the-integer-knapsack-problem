#include "heuristics.h"
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>

//#define _POSIX_C_SOURCE 199309L

typedef struct StVar {
    int qty;
    int value;
    int priority;
    double factor;
} Var;

typedef struct StBackpack {
    int variables;
    int capacity;
    Var ** items;
} Backpack;

typedef struct StSolData {
    int * solution;
    int occupied_space;
    int accumulated_priority;
    double execution_time;
} SolData;

Problem ReadProblem (char * filename)
{
    FILE * file = fopen(filename, "r");
    int VARS_TOTAL;

    Backpack * pack = malloc(sizeof(Backpack));
    fscanf(file, "%d %d %*d %*d %*d %*d", &VARS_TOTAL, &pack->capacity);
    pack->variables = VARS_TOTAL;
    pack->items = malloc(sizeof(Var *) * VARS_TOTAL);
    for (int i = 0; i < VARS_TOTAL; i++)
    {
        pack->items[i] = malloc(sizeof(Var));
        pack->items[i]->qty = 0;
        fscanf(file, "%d %d", &pack->items[i]->priority, &pack->items[i]->value);
    }
    fclose(file);
    return (Problem) pack;
}

void PrintVariables(Backpack * pack)
{
    printf("%s %3s %3s\n", "QTY", "PRI", "VAL");
    for (int i = 0; i < pack->variables; i++)
        printf("[%d] %3d %3d\n", pack->items[i]->qty, pack->items[i]->priority, pack->items[i]->value);
}



typedef void * Item;
typedef Item * Array;
typedef bool (*Compare)(Item A, Item B);

bool CompareByValue (Item a, Item b)
{
    Var * item_a = (Var *) a;
    Var * item_b = (Var *) b;
    return item_a->value > item_b->value;
}

bool CompareByPriority (Item a, Item b)
{
    Var * item_a = (Var *) a;
    Var * item_b = (Var *) b;
    return item_a->priority > item_b->priority;
}

bool CompareByFactor (Item a, Item b)
{
    Var * item_a = (Var *) a;
    Var * item_b = (Var *) b;
    return item_a->factor > item_b->factor;
}

void ExportSolution (char * filename, Problem data, Solution solution)
{
    Backpack * pack = (Backpack *) data;
    SolData * sol = (SolData *) solution;
    FILE * file = fopen(filename, "w");
    for (int i = 0; i < pack->variables; i++)
        fprintf(file, "%3d ", sol->solution[i]);
    fprintf(file, "\nREMAINING SPACE:      %d\n", pack->capacity - sol->occupied_space);
    fprintf(file, "ACCUMULATED PRIORITY: %d\n", sol->accumulated_priority);
    fprintf(file, "EXECUTION TIME:       %.9lf\n", sol->execution_time);
    fprintf(file, "%s %3s %3s\n", "QTY", "PRI", "VAL");
    for (int i = 0; i < pack->variables; i++)
        fprintf(file, "[%d] %3d %3d\n", pack->items[i]->qty, pack->items[i]->priority, pack->items[i]->value);
    fclose(file);
}

void RemoveSolution (Solution solution)
{
    SolData * sol = (SolData *) solution;
    free(sol->solution);
    free(sol);
}








Solution MOT  (Problem data)
{
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    Backpack * pack           = (Backpack *) data;
    SolData  * sol            = malloc(sizeof(SolData));
    sol->solution             = malloc(sizeof(int) * (pack->variables));

    QuickSort((Array) pack->items, CompareByValue, 0, pack->variables - 1);

    int capacity              = pack->capacity;
    int occupied              = 0;
    int acc_priority          = 0;
    sol->solution[0]          = floor(capacity / pack->items[0]->value);
    pack->items[0]->qty       = sol->solution[0];
    occupied                 += sol->solution[0] * pack->items[0]->value;
    acc_priority             += sol->solution[0] * pack->items[0]->priority;
    for (int i = 1; i < pack->variables; i++)
    {
        sol->solution[i]      = floor((capacity  - occupied)/ pack->items[i]->value);
        occupied             += sol->solution[i] * pack->items[i]->value;
        acc_priority         += sol->solution[i] * pack->items[i]->priority;
        pack->items[i]->qty   = sol->solution[i];
    }
    sol->occupied_space       = occupied;
    sol->accumulated_priority = acc_priority;
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    sol->execution_time       = elapsed;

    return (Solution) sol;
}

Solution MOP  (Problem data)
{
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    Backpack * pack           = (Backpack *) data;
    SolData  * sol            = malloc(sizeof(SolData));
    sol->solution             = malloc(sizeof(int) * (pack->variables));

    QuickSort((Array) pack->items, CompareByPriority, 0, pack->variables - 1);

    int capacity              = pack->capacity;
    int occupied              = 0;
    int acc_priority          = 0;
    sol->solution[0]          = floor(capacity / pack->items[0]->value);
    pack->items[0]->qty       = sol->solution[0];
    occupied                 += sol->solution[0] * pack->items[0]->value;
    acc_priority             += sol->solution[0] * pack->items[0]->priority;
    for (int i = 1; i < pack->variables; i++)
    {
        sol->solution[i]      = floor((capacity  - occupied)/ pack->items[i]->value);
        occupied             += sol->solution[i] * pack->items[i]->value;
        acc_priority         += sol->solution[i] * pack->items[i]->priority;
        pack->items[i]->qty   = sol->solution[i];
    }
    sol->occupied_space       = occupied;
    sol->accumulated_priority = acc_priority;
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    sol->execution_time       = elapsed;

    return (Solution) sol;
}

Solution MOPT (Problem data)
{
    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    Backpack * pack           = (Backpack *) data;
    SolData  * sol            = malloc(sizeof(SolData));
    sol->solution             = malloc(sizeof(int) * (pack->variables));

    for (int i = 0; i < pack->variables; i++)
        pack->items[i]->factor = (double) pack->items[i]->priority / pack->items[i]->value;
    
    QuickSort((Array) pack->items, CompareByFactor, 0, pack->variables - 1);

    int capacity              = pack->capacity;
    int occupied              = 0;
    int acc_priority          = 0;
    sol->solution[0]          = floor(capacity / pack->items[0]->value);
    pack->items[0]->qty       = sol->solution[0];
    occupied                 += sol->solution[0] * pack->items[0]->value;
    acc_priority             += sol->solution[0] * pack->items[0]->priority;
    for (int i = 1; i < pack->variables; i++)
    {
        sol->solution[i]      = floor((capacity  - occupied)/ pack->items[i]->value);
        occupied             += sol->solution[i] * pack->items[i]->value;
        acc_priority         += sol->solution[i] * pack->items[i]->priority;
        pack->items[i]->qty   = sol->solution[i];
    }

    sol->occupied_space       = occupied;
    sol->accumulated_priority = acc_priority;
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    sol->execution_time       = elapsed;

    return (Solution) sol;
}

















struct Variable {
    int weight;
    int value;
    double factor;
};
typedef struct Item {
    int quantity;
    struct Variable variable;
} item;

typedef void * Branch;
typedef item * BranchImpl;

Branch allocateBranch(int Static_Vars_Amount)
{
    BranchImpl branch = malloc(sizeof(item) * Static_Vars_Amount);
    return (Branch) branch;
}

int MaximizeItem(Branch branch, double c, int k)
{
    BranchImpl b = (BranchImpl) branch;
    int qtd;
    int occupied_space = 0;
    // Calcular o somatório dos espaços ocupados por todos os itens antes de K
    for (int i = 0; i <= k - 1; i++)
        occupied_space +=  b[i].variable.value * b[i].quantity;
    // Fórmula de x_k
    qtd = floor((c - occupied_space ) /
                (b[k].variable.value));
    return qtd;
}
int MaximizeBranch(Branch branch, double c, int VARS_AMOUNT, int InitialK)
{
    BranchImpl b = (BranchImpl) branch;
    int accumulated_priorities = 0;
    int i;
    for (i = 0; i < InitialK; i++)
        accumulated_priorities += b[i].quantity * b[i].variable.weight;
    for (i = InitialK; i < VARS_AMOUNT; i++)
    {
        b[i].quantity = MaximizeItem(b, c, i);
        accumulated_priorities += b[i].quantity * b[i].variable.weight;
    }
    return accumulated_priorities;
}

int AccumulatedPriorities (Branch branch, int VARS_AMOUNT)
{
    BranchImpl b = (BranchImpl) branch;
    int accumulated_priorities = 0;
    for (int i = 0; i < VARS_AMOUNT; i++)
        accumulated_priorities += b[i].variable.weight * b[i].quantity;
    return accumulated_priorities;
}

void writeBranchQty(Branch branch, int VARS_AMOUNT, FILE * logFile)
{
    BranchImpl b = (BranchImpl) branch;
    for (int i = 0; i < VARS_AMOUNT; i++)
        fprintf(logFile, "%3d ", b[i].quantity);
    fprintf(logFile, " - f = %d\n", AccumulatedPriorities(b, VARS_AMOUNT));
}

void writeDownLevel(Branch branch, int VARS_AMOUNT, int k, FILE * logFile)
{
    BranchImpl b = (BranchImpl) branch;
    for (int i = 0; i <= k; i++)
        fprintf(logFile, "%3d ", b[i].quantity);
    for (int i = k + 1; i < VARS_AMOUNT; i++)
        fprintf(logFile, "%3c ", '?');
    fprintf(logFile, "\n");
}

int findNonZeroItem(Branch branch, int VARS_AMOUNT)
{
    BranchImpl b = (BranchImpl) branch;
    int k;
    for (k = VARS_AMOUNT - 1; k > 0 && b[k].quantity <= 0; k--)
    {};
    return k;
}

int DownLevel(Branch branch, int VARS_AMOUNT)
{
    BranchImpl b = (BranchImpl) branch;
    int k = findNonZeroItem(b, VARS_AMOUNT);
    b[k].quantity--;
    return k;
}

void copyBranch(Branch _targetBranch, Branch _originBranch, int VARS_AMOUNT)
{
    BranchImpl targetBranch = (BranchImpl) _targetBranch;
    BranchImpl originBranch = (BranchImpl) _originBranch;
    for (int i = 0; i < VARS_AMOUNT; i++)
    {
        targetBranch[i].quantity = originBranch[i].quantity;
        targetBranch[i].variable = originBranch[i].variable;
    }
}

/*  [LaTeX] - CÁLCULOS MATEMÁTICOS UTILIZADOS

    \text{Quantidade em k dada por }x_k\text{ tal que:}
    \break
    x_k = \left \lfloor \frac{\sum_{i=1}^{k-1}l_ix_i}{l_k} \right \rfloor
    \break
    \text{Relacao entre a soma das pioridades }\bar f \text{ e o limitante } \bar F
    \break
    \bar{f} = \sum_{i=1}^{n}f_i\bar{x}_i \leq \sum_{i=1}^{k-1}p_i\hat{x}_i + p_k(\hat{x}_k-1)+\frac{p_{k+1}}{l_{k+1}}[c-\sum_{i=1}^{k-1}l_i\hat{x}_i + l_k(\hat{x}_k-1)] = \bar{F}
    \break
    \text{O limitante }L\text{ calculado como}
    \break
    L = \left \lfloor \bar{F} + 1\right \rfloor = \left \lfloor {\sum_{i=1}^{k-1}p_i\hat{x}_i + p_k(\hat{x}_k-1)+\frac{p_{k+1}}{l_{k+1}}[c-\sum_{i=1}^{k-1}l_i\hat{x}_i + l_k(\hat{x}_k-1)]} + 1\right \rfloor
*/

int CalculateBound(Branch branch, double c, int k)
{
    BranchImpl b = (BranchImpl) branch;
    int accumulated_weights = 0;
    int occupied_space      = 0;
    
    int p_k = b[k].variable.weight;
    int l_k = b[k].variable.value;
    int hatx_k = b[k].quantity + 1;
    double nextFactor = b[k+1].variable.factor;

    for (int i = 0; i <= k-1; i++)
    {
        accumulated_weights += b[i].variable.weight * b[i].quantity;
        occupied_space      += b[i].variable.value  * b[i].quantity;
    }

    double barF = accumulated_weights + p_k * (hatx_k - 1) + nextFactor * (c - occupied_space - l_k * (hatx_k - 1));
    int L = floor(barF + 1);

    return L;
}

int writeGraphBranch(FILE * dotFile, Branch branch, int * rowsEachLevelK, int initialK, int finalK)
{
    BranchImpl b = (BranchImpl) branch;
    char style[7] = "dotted";
    if (initialK != finalK)
        style[0] = '\0';
    
    for (int k = initialK; k <= finalK; k++)
    {
        rowsEachLevelK[k]++;
        fprintf(dotFile, "\t%d.%d [label=\"%d\" style=\"%s\"]\n", k, rowsEachLevelK[k], b[k].quantity, style);
    }

    if (initialK == 0)
        fprintf(dotFile, "\troot -- %d.%d\n", initialK, rowsEachLevelK[initialK]);
    else
    {
        // Ligar o pai ao ramo
        fprintf(dotFile, "\t%d.%d -- %d.%d\n", initialK-1, rowsEachLevelK[initialK-1], initialK, rowsEachLevelK[initialK]);
    }

    for (int k = initialK; k < finalK; k++)
    {
        fprintf(dotFile, "\t%d.%d -- %d.%d\n", k, rowsEachLevelK[k], k+1, rowsEachLevelK[k+1]);
    }

    fprintf(dotFile, "\n");
    return 0;
}

Solution BranchAndBound (Problem data, char * output_log_dir, char * problem_name)
{
    int i, j, k, Bound;
    int VARS_TOTAL;
    int capacity, greater, f;
    char * dotname;
    char * logname;



    struct timespec start, end;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    Backpack * pack           = (Backpack *) data;
    SolData  * sol            = malloc(sizeof(SolData));
    sol->solution             = malloc(sizeof(int) * (pack->variables));
    for (int i = 0; i < pack->variables; i++)
        pack->items[i]->factor = (double) pack->items[i]->priority / pack->items[i]->value;
    
    QuickSort((Array) pack->items, CompareByFactor, 0, pack->variables - 1);
    VARS_TOTAL = pack->variables;
    capacity = pack->capacity;


    // Escrever cabeçalho básico do grafo
    // dotname = malloc(sizeof(char) * (strlen(output_log_dir) + 1 + strlen(problem_name) + strlen("-BB-Graph.dot") + 1));
    // logname = malloc(sizeof(char) * (strlen(output_log_dir) + 1 + strlen(problem_name) + strlen("-BB-Log.dot"  ) + 1));
    // sprintf(dotname, "%s/%s-BB-Graph.dot", output_log_dir, problem_name);
    // sprintf(logname, "%s/%s-BB-Log.txt", output_log_dir, problem_name);
    // FILE * dotFile = fopen(dotname, "w");
    // FILE * logFile = fopen(logname, "w");
    // fprintf(dotFile, "graph G {\n");
    // fprintf(dotFile, "\trankdir=\"LR\"\n");
    // fprintf(dotFile, "\tnode [shape=circle]\n");
    // fprintf(dotFile, "\troot [label=\"root\" rankdir=\"LR\"]\n");
    // *
    // *
    // *
    BranchImpl current  = allocateBranch(VARS_TOTAL);
    BranchImpl SolBranch = allocateBranch(VARS_TOTAL);

    // Leitura do aquivo e armazenamento nas variáveis
    int * rowsEachLevelK = malloc(sizeof(int) * VARS_TOTAL);
    for (i = 0; i < VARS_TOTAL; i++)
    {
        current[i].quantity = 0;    
        current[i].variable.weight = pack->items[i]->priority;
        current[i].variable.value  = pack->items[i]->value;
        current[i].variable.factor = pack->items[i]->factor;
        rowsEachLevelK[i] = 0;
    }
    
    k = 0;
    MaximizeBranch(current, capacity, VARS_TOTAL, k);
    copyBranch(SolBranch, current, VARS_TOTAL);
    greater = AccumulatedPriorities(current, VARS_TOTAL);
    //fprintf(logFile, "Greater: %.2d\n", greater);
    // writeBranchQty(current, VARS_TOTAL, logFile);

    // printf("[0] = %d | [1] = %d | k = %d\n", current[0].quantity, current[1].quantity, k);

    //writeGraphBranch(dotFile, current, rowsEachLevelK, 0, VARS_TOTAL - 1);
    //fprintf(logFile, ">> descendo nivel <<\n");
    k = DownLevel(current, VARS_TOTAL);
    // writeDownLevel(current, VARS_TOTAL, k, logFile);
    //int unknown = 1;
    // printf("[0] = %d | [1] = %d | k = %d\n", current[0].quantity, current[1].quantity, k);
    while (current[0].quantity >= 0)
    {
        /* CALCULAR LIMITANTE
            - Se o limitante for estritamente MAIOR: continue as ramificações;
            - Senão: Desça mais um nível
        */
        // printf("non-0 = %d\n", findNonZeroItem(current, VARS_TOTAL));
        if (current[k].quantity >= 0 && k < VARS_TOTAL - 1)
        {
            //fprintf(logFile, "> calculando limitante em k = [%d] <\n", k);
            Bound = CalculateBound(current, capacity, k);
            //fprintf(logFile, "<limitante: %d>\n", Bound);
            if (Bound > greater)
            {
                //fprintf(logFile, ">>> limitante [%d] > solucao [%d] <<\n", Bound, greater);
                //fprintf(logFile, "::: calculando ramo :::\n");
                f = MaximizeBranch(current, capacity, VARS_TOTAL, k + 1);
                // writeBranchQty(current, VARS_TOTAL, logFile);
                //unknown = 1;

                if (f > greater)
                {
                    greater = f;
                    copyBranch(SolBranch, current, VARS_TOTAL);
                }
            }
        }
        //writeGraphBranch(dotFile, current, rowsEachLevelK, k, VARS_TOTAL - unknown);
        //fprintf(logFile, ">> descendo nivel <<\n");
        
        // printf("[0] = %d | [1] = %d | k = %d\n", current[0].quantity, current[1].quantity, k);
        k = DownLevel(current, VARS_TOTAL);
        // writeDownLevel(current, VARS_TOTAL, k, logFile);
        //unknown = VARS_TOTAL - k;
    }
    //fprintf(logFile, ">>> FIM DAS RAMIFICAOES <<<\n\n");
    //fprintf(logFile, "SOLUCAO:\n");
    // writeBranchQty(SolBranch, VARS_TOTAL, logFile);

    // fprintf(dotFile, "}");
    // fclose(dotFile);
    // fclose(logFile);

    int occupied_space = 0;
    int acc_priority   = 0;
    for(i = 0; i < VARS_TOTAL; i++)
    {
        sol->solution[i]    = SolBranch[i].quantity;
        pack->items[i]->qty = SolBranch[i].quantity;
        occupied_space     += SolBranch[i].quantity * pack->items[i]->value;
        acc_priority       += SolBranch[i].quantity * pack->items[i]->priority;
    }

    sol->accumulated_priority = acc_priority;
    sol->occupied_space       = occupied_space;
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = (end.tv_sec - start.tv_sec);
    elapsed += (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    sol->execution_time       = elapsed;
    return (Solution) sol;
}


// void main()
// {
//     Problem pack = ReadProblem("/mnt/c/Users/Gabriel/OneDrive/Codes/#Repositories/Mochila/CLASSES/G2/problem-G2-006.txt");
//     Solution Sol = MOPT(pack);
//     //sprintf(outpathfile, "%s/%s-MOP.txt", outpath, base);
//     ExportSolution("MOPT-Sol.txt", pack, Sol);
//     RemoveSolution(Sol);

//     Sol = BranchAndBound(pack, ".", "G2-006");
//     //sprintf(outpathfile, "%s/%s-BB.txt", outpath, base);
//     ExportSolution("BB-Sol.txt", pack, Sol);
//     RemoveSolution(Sol);
// }