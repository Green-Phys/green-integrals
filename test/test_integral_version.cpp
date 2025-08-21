/*
 * Copyright (c) 2023 University of Michigan
 *
 */

#include <green/symmetry/symmetry.h>
#include "green/integrals/df_integral_t.h"

#include <catch2/catch_session.hpp>
#include <catch2/catch_test_macros.hpp>
#include "catch2/matchers/catch_matchers.hpp"


using green::integrals::df_integral_t;

TEST_CASE("Integral version", "[Input]") {
  // SKIP("This test is disabled until all integral data and throw statements are updated");
  auto        p             = green::params::params("DESCR");
  std::string input_file    = TEST_PATH + "/Input/input.h5"s;
  std::string df_int_path_1 = TEST_PATH + "/Input/df_int"s;
  std::string df_int_path_2 = TEST_PATH + "/Input/df_int_x"s;
  std::string df_int_path_3 = TEST_PATH + "/Input/df_int_y"s;
  std::string df_int_path_4 = TEST_PATH + "/Input/df_int_0.3.0"s;
  std::string args =
      "test --restart 0 --itermax 2 --E_thr 1e-13 --mixing_type G_MIXING --mixing_weight 0.8 --input_file=" + input_file +
      " --BETA 100 --verbose=1 --dfintegral_file=" + df_int_path_1 +
      " --dfintegral_hf_file=" + df_int_path_2;
  green::symmetry::define_parameters(p);
  p.parse(args);
  int verbose = 1;
  green::symmetry::brillouin_zone_utils<green::symmetry::inv_symm_op> bz_utils(p);
  REQUIRE_THROWS_AS(df_integral_t(df_int_path_1, 2, 36, bz_utils, verbose), green::integrals::integrals_outdated_input);
  REQUIRE_THROWS_AS(df_integral_t(df_int_path_2, 2, 36, bz_utils, verbose), green::integrals::integrals_outdated_input);
  REQUIRE_NOTHROW(df_integral_t(df_int_path_3, 2, 36, bz_utils, verbose));
  REQUIRE_NOTHROW(df_integral_t(df_int_path_4, 2, 36, bz_utils, verbose));
}