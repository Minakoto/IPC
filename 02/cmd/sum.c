#include "head.h"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Wrong input\n");
        exit(EXIT_FAILURE);
    }
    float* nums = malloc((argc) * sizeof(float));
    float res = 0;
    for(int i = 0; i < argc; i++) nums[i] = atof(argv[i]);
    for(int i = 0; i < argc; i++) res += nums[i];
    printf("Sum: %.3f\n", res);
    free(nums);
    exit(EXIT_SUCCESS);
}