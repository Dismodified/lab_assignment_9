#include <stdio.h>
#include <stdlib.h>

#define HASH_SIZE 10

// RecordType
struct RecordType {
    int id;
    char name;
    int order; 
};

// Structure to represent an element stored in the hash table
struct HashElement {
    struct RecordType data;
    struct HashElement* next;
};

// Fill out this structure
struct HashType {
    struct HashElement* table[HASH_SIZE];
};

// Compute the hash function
int hash(int x) {
    return x % HASH_SIZE;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData) {
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile) {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i) {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }
    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz) {
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i) {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Insert an element into the hash table
void insertIntoHash(struct HashType* hashTable, struct RecordType record) {
    int index = hash(record.id);
    struct HashElement* newElement = (struct HashElement*)malloc(sizeof(struct HashElement));
    newElement->data = record;
    newElement->next = hashTable->table[index];
    hashTable->table[index] = newElement;
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz) {
    int i;

    for (i=0; i<hashSz; ++i) {
        struct HashElement* current = pHashArray->table[i];
        if (current != NULL) {
            printf("Index %d -> ", i);
            while (current != NULL) {
                printf("%d %c %d -> ", current->data.id, current->data.name, current->data.order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

int main(void) {
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create and initialize the hash table
    struct HashType hashTable;
    for (int i = 0; i < HASH_SIZE; ++i) {
        hashTable.table[i] = NULL;
    }
    for (int i = 0; i < recordSz; ++i) {
        insertIntoHash(&hashTable, pRecords[i]);
    }

    displayRecordsInHash(&hashTable, HASH_SIZE);
    free(pRecords);

    return 0;
}
