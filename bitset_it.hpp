#ifndef BITSET_IT_HPP
#define BITSET_IT_HPP

#include <bitset>
#include <iterator>
#include <type_traits>

#include <cstdint>
#include <cstddef>

#include <cassert>  // assert
#include <climits>  // climits

namespace iterators {

template < size_t N >
class bitset_it;

}

namespace std {
    template < size_t N >
    struct iterator_traits<iterators::bitset_it<N>> {
        static_assert( N<= 64, "Unsupported bitset size" );

        using value_type =
            conditional_t< N<= 8,  std::uint_fast8_t,
            conditional_t< N<=16, std::uint_fast16_t,
            conditional_t< N<=32, std::uint_fast32_t,
                                  std::uint_fast64_t>>>;

        using difference_type =
            conditional_t< N<= 8,  std::uint_fast8_t,
            conditional_t< N<=16, std::uint_fast16_t,
            conditional_t< N<=32, std::uint_fast32_t,
                                  std::uint_fast64_t>>>;

        using iterator_category = std::forward_iterator_tag;
    };
}

namespace detail {
    // For values fitting in ulong
    inline int lsb_set( unsigned long value ) {
        assert( value > 0 ); // Assuming at least one bit is set
        return __builtin_ffsl(value) -1;
    }

    // For values not fitting in ulong but in ulonglong instead
    inline int lsb_set( unsigned long long value ) {
        assert( value > 0 ); // Assuming at least one bit is set
        return __builtin_ffsll(value) -1;
    }

    template < size_t N >
    inline std::enable_if_t<N<=CHAR_BIT*sizeof(unsigned long),unsigned long>
    int_value( const std::bitset<N>& value ) {
        return value.to_ulong();
    }

    template < size_t N >
    inline std::enable_if_t<(N> CHAR_BIT*sizeof(unsigned long)),unsigned long>
    int_value( const std::bitset<N>& value ) {
        return value.to_ullong();
    }
}

namespace iterators {

template < size_t N >
class bitset_it {
public:
    using value_type = typename std::iterator_traits<bitset_it>::value_type;

    constexpr bitset_it() noexcept = default;

    constexpr bitset_it( const std::bitset<N>& init ) noexcept :
        _remainder(detail::int_value(init))
    {
    }

    constexpr bitset_it( const bitset_it& ) noexcept = default;

    ~bitset_it() noexcept = default;

    bitset_it& operator++() noexcept {
        unset_lsb();
        return *this;
    }

    bitset_it operator++(int) noexcept {
        bitset_it old(*this);
        unset_lsb();
        return old;
    }

    bool operator!=( const bitset_it& other ) const noexcept {
        return _remainder != other._remainder;
    }

    value_type operator*() const noexcept {
        return detail::lsb_set(_remainder);
    }

private:
    void unset_lsb() {
        _remainder &= _remainder - 1;
    }
    
    value_type _remainder;
};
        
} // namespace iterators

namespace std {
    template < size_t N >
    iterators::bitset_it<N> begin( const std::bitset<N>& value ) noexcept {
        return iterators::bitset_it<N>(value);
    }

    template < size_t N >
    iterators::bitset_it<N> end( const std::bitset<N>& ) noexcept {
        return iterators::bitset_it<N>();
    }
}

#endif // BITSET_IT_HPP
