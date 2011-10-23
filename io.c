int readinput(char* filename) {
	int i;
	FILE* file = fopen(filename, "r");
	fscanf(file, "%d", &TEST_COUNT);
	inputs = (struct cortage*)malloc(TEST_COUNT * sizeof(struct cortage));
	
	for (i = 0; i < TEST_COUNT; i++) {
		
		fscanf(file, "%d %d %d %d %d %d\n", 
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

void printCortage(struct cortage task) {
	printf("[%dx%d] (s=%d,a=%d,b=%d,m=%d)", task.M, task.N, task.seed0, task.a, task.b, task.m);
}

void printFrame(int idx, struct frame f) {
	printf("Case #%d: %d %d %d %d %d %d\r\n", idx + 1, f.mini, f.minj, f.maxi, f.maxj, f.sum, (f.maxi - f.mini + 1) * (f.maxj - f.minj + 1));
	//printf("[(%d, %d) -> (%d,%d)] = %d", f.mini, f.minj, f.maxi, f.maxj, f.sum);
}

void printA(struct cortage info) {
	int i, j;
	for (i = 0; i < info.M; i++) {
		for (j = 0; j < info.N; j++)
			printf("%d\t", A[i][j]);
		printf("\r\n");
	}
}

struct frame readNextTest() {
	int idx, area;
	struct frame f;
	fscanf(TST, "%d %d %d %d %d %d\n", &f.mini, &f.minj, &f.maxi, &f.maxj, &f.sum, &area);
	return f;
}