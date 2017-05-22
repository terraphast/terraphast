
#include <catch.hpp>

#include <terraces/bitmatrix.hpp>

namespace terraces {
namespace tests {

TEST_CASE("bitmatrix-construction", "[bitmatrix]") {
	auto mat = bitmatrix{10, 5};
	CHECK(mat.rows() == 10);
	CHECK(mat.cols() == 5);
}

TEST_CASE("bitmatrix set/get", "[bitmatrix]") {
	auto mat = bitmatrix{3, 4};

	CHECK(mat.get(1, 2) == false);
	mat.set(1, 2, true);
	CHECK(mat.get(1, 2) == true);
}

TEST_CASE("bitmatrix/copy_row", "[bitmatrix]") {
	auto m1 = bitmatrix{2, 2};
	auto m2 = bitmatrix{1, 2};
	m1.set(1, 1, true);
	m1.set(1, 0, true);
	m2.copy_row(1, 0, m1);
	CHECK(m2.get(0, 0) == true);
	CHECK(m2.get(0, 1) == true);
}

} // namespace tests
} // namespace terraces
