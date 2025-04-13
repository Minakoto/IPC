#include "list.h"

List* AddElem(Entry value, List* head) {
    List* newElem;
    List* tmp = head;
    newElem = malloc(sizeof(List));
    newElem->data = value;
    if(NULL == head) {
        newElem->next = NULL;
        newElem->prev = NULL;
        head = newElem;
        return head;
    }
    if(newElem->data.person.lastname[0] < head->data.person.lastname[0]) {
        newElem->next = head;
        newElem->prev = NULL;
        head->prev = newElem;
        return newElem;
    }
    while(tmp->next != NULL) {
        if(newElem->data.person.lastname[0] < tmp->next->data.person.lastname[0]) {
            newElem->next = tmp->next;
            newElem->prev = tmp;
            tmp->next->prev = newElem;
            tmp->next = newElem;
            break;
        }
        else tmp = tmp->next;
    }
    tmp->next = newElem;
    newElem->prev = tmp;
    newElem->next = NULL;
    return head;
}
List* DelElem(short value, List* head) {
    List* tmp = head;
    if(head == NULL) {
        printf("Nothing to delete\n");
        return head;
    }
    if(head->data.id == value) {
        tmp = head;
        head = head->next;
        free(tmp);
        return head;
    }
    do {
        if(tmp->data.id == value) {
            if(tmp->next != NULL) tmp->next->prev = tmp->prev;
            tmp->prev->next = tmp->next;
            if(tmp == head) head = head->next;
            free(tmp);
            return head;
        }
        tmp = tmp->next;
    }  while(tmp != NULL);
    printf("No element found for %d\n", value);
    return head;
}
List* DeleteList(List* head) {
    List* tmp = head;
    List* del;
    if(head == NULL) {
        return NULL;
    }
    while(tmp != NULL) {
        del = tmp;
        tmp = tmp->next;
        free(del);
    }
    return NULL;
}

void PrintList(List* head) {
    List* tmp = head;
    if(head == NULL) {
        printf("Try filling the list\n");
        return;
    }
    while(tmp != NULL) {
        printf("%p %d",tmp->next, tmp->data.id);
        tmp = tmp->next;
        if(tmp != NULL) printf(" <-> ");
    }
    printf("\n");
}