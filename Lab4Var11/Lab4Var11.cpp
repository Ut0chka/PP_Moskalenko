#include <iostream>
#include <string>
#include <thread>
#include "mpi.h"

using namespace std::chrono_literals;

int main(int argc, char** argv)
{
	const int MAX_MSG_LEN = 200;
	const int MAX_MSG_COUNT = 10;
	int rank;
	char question[MAX_MSG_LEN];
	char answer[MAX_MSG_LEN];
	MPI_Status mpi_status;
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0) {
		for (int i = 0; i < MAX_MSG_COUNT; i++) {
			MPI_Recv(question, MAX_MSG_LEN, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_status);
			std::cout << "Deputy received \"" << question << "\" from " << mpi_status.MPI_SOURCE << std::endl;
			std::string tmp = "Some abstract answer";
			strcpy_s(answer, tmp.c_str());
			std::this_thread::sleep_for(200ms);
			MPI_Send(answer, MAX_MSG_LEN, MPI_CHAR, mpi_status.MPI_SOURCE, 0, MPI_COMM_WORLD);
		}
		std::cout << "The reception is over!" << std::endl;
		MPI_Abort(MPI_COMM_WORLD, MPI_SUCCESS);
	}
	else {
		std::string tmp = "Some stupid question";
		strcpy_s(question, tmp.c_str());
		MPI_Send(question, MAX_MSG_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		MPI_Recv(answer, MAX_MSG_LEN, MPI_CHAR, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_status);
		std::cout << "Man " << rank << " received \"" << answer << "\"" << std::endl;
	}
	MPI_Finalize();
	return 0;
}