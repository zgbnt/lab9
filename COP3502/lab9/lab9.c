#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct Node** HashArray;
	int ArraySize;
};

struct Node
{
	struct Node* next;
	struct RecordType* Rptr;
};

// Compute the hash function
int hash(int x)
{
	return (x % 31);
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
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
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;
	struct Node* temp;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		temp = pHashArray->HashArray[i];
		while(temp != NULL && temp->Rptr != NULL) {
			printf("\t%d %c %d\n", temp->Rptr->id, temp->Rptr->name, temp->Rptr->order);
			temp = temp->next;
		}
		printf("\n\n");
	}
}

int main(void)
{
	struct HashType *pHash;
	struct RecordType *pRecords;
	struct Node *temp;
	int recordSz = 0;
	int count = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
	pHash = (struct HashType*)malloc(sizeof(struct HashType));
	pHash->ArraySize = recordSz;
	pHash->HashArray = (struct Node**)malloc(sizeof(struct Node*) * pHash->ArraySize);
	for(int i = 0; i < pHash->ArraySize; i++) {
		pHash->HashArray[i] = (struct Node*)malloc(sizeof(struct Node));
		pHash->HashArray[i]->next = NULL;
		pHash->HashArray[i]->Rptr = NULL;
	}
	for(int i = 0; i < recordSz; i++) {
		(pRecords + i)->order = hash(pRecords->id);
		int sort = (pRecords + i)->order;
		temp = pHash->HashArray[sort];
		printf("%d", sort);
		if(pHash->HashArray[sort]->Rptr == NULL) {
			pHash->HashArray[sort]->Rptr = (pRecords + i);
		}
		else {
			struct Node* temp2;
			while (temp != NULL) {
				temp2 = temp;
				temp = temp->next;
			}
			temp = (struct Node*)malloc(sizeof(struct Node));
			temp->Rptr = (pRecords + i);
			temp->next = NULL;
			temp2->next = temp;
		}
	}
	// assign order
	for(int i = 0; i < pHash->ArraySize; i++) {
		temp = pHash->HashArray[i];
		while(temp != NULL && temp->Rptr != NULL) {
			temp->Rptr->order = count;
			count++;
			temp = temp->next;
		}
	}
	displayRecordsInHash(pHash, pHash->ArraySize);
}