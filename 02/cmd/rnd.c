#include "head.h"

int main(int argc, char* argv[]) {
    srand(time(NULL));
    int num = 1;
    if(argc != 1) exit(EXIT_FAILURE);
    num = atoi(argv[0]);
    printf("Random number, enjoy):%d\n", rand() % num);
    exit(EXIT_SUCCESS);
}