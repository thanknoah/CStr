// Around 2.5x quicker at modifying / appending && 8x quicker in highest time taken at modifying / appending compared to std::string

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StrObj {
	char* data;
	size_t capacity;
	size_t size;

	void (*destroy)(StrObj*);
	void (*assign)(StrObj*, const char*);
	void (*append)(StrObj*, const char*);
	bool (*cmp)(StrObj*, const char*);
} StrObj;


void delete_string(StrObj* currentStr) {
	free(currentStr->data);

	currentStr->capacity = 0;
	currentStr->data = NULL;
	currentStr->size = 0;
}

bool equal_string(StrObj* currentStr, const char* newStr) {
	return strcmp(currentStr->data, newStr) == 0;
}

void modify_string(StrObj* currentStr, const char* newStr) {
	size_t newStrLen = strlen(newStr) + 1;
	size_t predictedNewSize = currentStr->capacity * 2;
	size_t newSize;

	if (newStrLen > currentStr->capacity) {
		newSize = (predictedNewSize > newStrLen) ? predictedNewSize : newStrLen;

		char* newMemoryBlock = (char*)malloc(newSize);
		if (!newMemoryBlock) {
			printf("\nCStr Error: memory allocation failed\n");
			return;
		}

		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
	}

	memcpy(currentStr->data, newStr, newStrLen+1);
}

void append_string(StrObj* currentStr, const char* newStr) {
	size_t newStrLen = strlen(newStr);
	size_t newTotalStrLen = currentStr->size + newStrLen + 1;
	size_t predictedNewSize = currentStr->capacity * 2;
	size_t newSize;

	if (newTotalStrLen > currentStr->capacity) {
		newSize = (predictedNewSize > newTotalStrLen) ? predictedNewSize : newTotalStrLen;

		char* newMemoryBlock = (char*)malloc(newSize);
		if (!newMemoryBlock) {
			printf("\nCStr Error: memory allocation failed\n");
			return;
		}

		memcpy(newMemoryBlock, currentStr->data, currentStr->size);
		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
	}

	memcpy(currentStr->data + currentStr->size, newStr, newStrLen+1);
}


StrObj create_str(StrObj* e) {
	e->data = (char*)malloc(15);
	e->capacity = 15;
	e->size = 0;

	e->destroy = delete_string;
	e->append = append_string;
	e->assign = modify_string;
	e->cmp = equal_string;

	if (!e->data) {
		printf("\nCStr Error: memory allocation failed\n");
		e->capacity = 0;
		e->data = NULL;
		e->size = 0;
		return *e;
	}

	e->data[0] = '\0';
	return *e;
}

int main() {
	StrObj mystring = create_str(&mystring); 
	mystring.assign(&mystring, "Hello "); 
	mystring.append(&mystring, "World"); 

	mystring.cmp(&mystring, mystring.data); // comparing with itself Returns true
	mystring.cmp(&mystring, "Bye World"); // Returns false

	printf(mystring.data); 
	mystring.destroy(&mystring);
	return 0;
}
