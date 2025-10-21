#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <chrono>
#include <vector>

typedef struct StrObj {
	char* data;
	size_t capacity;
	size_t size;
	bool memoryEfficency;

	void (*destroy)(StrObj*);
	void (*assign)(StrObj*, const char*);
	void (*append)(StrObj*, const char*);
	bool (*cmp)(StrObj*, const char*);
} StrObj;

size_t performance_growth(size_t currentCapacity) {
	size_t predicted;
	if (currentCapacity < 4 << 10) { 
		predicted = currentCapacity << 1;   
	}
	else if (currentCapacity < 4 << 20) { 
		predicted = currentCapacity + (currentCapacity >> 2) + (currentCapacity >> 1);
	}
	else if (currentCapacity < 512 << 20) {
		predicted = currentCapacity + (currentCapacity >> 1); 
	}
	else {
		predicted = currentCapacity + (currentCapacity >> 2);
	}

	return predicted;
}

size_t memory_efficient_growth(size_t currentCapacity) {
	size_t predicted;
	if (currentCapacity < 4 << 10) {  
		predicted = currentCapacity << 1;
	}
	else if (currentCapacity < 1 << 20) {         
		predicted = currentCapacity + (currentCapacity >> 1); 
	}
	else if (currentCapacity < 100 << 20) {      
		predicted = currentCapacity + (currentCapacity >> 3); 
	}
	else {                                        
		predicted = currentCapacity + (currentCapacity >> 4);
	}

	return predicted;
}


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
	size_t newStrLen = strlen(newStr);
	size_t newStrSize = newStrLen + 1;
	size_t predictedNewSize;
	size_t newSize;

	if (newStrSize > currentStr->capacity) {
		if (currentStr->memoryEfficency)
			predictedNewSize = memory_efficient_growth(currentStr->capacity);
		else
			predictedNewSize = performance_growth(currentStr->capacity);

		if (newStrSize > predictedNewSize) 
			newSize = newStrSize;
		else 
			newSize = predictedNewSize;


		char* newMemoryBlock = (char*)malloc(newSize);
		if (!newMemoryBlock) {
			printf("\nCStr Error: memory allocation failed\n");
			return;
		}

		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
	}

	memcpy(currentStr->data, newStr, newStrSize);
	currentStr->size = newStrLen;
}

void append_string(StrObj* currentStr, const char* newStr) {
	size_t newStrLen = strlen(newStr);
	size_t newStrSize = newStrLen + 1;
	size_t newTotalStrLen = currentStr->size + newStrSize + 1;
	size_t predictedNewSize;
	size_t newSize;

	if (newTotalStrLen > currentStr->capacity) {
		if (currentStr->memoryEfficency)
			predictedNewSize = memory_efficient_growth(currentStr->capacity);
		else
			predictedNewSize = performance_growth(currentStr->capacity);

		newSize = newTotalStrLen > predictedNewSize ? newTotalStrLen : predictedNewSize;

		char* newMemoryBlock = (char*)malloc(newSize);
		if (!newMemoryBlock) {
			printf("\nCStr Error: memory allocation failed\n");
			return;
		}

		memcpy(newMemoryBlock, currentStr->data, currentStr->size + 1);
		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
	}

	memcpy(currentStr->data + currentStr->size, newStr, newStrSize);
	currentStr->size += newStrLen;
}


StrObj create_str(StrObj* e) {
	e->data = (char*)malloc(15);
	e->memoryEfficency = false;
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
	// Functions
	StrObj s = create_str(&s);
	StrObj a = create_str(&a);

	s.assign(&s, "Hello"); a.assign(&a, "Bye"); // Assigns value
	s.append(&s, " World!"); a.append(&a, " World!"); // Appends value

	s.cmp(&s, "Hello World!"); // Compare with strings returns true
	s.cmp(&s, "Bye World!"); // Compare with strings returns false
	s.cmp(&s, a.data); // compare with other StrObj (a) returns false

	// Print Data
	printf("String data: ");
	printf(s.data);
	printf("\nSize of string: %zu", s.size);
	printf("\nCapacity of string: %zu", s.capacity);


	s.memoryEfficency = true; // Optional, but leaving it to false is recommended for peformance
	s.destroy(&s); // Destroy String

	return 0;
}
