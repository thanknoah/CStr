#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StrObj {
	char* data;
	size_t capacity;
	size_t size;
	bool memoryEfficency;

} StrObj;

typedef struct StrLib {
	void (*get_input)(StrObj*, size_t);
	void (*destroy)(StrObj*);
	void (*assign)(StrObj*, const char*);
	void (*append)(StrObj*, const char*);
	void (*move)(StrObj*, StrObj*);
	bool (*cmp)(const char*, const char*);
} StrLib;

bool alloc_custom_str_mem(StrObj* currentStr, size_t size) {
	char* newMemoryBlock = (char*)malloc(size + 1);
	if (!newMemoryBlock) {
		printf("\nCStr Error: memory allocation failed\n");
		return false;
	}

	free(currentStr->data);
	currentStr->data = newMemoryBlock;
	currentStr->capacity = size + 1;

	return true;
}

void delete_string(StrObj* currentStr) {
	free(currentStr->data);

	currentStr->capacity = 0;
	currentStr->data = NULL;
	currentStr->size = 0;
}


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

void move_ownership(StrObj* oldStr, StrObj* newStr) {
	if (newStr->capacity < oldStr->size + 1) {
		bool allocateMemory = alloc_custom_str_mem(newStr, oldStr->size + 1);
		if (!allocateMemory) {
			printf("\nCStr Error: failed to transfer ownership of str");
			return;
		}
	}

	free(newStr->data);
	newStr->data = oldStr->data;
	newStr->size = oldStr->size;
	oldStr->data = NULL;
	delete_string(oldStr);
}

bool equal_string(const char* firstStr, const char* secondStr) {
	return strcmp(firstStr, secondStr) == 0;
}

void recv_input(StrObj* currentStr, size_t charLength) {
	bool allocatedMemory = alloc_custom_str_mem(currentStr, charLength);
	if (!allocatedMemory) {
		printf("\nCStr Error: failed to get input");
		return;
	}

	fgets(currentStr->data, charLength, stdin);
	size_t size = strlen(currentStr->data);

	if (currentStr->data[size - 1] == '\n' && size > 0) {
		currentStr->data[size - 1] = '\0';
		size--;
	}

	currentStr->size = size;
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


		bool allocatedMemory = alloc_custom_str_mem(currentStr, newStrLen);
		if (!allocatedMemory) {
			printf("\nCStr Error: failed to assign new value to StrObj");
			return;
		}
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

StrLib init_str_lib(StrLib* e) {
	e->destroy = delete_string;
	e->append = append_string;
	e->assign = modify_string;
	e->cmp = equal_string;
	e->get_input = recv_input;
	e->move = move_ownership;

	return *e;
}

StrObj init_str(StrObj* e) {
	e->data = (char*)malloc(15);
	e->memoryEfficency = false;
	e->capacity = 15;
	e->size = 0;

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

// EXAMPLE PROGRAM
void test() {
	StrLib strlib = init_str_lib(&strlib);
	StrObj string1 = init_str(&string1);
	StrObj string2 = init_str(&string2);
    StrObj input = init_str(&input);

	strlib.assign(&string1, "My String");
	strlib.assign(&string2, "My String");
	strlib.cmp(string1.data, string2.data); // Returns true

	strlib.append(&string1, " is the best!");
	printf(string1.data); // Prints My String is the best!
	printf("\n");
	printf(string2.data); // Prints My String
	printf("\n");

	strlib.move(&string1, &string2); // moves ownership from string 1 to string 2, string1 becomes valid

	printf("\n");
	strlib.get_input(&input, 200); // gets input, max 200 chars
	if (strlib.cmp(input.data, "Test")) { // checks if input is equal to test
		printf("\nNice you guessed the password");
	}

	// Make sure to destroy striings!
	strlib.destroy(&string1);
	strlib.destroy(&string2);
	strlib.destroy(&input);
}
