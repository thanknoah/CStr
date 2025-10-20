#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct strObj strObj;

struct strObj {
	char* data;
	size_t capacity;
	size_t size;
	bool destroyed;

	void (*destroy)(strObj*);
	void (*assign)(strObj*, const char*);
	bool (*cmp)(strObj*, const char*);
};


void delete_string(strObj* currentStr) {
	free(currentStr->data);

	currentStr->destroyed = true;
	currentStr->capacity = 0;
	currentStr->data = NULL;
	currentStr->size = 0;
}

bool equal_string(strObj* currentStr, const char* newStr) {
	if (currentStr->destroyed) {
		printf("\nCStr Error: str is destroyed\n");
		return false;
	}

	if (strcmp(currentStr->data, newStr) == 0)
		return true;

    return false;
}

void modify_string(strObj* currentStr, const char* newStr) {
	size_t newStrLen = strlen(newStr) + 1;
	size_t predictedNewSize = (size_t)(currentStr->capacity * 2);
	size_t newSize;

	if (currentStr->destroyed) {
		printf("\nCStr Error: str is destroyed\n");
		return;
	}

	if (newStrLen > currentStr->capacity) {
		if (predictedNewSize > newStrLen) {
			newSize = predictedNewSize;
		}
		else {
			newSize = newStrLen;
		}

		char* newMemoryBlock = (char*)malloc(newSize);
		if (newMemoryBlock == NULL) {
			printf("\nCStr Error: memory allocation failed\n");
			return;
		}

		free(currentStr->data);

		currentStr->data = newMemoryBlock;
		currentStr->capacity = newSize;
		currentStr->size = newStrLen - 1;
	}

	errno_t err = memcpy_s(currentStr->data, currentStr->capacity, newStr, newStrLen);
	if (err != 0) {
		printf("\nCStr Error: failed to copy new char to char ptr\n");
		return;
	}
}


strObj create_str(strObj* e) {
	e->data = (char*)malloc(sizeof(char) * 15);
	e->capacity = (size_t)(sizeof(char) * 15);
	e->destroyed = false;
	e->size = 15;

	e->destroy = delete_string;
	e->assign = modify_string;
	e->cmp = equal_string;


	if (e->data == NULL) {
		printf("\nCStr Error: memory allocation failed\n");
		e->destroyed = true;
		e->capacity = 0;
		e->data = NULL;
		e->size = 0;
		return *e;
	}

	e->data[0] = '\0';
	return *e;
}

int main() {
	// Create string objects
	strObj name = create_str(&name);
	strObj greeting = create_str(&greeting);
	strObj secret = create_str(&secret);

	// Assign initial values
	name.assign(&name, "Alice");
	greeting.assign(&greeting, "Hello");
	secret.assign(&secret, "OpenSesame");

	// Getting values
	printf(name.data); // Prints Alice
	printf(greeting.data); // Prints Hello
	printf(secret.data) // Prints OpenSesame

	// Check if values equal to something
	name.cmp(&name, "Alice"); // Results in true
	greeting.cmp(&greeting, greeting.data) // Results in true, compares to itself
    secret.cmp(&secret, name.data) // Results in false

	// Modifying values
	name.assign(&name, "ExampleNameThatIsReallyLong"); // Readjusts memory size by x2
	greeting.assign(&greeting, "ExampleGreetingThatIsReallyLong");  // Readjusts memory size by x2
	secret.assign(&secret, "ExampleSecretThatIsReallyLong");  // Readjusts memory size by x2

	// Cleanup, doesnt auto clean up yet for C
	name.destroy(&name);
	greeting.destroy(&greeting);
	secret.destroy(&secret);

	return 0;
}
