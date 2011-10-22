#include "io.c"
#include "algs.c"

void runalgorithms(int i) {
	
	naive(inputs[i]);
}

int main(int argc, char** argv) {
	int i, testcount = readinput(argv[1]);
	
	for (i = 0; i < testcount; i++) {
		runalgorithms(i);
	}
	
	return 0;
}