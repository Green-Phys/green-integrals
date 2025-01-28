#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "green/integrals/buffered_reader/buffer.hpp"
#include "green/integrals/buffered_reader/chunk_reader.hpp"
#include <mpi.h>
#include <filesystem>


// For these tests of reading integrals as chunks, we use cooked up density-fitted integrals.
// system           :   Silicon
// k-mesh           :   2x2x2
// basis            :   gth-dzvp-molopt-sr
// pseudo           :   gth-pbe
// size per file    :   1MB
// beta (df params) :   2.0
// diffuse_cutoff   :   0.3

using namespace std::string_literals;

TEST_CASE("Init","[ReadingSI]") {

  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path = TEST_PATH + "/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}

  chunk_reader c(df_path, number_of_keys, naux, nao, true); //test these numbers
  buffer b(c.element_size(), number_of_keys, 1000, &c);

  const double* val=b.access_element(0);
  REQUIRE_THAT(val[0], Catch::Matchers::WithinAbs(1.5409454, 1.e-5));
  b.release_element(0);
}

TEST_CASE("ReadAllIntsConsecutively","[ReadingSI]") {
  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path = TEST_PATH + "/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}
  chunk_reader c(df_path, number_of_keys, naux, nao); //test these numbers
  buffer b(c.element_size(), number_of_keys, 1000, &c, 2);

  for(int i=b.shmem_rank();i<number_of_keys;i+=b.shmem_size()){
    if(i>=number_of_keys) break;
    const double* val=b.access_element(i);
    b.release_element(i);
  }
}

TEST_CASE("ReadAllIntsSmallBuffer","[ReadingSI]") {
  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  int n_buffered_elem=100;
  std::string df_path = TEST_PATH + "/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}
  chunk_reader c(df_path, number_of_keys, naux, nao,true); //test these numbers
  buffer b(c.element_size(), number_of_keys, n_buffered_elem, &c);

  for(int i=b.shmem_rank();i<chunks_per_file*total_files;i+=b.shmem_size()){
    if(i>=chunks_per_file*total_files) break;
    const double* val=b.access_element(i);
    b.release_element(i);
  }
}

TEST_CASE("ReadAllIntsConsecutivelyLargeStride","[ReadingSI]") {
  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path = TEST_PATH + "/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}
  chunk_reader c(df_path, number_of_keys, naux, nao,true); //test these numbers
  buffer b(c.element_size(), number_of_keys, number_of_keys, &c, 0, false);

  int stride=number_of_keys/b.shmem_size();
  int start=number_of_keys/b.shmem_size()*b.shmem_rank();
  int stop=std::min(number_of_keys/b.shmem_size()*(b.shmem_rank()+1), number_of_keys);
  for(int i=start;i<stop;++i){
    const double* val=b.access_element(i);
    b.release_element(i);
  }
}
