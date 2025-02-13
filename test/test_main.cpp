#define CATCH_CONFIG_RUNNER
#include <iostream>
#include <catch2/catch_session.hpp>
#include<mpi.h>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int result = Catch::Session().run( argc, argv );
  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return result;
}


