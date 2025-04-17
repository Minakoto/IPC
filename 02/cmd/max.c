#include "head.h"

int main(int argc, char* argv[]) {
    printf("argc%d", argc);
    if(argc < 2) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    float* nums = malloc((argc) * sizeof(float));
    float res = 0;
    for(int i = 0; i < argc; i++) nums[i] = atof(argv[i]);
    res = nums[0];
    for(int i = 1; i < argc; i++) {
        if(res < nums[i]) res = nums[i];
    }
    printf("Max: %.3f\n", res);
    free(nums);
    exit(EXIT_SUCCESS);
}