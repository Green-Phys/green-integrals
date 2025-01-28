#include <catch2/catch_test_macros.hpp>
#include "green/integrals/buffered_reader/buffer.hpp"
#include "green/integrals/buffered_reader/chunk_reader.hpp"
#include <mpi.h>
#include <openssl/sha.h>
#include <iomanip>


// For these tests of reading integrals as chunks, we use cooked up density-fitted integrals.
// system           :   Silicon
// k-mesh           :   2x2x2
// basis            :   gth-dzvp-molopt-sr
// pseudo           :   gth-pbe
// size per file    :   1MB
// beta (df params) :   2.0
// diffuse_cutoff   :   0.3

using namespace std::string_literals;

TEST_CASE("HashOfKeys","[ReadingSI]") {
  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path=TEST_PATH+"/Si/df_int"s;

  unsigned char digest[SHA_DIGEST_LENGTH];

  chunk_reader c(df_path, number_of_keys, naux, nao); //test these numbers
  buffer b(c.element_size(), number_of_keys, 100, &c, 0, false);
  if(b.shmem_rank()==0){
  for(int i=0;i<5;++i){
    const double* val=b.access_element(i);
    SHA1((unsigned char*)val, c.element_size()*sizeof(double), digest);
    std::cout<<"digest of: "<<i<<" is: "<<std::hex<<std::setw(2)<<std::setfill('0');
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        std::cout << (int)digest[i];
    }
    std::cout<<std::dec<<std::endl;

    b.release_element(i);
  }

  }
  MPI_Barrier(MPI_COMM_WORLD);
}

