#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct strObj {
	char* data;
	size_t capacity;
	size_t size;
	bool destroyed;
}; 

void str_destroy_internal(strObj* currentStr) {
	currentStr->destroyed = true;
	currentStr->capacity = NULL;
	currentStr->data = NULL;
	currentStr->size = NULL;
}

void str_destroy(strObj* currentStr) {
	free(currentStr->data);

	currentStr->destroyed = true;
	currentStr->capacity = NULL;
	currentStr->data = NULL;
	currentStr->size = NULL;
}

void str_create(strObj* e) {
	e->data = (char*)malloc(sizeof(char) * 15);
	e->capacity = (size_t)(sizeof(char) * 15);
	e->destroyed = false;
	e->size = 15;

	if (e->data == NULL) {
		printf("CStr Error: memory allocation failed");
		str_destroy_internal(e);
		return;
	}

	e->data[0] = '\0';
}

void str_modify(strObj* currentStr, const char* newStr) {
	size_t newStrLen = strlen(newStr) + 1;
	size_t predictedNewSize = (size_t)(currentStr->capacity * 2);
	size_t newSize;

	if (currentStr->destroyed) {
		printf("CStr Error: str is destroyed");
		return;
	}

	if (newStrLen > currentStr->capacity) {
		if (predictedNewSize > newStrLen)
			newSize = predictedNewSize;
		else 
			newSize = newStrLen;
		
		char* newMemoryBlock = (char*)malloc(newSize);
		if (newMemoryBlock == NULL) {
			printf("CStr Error: memory allocation failed");
			return;
		}

		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
		currentStr->size = newStrLen - 1;
	}
	
	errno_t err = memcpy_s(currentStr->data, currentStr->capacity, newStr, newStrLen);
	if (err != 0) {
		printf("CStr Error: failed to copy new char to char ptr");
		return;
	}
}

int main() {
	strObj test;
	str_create(&test);
	str_modify(&test, "Hello World");
	printf(test.data);
	str_destroy(&test);
}
