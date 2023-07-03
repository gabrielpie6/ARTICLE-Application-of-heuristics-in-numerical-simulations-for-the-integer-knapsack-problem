#ifndef HEURISTICS_H
#define HEURISTICS_H

typedef void * Problem;
typedef void * Solution;

Problem ReadProblem (char * filename);
void ExportSolution (char * filename, Problem data, Solution solution);
void RemoveSolution (Solution solution);

Solution MOT  (Problem data);
Solution MOP  (Problem data);
Solution MOPT (Problem data);
Solution BranchAndBound (Problem data, char * output_log_dir, char * log_files_name);

#endif