#include <stdio.h>
#include <stdlib.h>

struct cortage { int M, N, seed0, a, b, m; };

int TEST_COUNT;
struct cortage *inputs;

int readinput(char* filename) {
	int i;
	FILE* file = fopen(filename, "r");
	fscanf(file, "%d", &TEST_COUNT);
	inputs = (struct cortage*)malloc(TEST_COUNT * sizeof(struct cortage));
	
	for (i = 0; i < TEST_COUNT; i++) {
		
		fscanf(file, "%d %d %d %d %d %d", 
				&(inputs[i].M),
				&(inputs[i].N),
				&(inputs[i].seed0),
				&(inputs[i].a),
				&(inputs[i].b),
				&(inputs[i].m)
				);
	}
	printf("test count: %d\r\n", TEST_COUNT);
	fclose(file);
	return TEST_COUNT;
}