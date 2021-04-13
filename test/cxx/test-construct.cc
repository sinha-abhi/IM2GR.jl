#define BOOST_TEST_MODULE im2gr-construct

#include <boost/test/included/unit_test.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

#include "im2gr/diff-func.h"
#include "im2gr/im2gr.h"

float norm(const float *x, size_t s) {
  return (float) std::sqrt(std::inner_product(x, x+s, x, 0.0));
}

BOOST_AUTO_TEST_SUITE( singlethread )

BOOST_AUTO_TEST_CASE( test_small_1 ) {
  Data *data = new Data("data/simple1.txt");
  auto image = im2gr(data, 1, SingleThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 56 );
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 13.856406, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 11.313708, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_CASE( test_small_2 ) {
  Data *data = new Data("data/simple2.txt");
  auto image = im2gr(data, 1, SingleThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 334 );
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 36.083237, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 367031.792222, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_CASE( test_small_2_d2 ) {
  Data *data = new Data("data/simple2.txt");
  auto image = im2gr(data, 2, SingleThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 654);
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 109.352641, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 479834.242936, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( multithread )

BOOST_AUTO_TEST_CASE( test_small_1 ) {
  Data *data = new Data("data/simple1.txt");
  auto image = im2gr(data, 1, MultiThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 56 );
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 13.856406, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 11.313708, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_CASE( test_small_2 ) {
  Data *data = new Data("data/simple2.txt");
  auto image = im2gr(data, 1, MultiThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 334 );
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 36.083237, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 367031.792222, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_CASE( test_small_2_d2 ) {
  Data *data = new Data("data/simple2.txt");
  auto image = im2gr(data, 2, MultiThread, default_diff_func);

  size_t vc = image.get_vc();
  BOOST_CHECK( vc == 654);
  BOOST_CHECK_CLOSE( norm(image.get_evd(), vc), 109.352641, 0.00001 );
  BOOST_CHECK_CLOSE( norm(image.get_evi(), vc), 479834.242936, 0.00001 );

  delete data;
}

BOOST_AUTO_TEST_SUITE_END()
