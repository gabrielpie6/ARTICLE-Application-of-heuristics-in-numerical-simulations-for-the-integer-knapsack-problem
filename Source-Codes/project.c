#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include "heuristics.h"

int main() {
    DIR *dir;
    struct dirent *ent;
    char path[200];
    char outpath[1000];
    char outpathfile[1000];
    char *classes[] = {"P1", "P2", "M1", "M2", "G1", "G2"};
    char *base;
    int n_classes = sizeof(classes)/sizeof(classes[0]);

    Problem pack;
    Solution Sol;

    for (int i = 0; i < n_classes; i++) {
        sprintf(path, "./CLASSES/%s", classes[i]);
        sprintf(outpath, "./OUTPUT/%s", classes[i]);
        //mkdir(outpath, 0777);

        if ((dir = opendir(path)) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                    char filepath[1000];
                    sprintf(filepath, "%s/%s", path, ent->d_name);
                    base = strtok(ent->d_name, ".");
                    sprintf(outpath, "./OUTPUT/%s/%s", classes[i], base);
                    //mkdir(outpath, 0777);

                    // Quero calcular apenas no diretório P1
                    if (strcmp(classes[i], "G2") == 0)
                    {
                        printf("Calculating for: %s\n", filepath);
                        // CÁLCULO DAS HEURÍSTICAS
                        pack = ReadProblem(filepath);
                        Sol = MOT(pack);
                        sprintf(outpathfile, "%s/%s-MOT.txt", outpath, base);
                        ExportSolution(outpathfile, pack, Sol);
                        RemoveSolution(Sol);

                        Sol = MOP(pack);
                        sprintf(outpathfile, "%s/%s-MOP.txt", outpath, base);
                        ExportSolution(outpathfile, pack, Sol);
                        RemoveSolution(Sol);

                        Sol = MOPT(pack);
                        sprintf(outpathfile, "%s/%s-MOPT.txt", outpath, base);
                        ExportSolution(outpathfile, pack, Sol);
                        RemoveSolution(Sol);

                        sprintf(outpathfile, "%s/%s-BB.txt", outpath, base);
                        Sol = BranchAndBound(pack, outpath, base);
                        ExportSolution(outpathfile, pack, Sol);
                        RemoveSolution(Sol);
                    }
                }
            }
            closedir(dir);
        } else {
            perror("Não foi possível abrir o diretório");
            return EXIT_FAILURE;
        }
    }

    return 0;
}