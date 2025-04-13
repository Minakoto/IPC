#include "head.h"


int main(int argc, char* argv[]) {
    if(argc != 1) {
        printf("Not enough arguments\n");
        exit(EXIT_FAILURE);
    }
    float num = atof(argv[0]);
    printf("Number squared: %.3f\n", num * num);
    exit(EXIT_SUCCESS);
}