#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

int main() {
    DIR* d;
    struct dirent* f;
    char func[10];
    char** args;

    do {
        printf("Available functions: ");
        d = opendir("exec");
        if(d != NULL) {
            f = readdir(d);
            while(f != NULL) {
                if(*f->d_name != 46)
                    printf("%s ", f->d_name);
                f = readdir(d);
            }
            free(f);
            closedir(d);
        } else {
            printf("Unable to open directory\n");
            exit(EXIT_FAILURE);
        }
        printf("\nInput name and arguments of function you want to call (-1 to exit)");
        printf("\n(Notice: the first argument after name is read as the number of arguments): ");
        scanf("%s", func);
        if(strcmp(func, "q") == 0) continue;
        int s;
        scanf("%d", &s);
        if(s > 0)
            args = malloc((s + 1) * sizeof(char*));
        else {
            printf("Unable to get arguments\n");
            exit(EXIT_FAILURE);
        }
        char tmp[16];
        for(int i = 0; i < s; i++) {
            scanf("%s", tmp);
            args[i] = malloc(strlen(tmp) * sizeof(char));
            strcpy(args[i], tmp);
        }
        args[s] = NULL;
        char* nfunc = malloc((strlen(func) + 6)*sizeof(char));
        strcpy(nfunc, "exec/");
        nfunc = strncat(nfunc, func, strlen(func));
        pid_t p;
        switch(p = fork()) {
            case -1:
                perror("fork");
                exit(EXIT_FAILURE);
            case 0:
                execvp(nfunc, args);
                perror("external func");
                exit(EXIT_FAILURE);
            default:
                waitpid(p, NULL, 0);
                break;
        }
        
        for(int i = 0; i < s; i++) free(args[i]);
        free(args);
        free(nfunc);
} while(strcmp(func, "q") != 0);
}