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
	bool (*cmp)(const char*, const char*);
} StrLib;


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

bool equal_string(const char* firstStr, const char* secondStr) {
	return strcmp(firstStr, secondStr) == 0;
}

bool input_custom_str_mem(StrObj* currentStr, size_t size) {
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


void recv_input(StrObj* currentStr, size_t charLength) {
	bool allocatedMemory = input_custom_str_mem(currentStr, charLength);
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

StrLib init_str_lib(StrLib* e) {
	e->destroy = delete_string;
	e->append = append_string;
	e->assign = modify_string;
	e->cmp = equal_string;
	e->get_input = recv_input;

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

int main() {
	// Initiate Str Lib
	StrLib strlib = init_str_lib(&strlib);
	
    /*
    You can also do it this way
	StrLib strlib;
	init_str_lib(&strlib);
	*/
	
	// Initate Strings
	StrObj user = init_str(&user);
	StrObj pass = init_str(&pass);
	StrObj message = init_str(&message);

	/*
	You can also do it this way
	StrObj user;
	StrObj pass;
	StrObj message;

	init_str(&user);
	init_str(&pass);
	init_str(&message);
	*/

	// Assigning str to value
	strlib.assign(&message, "You have");

	// Get Inputs
	printf("User: ");
	strlib.get_input(&user, 200);
	printf("Password: ");
	strlib.get_input(&pass, 200);

	// Check if they match
	if (strlib.cmp(user.data, "Noah") && strlib.cmp(pass.data, "MySecretPassword")) {
		strlib.append(&message, " successfuly logged in!"); // Appending to string
	}
	else {
		strlib.append(&message, " successfuly NOT logged in!"); // Appending to string
	}

	// Print result
	printf("\n");
	printf(message.data);

	// Change message and reprint it
	strlib.assign(&message, "Thank you for visiting my program, bye!");
    printf("\n");
	printf(message.data);

	// Additional Data
	message.size; // length of str
	message.capacity; // size of memory block
	message.memoryEfficency; // u can set this to true or false but recommended to be false for peformance


	return 0;
}
