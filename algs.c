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
	
	//omp_set_num_threads(4);
	
#pragma omp parallel for private(j, dj, di, currentSum)
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
		
	for (i = 0; i < task.M; i++) 
	for (j = 0; j < task.N; j++) 
		
		for (di = 1; di <= task.M - i; di++)
		for (dj = 1; dj <= task.N - j; dj++) {
			
			currentSum = sum_subarray(i, j, i + di, j + dj);
			if (currentSum > maxSum) {
				maxSum = currentSum;
				maxI0 = i;
				maxJ0 = j;
				maxI1 = i + di - 1;
				maxJ1 = j + dj - 1;
			}
		}
		
	struct frame result = { maxI0, maxJ0, maxI1, maxJ1, maxSum };
	return result;
}

struct frame dailyOne(struct cortage task) {
	int i, j, i0, j0, currentSum;
	int maxSum = A[0][0];
	
	int maxI0 = 0, maxJ0 = 0, maxI1 = 0, maxJ1 = 0;
	
	omp_lock_t lock;
	omp_init_lock(&lock);

	int **H = A;
	for (i = 0; i < task.M; i++) 
		for (j = 0; j < task.N; j++) 
			H[i][j] = 
				(i ? H[i-1][j] : 0)	+ (j ? H[i][j-1] : 0) + A[i][j] 
				- ((i && j) ? H[i-1][j-1] : 0);		
		
#pragma omp parallel for private(j, j0, i0, currentSum) shared(maxSum)
	for (i = 0; i < task.M; i++) 
	for (j = 0; j < task.N; j++) 
		for (i0 = 0; i0 <= i; i0++)
		for (j0 = 0; j0 <= j; j0++) {
			
			currentSum = H[i][j] 
				- ( i0 ? H[i0 - 1][j] : 0 ) 
				- ( j0 ? H[i][j0 - 1] : 0 )
				+ ((i0 && j0) ? H[i0-1][j0-1] : 0);
						
			if (currentSum > maxSum) {
				omp_set_lock(&lock);
				maxSum = currentSum;
				maxI0 = i0;
				maxJ0 = j0;
				maxI1 = i;
				maxJ1 = j;
				omp_unset_lock(&lock);
			}
		}
		
	struct frame result = { maxI0, maxJ0, maxI1, maxJ1, maxSum };
	return result;
}

struct frame Kadane(struct cortage task) {
	int i, j, k, top, bottom, left, right, maxSum;
	bottom = top = left = right = 0;
	maxSum = A[0][0];
	int local_max, value;
	
	// calculate vertical prefix sum
	int vps[task.M][task.N];
	
	for (j = 0; j < task.N; j++) {
		vps[0][j] = A[0][j];
		for (i = 1; i < task.M; i++) 
			vps[i][j] = vps[i-1][j] + A[i][j];
	}
	
	omp_lock_t lock;
	omp_init_lock(&lock);

	
//#pragma omp parallel for private(j,k, local_max) shared(maxSum)
	for (i = 0; i < task.M; i++) {
		for (k = i; k < task.M; k++) {
			int sum[task.N], pos[task.N];
			
			memset(sum, 0, sizeof(int) * task.N);
			memset(pos, 0, sizeof(int) * task.N);
			local_max = 0;
						
			sum[0] = vps[k][0] - (i ? vps[i - 1][0] : 0);
		
			for (j = 1; j < task.N; j++) {
				value = vps[k][j] - (i ? vps[i - 1][j]: 0);
				if (sum[j - 1] > 0) {
					sum[j] = sum[j - 1] + value;
					pos[j] = pos[j - 1];
				} else {
					sum[j] = value;
					pos[j] = j;
				}
				
				if (sum[j] > sum[local_max])
					local_max = j;
			}
			
			omp_set_lock(&lock);
			// Here's the key thing
			// If the max value obtained over the past Kadane's execution is larger than
			// the current maximum, then update the max array with sum and bounds
			if (sum[local_max] > maxSum) {
			// sum[local_max] is the new max value
			// the corresponding submatrix goes from rows i..k.
			// and from columns pos[local_max]..local_max
			// the array below contains [max_sum,top,left,bottom,right]
				maxSum = sum[local_max];
				top = k;
				bottom = i;
				left = pos[local_max];
				right = local_max;
			}	
			omp_unset_lock(&lock);	
		}
	}

	struct frame result = { bottom, left, top, right, maxSum }; 
	return result;
}
