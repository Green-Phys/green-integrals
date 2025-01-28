#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "green/integrals/buffered_reader/chunk_reader.hpp"
#include <mpi.h>
#include <chrono>
#include <thread>
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

TEST_CASE("ReadFakeData","[reader]") {

  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;

  reader c("no_such_file", number_of_keys, naux, nao); //test these numbers

  Eigen::VectorXd buffer(naux*nao*nao*2);
  c.read_key(0, &(buffer[0]));

  REQUIRE(buffer[0]==42);
  REQUIRE(buffer[naux]== 42.);
  REQUIRE(buffer[naux*nao*nao*2-1]==42.);
}

TEST_CASE("Init","[chunk_reader]") {

  chunk_reader c;
}

TEST_CASE("InitBasePath","[chunk_reader]") {

  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path=TEST_PATH+"/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}

  chunk_reader c(df_path, number_of_keys, naux, nao); //test these numbers

  REQUIRE(c.chunk_indices()[0]==0);
  REQUIRE(c.chunk_indices()[1]==18);
}

TEST_CASE("ReadSomething","[chunk_reader]") {
  int chunks_per_file=18;
  int total_files=2;
  int nao=8;
  int naux=36;
  int number_of_keys=chunks_per_file*total_files;
  std::string df_path=TEST_PATH+"/Si/df_int"s;

  if(!std::filesystem::exists(df_path)){ std::cerr<<"hdf5 data not found. aborting test"<<std::endl; return;}
  chunk_reader c(df_path, number_of_keys, naux, nao,true); //test these numbers

  Eigen::VectorXd data(c.element_size());
  c.read_key(0, &(data[0]));
  REQUIRE_THAT(data[0], Catch::Matchers::WithinAbs(1.5409454, 1.e-5));
  c.read_key(17, &(data[0]));
  c.read_key(18, &(data[0]));
}
