#include "phonebook.h"
#include "list.h"

List* menu_l1(List*, short*, char);
List* menu_l2(List*, short*, char);
void print_options();
void print_types();
void print_book(List*);
void print_person(Person*);
void print_id(List*);
char handle_error(char);
int is_empty(char*);
Entry ReadEntry(int fd, short* cur, int* res);
void WriteElem(int fd, short cur, List* head);


int main() {
    List* head = NULL;
    short current = 0;
    char big_cmd = 0;
    print_options();
    int fd = open("input.txt", O_RDONLY);
    int res = 1;
    int i = 0;
    Entry* tm;
    while(fd != -1 && res) {
        tm = malloc(sizeof(Entry));
        *tm = ReadEntry(fd, &current, &res);
        if(!res) break;
        head = AddElem(*tm, head);
        free(tm);
    }
    if(fd != -1)
        close(fd);
    while(big_cmd != 6) {
        if(scanf("%hhd", &big_cmd))
            head = menu_l1(head, &current, big_cmd);
    }
    fd = open("input.txt", O_CREAT|O_WRONLY|O_TRUNC, 0777);
    WriteElem(fd, current, head);
    close(fd);
    return 0;
}
void WriteElem(int fd, short cur, List* head) {
    List* tmp = head;
    for(int i = 0; i < cur; i++) {
        write(fd, &tmp->data.id, sizeof(short));
        write(fd, tmp->data.person.firstname, ENT_SIZE * sizeof(char));
        write(fd, tmp->data.person.lastname, ENT_SIZE * sizeof(char));
        write(fd, tmp->data.adress.city, ADR_SIZE * sizeof(char));
        write(fd, tmp->data.job.position, ENT_SIZE * sizeof(char));
        write(fd, tmp->data.contacts.email, EMAIL_SIZE * sizeof(char));
        tmp = tmp->next;
    }
}
Entry ReadEntry(int fd, short* cur, int* res) {
    Entry t;
    int cm = 1;
    char* str1, *str2;
    short tmpid = 0;
    str1 = malloc(ENT_SIZE*sizeof(char));
    str2 = malloc(ENT_SIZE*sizeof(char));
    *res = read(fd, &tmpid, sizeof(short));
    if(!*res) return t;
    *res = read(fd, str1, ENT_SIZE * sizeof(char));
    *res = read(fd, str2, ENT_SIZE * sizeof(char));
    t = init_entry(str1, str2, cur);
    free(str1);
    free(str2);
    str1 = malloc(ADR_SIZE*sizeof(char));
    *res = read(fd, str1, ADR_SIZE * sizeof(char));
    *res = add_address_char(&t.adress, 1, str1);
    free(str1);
    str1 = malloc(ENT_SIZE*sizeof(char));
    *res = read(fd, str1, ENT_SIZE * sizeof(char));
    *res = add_job_char(&t.job, 3, str1);
    free(str1);
    str1 = malloc(EMAIL_SIZE*sizeof(char));
    *res = read(fd, str1, EMAIL_SIZE * sizeof(char));
    *res = add_contacts(&t.contacts, 1, str1);
    free(str1);
    *cur += 1;
    return t;
}


List* menu_l1(List* head, short* cur, char cmd) {
    List* tmp = head;
    char res = 1;
    short id;
    char* string1;
    char * string2;
    printf("\n");
    switch(cmd) {
        case 1:
            Entry t;
            string1 = (char*)malloc(ENT_SIZE * sizeof(char));
            string2 = (char*)malloc(ENT_SIZE * sizeof(char));
            printf("Input first and last names: ");
            if(scanf("%s", string1) && scanf("%s", string2)) {
                t = init_entry(string1, string2, cur);
                tmp = AddElem(t, tmp);
            }
            else res = -3;
            if(res == 1) *cur += 1;
            free(string1);
            free(string2);
            break;
        case 2: //edit
            tmp = menu_l2(tmp, cur, cmd);
            break;
        case 3:
            printf("Which ID? ");
            scanf("%hd", &id);
            tmp = DelElem(id, tmp);
            break;
        case 4:
            print_book(tmp);
            break;
        case 5:
            print_id(tmp);
            break;
        case 6:
            res = 6;
        default:
    }
    print_options();
    return tmp;
}
List* menu_l2(List* book, short* cur, char cmd) {
    List* tmp = book;
    char res = 1;
    char cmd2;
    short val;
    char* string;
    short wrk;
    printf("Which ID? ");
    scanf("%hd", &wrk);
    print_types();
    while(tmp->data.id != wrk) tmp = tmp->next;
    if(wrk != -1 && scanf("%hhd", &cmd2)) {
        switch (cmd2) {
        case 1:
            if(cmd == 2) {
                printf("Choose field:\n1.First name\n2. Last name\n3. Middle name\n");
                scanf("%hhd", &cmd2);
                printf("New field: ");
                string = malloc(ENT_SIZE*sizeof(char));
                scanf("%s", string);
                res = edit_person(&(tmp->data.person), cmd2, string);
                free(string);
            }
            else if(cmd == 3)
                zero_person(&(tmp->data.person));
            break;
        case 2:
            printf("%hd", wrk);
            if(cmd == 2) {
                printf("Choose field:\n1. City\n2. Street\n3. Home number\n4. Apt number\n");
                scanf("%hhd", &cmd2);
                printf("New field: ");
                if(cmd2 == 1 || cmd2 == 2) {
                    string = malloc(ADR_SIZE*sizeof(char));
                    scanf("%s", string);
                    res = add_address_char(&(tmp->data.adress), cmd2, string);
                    free(string);
                }
                else if(cmd2 == 3 || cmd2 == 4) {
                    scanf("%hd", &val);
                    res = add_address_short(&(tmp->data.adress), cmd2, val);
                }
            }
            else if(cmd == 3)
                zero_address(&(tmp->data.adress));
            break;
        case 3:
            if(cmd == 2) {
                printf("Choose field:\n1. Office city\n2.Office street\n3. Position\n");
                printf("4. Office number\n5. Office room number\n6. Salary\n");
                scanf("%hhd", &cmd2);
                printf("New field: ");
                if(cmd2 == 1 || cmd2 == 2 || cmd2 == 3) {
                    if(cmd == 3)
                        string = malloc(ENT_SIZE*sizeof(char));
                    else
                        string = malloc(ADR_SIZE*sizeof(char));
                    scanf("%s", string);
                    res = add_job_char(&(tmp->data.job), cmd2, string);
                    free(string);
                }
                else if(cmd2 == 4 || cmd2 == 5 || cmd2 == 6) {
                    scanf("%hd", &val);
                    res = add_job_short(&(tmp->data.job), cmd2, val);
                }
            }
            else if(cmd == 3)
                zero_job(&(tmp->data.job));
            break;
        case 4:
            if(cmd == 2) {
                printf("Choose field:\n1. Email\n2.Phone number\n3. Telegram\n4. VK\n");
                scanf("%hhd", &cmd2);
                printf("New field: ");
                if(cmd <= 4) {
                    if(cmd2 == 1) string = malloc(EMAIL_SIZE*sizeof(char));
                    else if(cmd2 == 2) string = malloc(PHONE_SIZE*sizeof(char));
                    else if(cmd2 == 3) string = malloc(TG_SIZE*sizeof(char));
                    else string = malloc(VK_SIZE*sizeof(char));
                    scanf("%s", string);
                    res = add_contacts(&(tmp->data.contacts), cmd2, string);
                    free(string);
                }
            }
            else if(cmd == 3)
                zero_contacts(&(tmp->data.contacts));
            break;
        case 5:
        default:
        }
    }
    return tmp;
}
void print_options() {
    printf("\nMENU: \n");
    printf("1. Initiate entry\n");
    printf("2. Edit entry\n");
    printf("3. Delete entry\n");
    printf("4. View phonebook\n");
    printf("5. List IDs\n");
    printf("6. Exit\n");
    printf("Input command: ");
}
void print_types() {
    printf("\nCategory:\n");
    printf("1. Person\n");
    printf("2. Address\n");
    printf("3. Job\n");
    printf("4. Contacts\n");
    printf("5. Back\n");
}

int is_empty(char* string) {
    return (string[0] == '0') ? 1: 0;
}

void print_book(List* head) {
    List* tmp = head;
    // PrintList(tmp);
    if(tmp == NULL) {
        printf("Phonebook empty!\n");
        return;
    }
    PrintList(head);

    printf("%-5s|%-20s|%-20s|%-20s|%-20s|%-20s|\n", "ID", "First name", "Last name", "Address", "Job", "Contacts");
    for(int i =0; i < 5; i++) printf("-");
    printf("|");
    for(int i =0; i < 20; i++) printf("-");
    printf("|");
    for(int i =0; i < 20; i++) printf("-");
    printf("|");
    for(int i =0; i < 20; i++) printf("-");
    printf("|");
    for(int i =0; i < 20; i++) printf("-");
    printf("|");
    for(int i =0; i < 20; i++) printf("-");
    printf("|");
    printf("\n");
    while(tmp != NULL) {
        printf("%-5d|%-20s|%-20s|", tmp->data.id, tmp->data.person.firstname, tmp->data.person.lastname);
        printf("%-20s|", !is_empty(tmp->data.adress.city) ? tmp->data.adress.city : "0");
        printf("%-20s|", !is_empty(tmp->data.job.position) ? tmp->data.job.position : "0");
        printf("%-20s|", !is_empty(tmp->data.contacts.email) ? tmp->data.contacts.email : "0");
        printf("\n");
        tmp = tmp->next;
    }
    
}
void print_id(List* head) {
    List* tmp = head;
    printf("Avalalbe IDs: ");
    if(head == NULL) {
        printf("No Ids\n");
        return;
    }
    while(tmp->next != NULL) {
        printf("%d, ", tmp->data.id);
        tmp = tmp->next;
    }
    printf("%d\n", tmp->data.id);
}