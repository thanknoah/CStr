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

	memcpy(currentStr->data, newStr, newStrLen);
	currentStr->size = newStrLen - 1;
	currentStr->data[currentStr->size] = '\0';
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

	memcpy(currentStr->data + currentStr->size, newStr, newStrLen);
	currentStr->size += newStrLen;
	currentStr->data[currentStr->size] = '\0';  // ensure null termination
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
	StrObj e = create_str(&e);

	e.assign(&e, "Hello");
	e.append(&e, " World");
	printf(e.data); // Prints Hello World
	e.cmp(&e, "Hello World"); // Returns true
	e.destroy(&e);


	return 0;
}
