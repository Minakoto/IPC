#ifndef PHONE
#define PHONE
#define ENT_SIZE 20
#define ADR_SIZE 20
#define EMAIL_SIZE 25
#define TG_SIZE 20
#define VK_SIZE 30
#define PHONE_SIZE 12
#define SIZE 50

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>


typedef struct Person {
    char lastname[ENT_SIZE];
    char firstname[ENT_SIZE];
    char middlename[ENT_SIZE];
} Person;

typedef struct Address {
    char city[ADR_SIZE];
    char street[ADR_SIZE];
    short adr;
    short apt;
} Address;

typedef struct Job {
    Address office;
    char position[ENT_SIZE];
    short salary;
} Job;

typedef  struct Contacts {
    char email[EMAIL_SIZE];
    char social_t[TG_SIZE];
    char social_vk[VK_SIZE];
    char phone[PHONE_SIZE];
} Contacts;

typedef struct Entry {
    short id;
    Person person;
    Address adress;
    Job job;
    Contacts contacts;
} Entry;

Entry init_entry(char* first, char* last, short* cur);
char init_person(Person*, char*, char*);
void zero_person(Person*);
void zero_address(Address*);
void zero_job(Job*);
void zero_contacts(Contacts*);
void zero_everything(Entry*);

char add_address_char(Address*, char, char*);
char add_address_short(Address*, char, short);
char add_job_char(Job*, char, char*);
char add_job_short(Job*, char, short);
char add_contacts(Contacts*, char, char*);

char edit_person(Person*, char, char*);

short find(Entry*, short, short);
char del(Entry*, short*, short);
#endif