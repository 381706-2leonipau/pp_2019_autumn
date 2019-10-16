//Copyright 2019 Pauzin Leonid
#include "../../../modules/task_1/pauzin_l_min_elem_in_vector/pauzin_l_min_elem_in_vector.h"
#include <vector>
#include <mpi.h>	
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <stdexcept>


int getSequentialMin(std::vector<int> vec) {
	const int  n = vec.size();
	int min = vec[0];
	for (int i = 0; i < n; i++) {
		if (min > vec[i]) {
			min = vec[i];
		}
	}
	return min;
}

int minElemInVec(const std::vector <int> global_vec, int n) {
	int rezult = 10;
	int min;
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int delta = n / size;
	const int remainder = n % size; //ostatok 
	std::vector<int> local_vec;
	if (n < size) {
		if (rank == 0) {
			rezult = getSequentialMin(global_vec);
			return rezult;
		}
		else {
			return rezult;
		}
	}

	if (rank == 0) {
		for (int proc = 1; proc < size; proc++) {
			MPI_Send(&global_vec[0] + proc * delta + remainder, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
		}
	}


	if (rank == 0) {
		min = global_vec[0];
		for (int i = 0; i < delta + remainder; i++) {
			min = std::min(min, global_vec[i]);
		}
	} else {
		local_vec.resize(delta);
		MPI_Status status;
		MPI_Recv(&local_vec[0], delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		min = local_vec[0];
		min = getSequentialMin(local_vec);
	}
//...................................................
	MPI_Reduce(&min, &rezult, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
	return rezult;
}

std::vector<int> getVec(int n) {
	std::vector<int> vec1(n);
	for (int i = 0; i < n; i++)
	{
		vec1[i] = i;
	}
	return vec1;
}

std::vector<int> getRandomVector(int n) {
	std::mt19937 gen;
	gen.seed(static_cast<unsigned int>(time(0)));
	std::vector<int> vec(n);
	for (int i = 0; i < n; i++) { vec[i] = gen() % 100; }
	return vec;
}


