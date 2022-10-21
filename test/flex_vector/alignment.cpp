#include <catch.hpp>
#include <cstdint>
#include <immer/flex_vector.hpp>

// A structure with the same alignment requirements as Eigen's vectorized types
// (which is what originally exposed this issue)
struct AlignConstrainedType {
    alignas(16) double buf[4] = {0, 0, 0, 0};

    bool is_aligned() const {
        // https://stackoverflow.com/a/42093940
        const auto rem = reinterpret_cast<intptr_t>(this) % 16;
        return !rem;
    }
};


constexpr int N = 4; // just in case alignemnt issues only show up for the non-first element

TEST_CASE("Sanity check: vector")
{
    std::vector<AlignConstrainedType> v;
    for (int i = 0; i < N; ++i) {
        v.emplace_back();
    }

    for (int i = 0; i < N; ++i) {
        CHECK(v[i].is_aligned());
    }
}

TEST_CASE("direct alignment")
{
    immer::flex_vector<AlignConstrainedType> v;

    for (int i = 0; i < N; ++i) {
        v = std::move(v).push_back({});
    }

    for (int i = 0; i < N; ++i) {
        CHECK(v[i].is_aligned());
    }
}
