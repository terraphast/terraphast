#include <catch.hpp>

#include <iostream>

#include <terraces/parser.hpp>
#include <terraces/rooting.hpp>

#include "../lib/validation.hpp"

namespace terraces {
namespace tests {

TEST_CASE("is_isomorphic (simple true)", "[validation]") {
	auto fst = parse_nwk("(1,(2,3));");
	reroot_inplace(fst.tree, fst.indices.at("1"));
	auto snd = parse_nwk("(2,(1,3));");
	reroot_inplace(snd.tree, snd.indices.at("3"));
	CHECK(is_isomorphic(fst, snd));
}

TEST_CASE("is_isomorphic (simple false)", "[validation]") {
	auto fst = parse_nwk("(1,(2,(3,(4,5))));");
	reroot_inplace(fst.tree, fst.indices.at("1"));
	auto snd = parse_nwk("(2,((1,4),(3,5)));");
	reroot_inplace(snd.tree, snd.indices.at("3"));
	CHECK(!is_isomorphic(fst, snd));
}

TEST_CASE("is_isomorphic (complex)", "[validation]") {
	//
	auto fst = parse_nwk("((((s2,s4),((s13,s1),s7)),s3),s5);");
	reroot_inplace(fst.tree, fst.indices.at("s2"));
	auto snd = parse_nwk("((s13,((s2,s7),(s4,(s5,s3)))),s1);");
	reroot_inplace(snd.tree, snd.indices.at("s7"));
	auto trd = parse_nwk("((s13,(s7,((s4,s2),(s5,s3)))),s1);");
	reroot_inplace(trd.tree, trd.indices.at("s13"));
	auto fth = parse_nwk("((s13,(((s3,s5),s4),(s2,s7))),s1);");
	reroot_inplace(fth.tree, fth.indices.at("s5"));
	CHECK(!is_isomorphic(fst, snd));
	CHECK(is_isomorphic(fst, trd));
	CHECK(!is_isomorphic(snd, trd));
	CHECK(is_isomorphic(snd, fth));
	CHECK(!is_isomorphic(fst, fth));
}

} // namespace tests
} // namespace terraces
