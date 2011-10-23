int sum_subarray(int i, int j, int ii, int jj) {
	int y, x;
	int res = 0;
	for (y = i; y < ii; y++)
		for (x = j; x < jj; x++)
			res += A[y][x];
	return res;
}

struct frame naive_parallel(struct cortage task) {
	int i, j, di, dj, currentSum;
	int maxSum = A[0][0];
	
	int maxI0 = 0, maxJ0 = 0, maxI1 = 0, maxJ1 = 0;
	
	omp_lock_t lock;
	omp_init_lock(&lock);
	
	omp_set_num_threads(4);
	
#pragma omp parallel for private(j, dj, di)
	for (i = 0; i < task.M; i++) {
	for (j = 0; j < task.N; j++) 
		
		for (di = 1; di <= task.M - i; di++)
		for (dj = 1; dj <= task.N - j; dj++) {
			
			currentSum = sum_subarray(i, j, i + di, j + dj);
			if (currentSum > maxSum) {
				omp_set_lock(&lock);
				maxSum = currentSum;
				maxI0 = i;
				maxJ0 = j;
				maxI1 = i + di - 1;
				maxJ1 = j + dj - 1;
				omp_unset_lock(&lock);
			}
		}
	}	
	struct frame result = { maxI0, maxJ0, maxI1, maxJ1, maxSum };
	return result;
}


struct frame naive(struct cortage task) {
	int i, j, di, dj, currentSum;
	int maxSum = A[0][0];
	
	int maxI0 = 0, maxJ0 = 0, maxI1 = 0, maxJ1 = 0;
	
	omp_lock_t lock;
	omp_init_lock(&lock);
	
	for (i = 0; i < task.M; i++) 
	for (j = 0; j < task.N; j++) 
		
		for (di = 1; di <= task.M - i; di++)
		for (dj = 1; dj <= task.N - j; dj++) {
			
			currentSum = sum_subarray(i, j, i + di, j + dj);
			if (currentSum > maxSum) {
				omp_set_lock(&lock);
				maxSum = currentSum;
				maxI0 = i;
				maxJ0 = j;
				maxI1 = i + di - 1;
				maxJ1 = j + dj - 1;
				omp_unset_lock(&lock);
			}
		}
		
	struct frame result = { maxI0, maxJ0, maxI1, maxJ1, maxSum };
	return result;
}