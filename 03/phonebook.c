#include "phonebook.h"
/*ERROR CODES -1 Init -2 Add*/

Entry init_entry(char* first, char* last, short* cur) {
    Entry entry;
    entry.id = *cur;
    char res = init_person(&entry.person, first, last);
    zero_address(&(entry.adress));
    zero_job(&(entry.job));
    zero_contacts(&(entry.contacts));
    return entry;
}
char init_person(Person* person, char* first, char* last ) {
    char res = 1;
    if (strlen(first) < ENT_SIZE || strlen(last) < ENT_SIZE) {
        strncpy(person->firstname, first, ENT_SIZE);
        strncpy(person->lastname, last, ENT_SIZE);
        for(int i = 0; i < ENT_SIZE; i++)
            *(person->middlename + i) = '0';
    } else
        res = -1;
    return res;
}
void zero_person(Person* person) {
    for(int i = 0; i < ENT_SIZE; i++) {
        *(person->firstname + i) = '0';
        *(person->lastname + i) = '0';
        *(person->middlename + i) = '0';
    }
}
void zero_address(Address* address) {
    address->adr = 0;
    address->apt = 0;
    for(int i = 0; i < ADR_SIZE; i++) {
        *(address->city + i) = '0';
        *(address->street + i) = '0';
    }
}
void zero_job(Job* job) {
    zero_address(&job->office);
    for(int i = 0; i < ENT_SIZE; i++)
        *(job->position + i) = '0';
    job->salary = 0;
}
void zero_contacts(Contacts* contacts) {
    for(int i = 0; i < EMAIL_SIZE; i++) 
        *(contacts->email + i) = '0';
    for(int i = 0; i < PHONE_SIZE; i++) 
        *(contacts->phone + i) = '0';
    for(int i = 0; i < TG_SIZE; i++) 
        *(contacts->social_t + i) = '0';
    for(int i = 0; i < VK_SIZE; i++) 
        *(contacts->social_vk + i) = '0';
}
void zero_everything(Entry* entry) {
    zero_person(&entry->person);
    zero_address(&entry->adress);
    zero_job(&entry->job);
    zero_contacts(&entry->contacts);
    entry->id = 0;
}
char add_address_char(Address* adr, char cmd, char* string) {
    char res = 1;
    if(cmd == 1) strncpy(adr->city, string,  ADR_SIZE);
    else if (cmd == 2) strncpy(adr->street, string,  ADR_SIZE);
    else res = -2;
    return res;
}
char add_address_short(Address* adr, char cmd, short val) {
    char res = 1;
    if(cmd == 3) adr->adr = val;
    else if(cmd == 4) adr->apt = val;
    else res = -2;
    return res;
}
char add_job_char(Job* job, char cmd, char* string) {
    char res = 1;
    if(cmd == 1 || cmd == 2) res = add_address_char(&job->office, cmd, string);
    else if (cmd == 3) strncpy(job->position, string, ENT_SIZE);
    else res = -2;
    return res;
}
char add_job_short(Job* job, char cmd, short val) {
    char res = 1;
    if(cmd == 4 | cmd == 5) res = add_address_short(&job->office, cmd - 1, val);
    else if(cmd == 6) job->salary = val;
    else res = -2;
    return res;
}
char add_contacts(Contacts* cnt, char cmd, char* string) {
    char res = 1;
    if(cmd == 1) strncpy(cnt->email, string, EMAIL_SIZE);
    else if(cmd == 2) strncpy(cnt->phone, string, PHONE_SIZE);
    else if(cmd == 3) strncpy(cnt->social_t, string, TG_SIZE);
    else if(cmd == 4) strncpy(cnt->social_vk, string, VK_SIZE);
    else res = -2;
    return res;
}

char edit_person(Person* person, char cmd, char* string) {
    char res = 1;
    if(cmd == 1) strncpy(person->firstname, string, ENT_SIZE);
    else if(cmd == 2) strncpy(person->lastname, string, ENT_SIZE);
    else if(cmd == 3) strncpy(person->middlename, string, ENT_SIZE);
    else res = -2;
}

// short find(Entry * book, short ide, short cur) {
//     short i = 0;
//     while(i <= cur && (book + i)->id != ide) {
//         i++;
//     }
//     return (i > cur) ? -1 : i;
// }
// char del(Entry* book, short *cur, short id) {
//     char res = 1;
//     short wrk = find(book, id, *cur);
//     zero_everything(book+ wrk);
//     if(wrk != -1) {
//         for(int i = wrk ; i < *cur - 1; i++) {
//             *(book + i) = *(book + i + 1); 
//         }
//         zero_everything(book + *cur - 1);
//         *cur -= 1;
//     }
//     else res = -3;
//     return res;
// }