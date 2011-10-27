#include "global.c"
#include "io.c"
#include "algs.c"

inline int PRNG(int seed, int a, int b, int m) {
	return (a * seed + b) % m;
}

void fillA(struct cortage c, int** A) {
	int i, j, mean,	sum = 0, seed = c.seed0;
	for (i = 0; i < c.M; i++) 
		for (j = 0; j < c.N; j++) {
			seed = PRNG(seed, c.a, c.b, c.m);
			A[i][j] = seed;
			sum += A[i][j];	
	}
	mean = ((float)sum / (c.M * c.N)) + 0.5;
	for (i = 0; i < c.M; i++) 
		for (j = 0; j < c.N; j++)
			A[i][j] -= mean;
}

inline char testCorrectness(struct frame a, struct frame b) {
	return /*!(a.mini - b.mini) && !(a.minj - b.minj) && !(a.maxi - b.maxi) && !(a.maxj - b.maxj) &&*/ !(a.sum - b.sum); 
}

void runalgorithms(int testIdx) {
	int i;
	double start, finish;
	struct cortage task = inputs[testIdx];
	struct frame f;
	A = (int**)malloc(task.M * sizeof(int*));
	for (i = 0; i < task.M; i++) 
		A[i] = (int*)malloc(task.N * sizeof(int));
	
	printf("\r\n========= test [%d]", testIdx);
	printCortage(task);
	printf("=========\r\n");	
	struct frame test = readNextTest();
	printFrame(testIdx, test);
	fillA(task, A);
	
	if (task.M * task.N < 5000) {
		//------------------------ one alg block -------------------------------
		start = omp_get_wtime();
		f = naive_parallel(inputs[testIdx]);
		finish = omp_get_wtime();
		printFrame(testIdx, f);
		printf(" | naive parallel[%d]: %lf ... %s\r\n", testIdx, (finish - start), testCorrectness(test, f) ? "ok" : "failed");
		//----------------------------------------------------------------------

		//------------------------ one alg block -------------------------------
		start = omp_get_wtime();
		f = naive(inputs[testIdx]);
		finish = omp_get_wtime();
		printFrame(testIdx, f);
		printf(" | naive[%d]: %lf ... %s\r\n", testIdx, (finish - start), testCorrectness(test, f) ? "ok" : "failed");
		//----------------------------------------------------------------------
	}

	//------------------------ one alg block -------------------------------
	start = omp_get_wtime();
	f = Kadane(inputs[testIdx]);
	finish = omp_get_wtime();
	printFrame(testIdx, f);
	printf(" | Kadane[%d]: %lf ... %s\r\n", testIdx, (finish - start), testCorrectness(test, f) ? "ok" : "failed");
	//----------------------------------------------------------------------

	//------------------------ one alg block -------------------------------
	start = omp_get_wtime();
	f = dailyOne(inputs[testIdx]);
	finish = omp_get_wtime();
	printFrame(testIdx, f);
	printf(" | daily one[%d]: %lf ... %s\r\n", testIdx, (finish - start), testCorrectness(test, f) ? "ok" : "failed");
	//----------------------------------------------------------------------

	for (i = 0; i < task.M; i++) free(A[i]);
	free(A);	
}

int main(int argc, char** argv) {
	int i, testcount = readinput(argv[1]);
	TST = fopen("test.txt", "r");
	for (i = 0; i < testcount; i++) runalgorithms(i);
	fclose(TST);
	return 0;
}
