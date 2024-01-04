#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SENDER_LENGTH 20
#define MAX_TIMESTAMP_LENGTH 20
#define MAX_CONTENT_LENGTH 200

// SMS structure
struct SMS {
    char sender[MAX_SENDER_LENGTH];
    char timestamp[MAX_TIMESTAMP_LENGTH];
    char content[MAX_CONTENT_LENGTH];
};

// Node structure for linked list
struct Node {
    struct SMS data;
    struct Node* next;
};

// Linked list structure
struct LinkedList {
    struct Node* head;
};

// Function to create a new node
struct Node* createNode(const struct SMS* sms) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode != NULL) {
        newNode->data = *sms;
        newNode->next = NULL;
    }
    return newNode;
}

// Function to insert a node at the end of the linked list
void insertNode(struct LinkedList* list, const struct SMS* sms) {
    struct Node* newNode = createNode(sms);
    if (newNode == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    if (list->head == NULL) {
        list->head = newNode;
    } else {
        struct Node* current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

// Function to check if an SMS is spam
int isSpam(const struct SMS* sms) {
    // Implement your spam filtering logic here
    // For simplicity, this example considers any message containing "spam" as spam
    const char* keyword = "spam";
    return (strstr(sms->content, keyword) != NULL);
}

// Function to filter spam messages and create a new linked list
struct LinkedList* filterSpam(struct LinkedList* allMessages) {
    struct LinkedList* filteredMessages = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    if (filteredMessages == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    filteredMessages->head = NULL;

    struct Node* current = allMessages->head;
    while (current != NULL) {
        if (!isSpam(&current->data)) {
            insertNode(filteredMessages, &current->data);
        }
        current = current->next;
    }

    return filteredMessages;
}

// Function to display SMS messages
void displayMessages(struct LinkedList* messages) {
    struct Node* current = messages->head;
    while (current != NULL) {
        printf("Sender: %s\nTimestamp: %s\nContent: %s\n\n", current->data.sender, current->data.timestamp, current->data.content);
        current = current->next;
    }
}

// Function to free memory used by the linked list
void freeLinkedList(struct LinkedList* list) {
    struct Node* current = list->head;
    struct Node* next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->head = NULL;
}

// Function to read SMS messages from a file and populate a linked list
struct LinkedList* readMessagesFromFile(const char* filename) {
    struct LinkedList* allMessages = (struct LinkedList*)malloc(sizeof(struct LinkedList));
    if (allMessages == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    allMessages->head = NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    struct SMS sms;
    while (fscanf(file, "%s %s %[^\n]", sms.sender, sms.timestamp, sms.content) == 3) {
        insertNode(allMessages, &sms);
    }

    fclose(file);
    return allMessages;
}

int main() {
    const char* filename = "sms_messages.txt";

    // Step 1: Read SMS messages from a file
    struct LinkedList* allMessages = readMessagesFromFile(filename);

    // Step 2: Filter out spam messages
    struct LinkedList* filteredMessages = filterSpam(allMessages);

    // Step 3: Display the filtered messages
    displayMessages(filteredMessages);

    // Step 4: Clean up memory
    freeLinkedList(allMessages);
    freeLinkedList(filteredMessages);

    return 0;
}
