#include <iostream>
#include <mpi.h>

using namespace std;
const long long  itnum = 1000000000;
long long sum = 0;

int ___main(int argc, char **argv)
{
	int id, nproc;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc);
	
	long long step;
	MPI_Status state;
	if (id == 0)
	{
		step = itnum / (nproc - 1);
		for (int i = 1; i < nproc; i++)
			MPI_Send(&step, 1, MPI_LONG_LONG, i, 1, MPI_COMM_WORLD);

		long long rcsum = 0;
		for (int i = 1; i < nproc; i++)
		{
			MPI_Recv(&rcsum, 1, MPI_LONG_LONG, i, 2, MPI_COMM_WORLD, &state);
			sum += rcsum;
		}
		//double pi = (4 * sum) / itnum;
		cout << "PI : " << (4 * sum) / itnum << endl;
	}
	else
	{
		double x, y;
		MPI_Recv(&step, 1, MPI_LONG_LONG, 0, 1, MPI_COMM_WORLD, &state);
		
		for (long long i = 0; i < itnum; i++)
		{
			x = (double)rand() / RAND_MAX;
			y = (double)rand() / RAND_MAX;
			if (x*x + y*y < 1)
				sum++;
		}

		MPI_Send(&sum, 1, MPI_LONG_LONG, 0, 2, MPI_COMM_WORLD);
	}
	
	

	MPI_Finalize();
	return 0;
}