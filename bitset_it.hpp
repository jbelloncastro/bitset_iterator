
#ifndef BITSET_IT_HPP
#define BITSET_IT_HPP

#include <bitset>   // bitset
#include <iterator> // iterator_traits

#include <cassert>  // assert
#include <climits>  // CHAR_BIT

namespace traits {
    // Always fits in integer type
    template < class T, class Int >
    struct always_fits;

    template < size_t N, class Int >
    struct always_fits< std::bitset<N>,Int> : std::integral_constant<bool, N <= CHAR_BIT*sizeof(Int)>
    {
    };

    template < class T >
    struct best_fit_int;

    template < size_t N >
    struct best_fit_int< std::bitset<N> > {
        static_assert( traits::always_fits<std::bitset<N>, unsigned long long>::value );

        typedef typename std::conditional<
                N < CHAR_BIT*sizeof(unsigned long),
                unsigned long,
                unsigned long long
            >::type unsigned_type;

        typedef typename std::conditional<
                N < CHAR_BIT*sizeof(long),
                long,
                long long
            >::type signed_type;
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
    inline std::enable_if_t<N < CHAR_BIT*sizeof(unsigned long), unsigned long>
    int_value( const std::bitset<N>& value ) {
        return value.to_ulong();
    }

    template < size_t N >
    inline std::enable_if_t<CHAR_BIT*sizeof(unsigned long) < N 
                       && N < CHAR_BIT*sizeof(unsigned long long),
                     unsigned long long>
    int_value( const std::bitset<N>& value ) {
        return value.to_ullong();
    }
}

namespace iterators {

template < size_t N >
class bitset_it {
public:
    typedef typename traits::best_fit_int<std::bitset<N>>::unsigned_type value_type;
    typedef typename traits::best_fit_int<std::bitset<N>>::signed_type   difference_type;

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
        _remainder -= value_type(1)<<detail::lsb_set(_remainder);
    }
    
    value_type _remainder;
};
        
} // namespace iterators

namespace std {
    template < size_t N >
    struct iterator_traits<bitset_it<N>> {
        typedef typename bitset_it<N>::value_type      value_type;
        typedef typename bitset_it<N>::difference_type difference_type;
        typedef std::forward_iterator_tag              iterator_category;
    };

    template < size_t N >
    bitset_it<N> begin( const std::bitset<N>& value ) noexcept {
        return bitset_it<N>(value);
    }

    template < size_t N >
    bitset_it<N> end( const std::bitset<N>& ) noexcept {
        return bitset_it<N>();
    }
}

#endif // BITSET_IT_HPP
