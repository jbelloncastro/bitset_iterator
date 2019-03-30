
#ifndef BITSET_IT_HPP
#define BITSET_IT_HPP

#include <bitset>   // bitset
#include <iterator> // iterator_traits
#include <limits>   // numeric_limits

#include <cassert>  // assert

namespace util {

template < size_t N >
class bitset_iterator {
        static_assert( N <= 8*sizeof(unsigned long long), "Unsupported bitset capacity" );
    public:

        bitset_iterator() :
            _value(0ULL)
        {
        }

        explicit bitset_iterator( const std::bitset<N>& value ) :
            _value(value.to_ullong())
        {
        }

        bitset_iterator( const bitset_iterator& other ) :
            _value(other._value)
        {
        }

        // Pre-increment
        bitset_iterator& operator++() {
            unset( lsb_set() );
            return *this;
        }

        // Post-increment
        bitset_iterator operator++(int) {
            bitset_iterator it(*this);
            ++(*this);
            return it;
        }

        unsigned operator*() const {
            return lsb_set();
        }

        bool operator!= ( const bitset_iterator& other ) const {
            return _value != other._value;
        }

    private:
        unsigned lsb_set() const {
            assert( _value > 0 );
            return __builtin_ffsll(_value) - 1;
        }

        void unset( unsigned position ) {
            assert( position < std::numeric_limits<unsigned long long>::max() );
            unsigned long long comp = 1ULL << position;
            _value &= ~comp;
        }

        unsigned long long _value;
};

} // namespace util

namespace std {

template < size_t N >
struct iterator_traits< util::bitset_iterator<N> > {
    typedef unsigned                  value_type;
    typedef int                       difference_type;
    typedef std::forward_iterator_tag iterator_category;
};

template < size_t N >
util::bitset_iterator<N> begin( const std::bitset<N>& bs ) {
    return util::bitset_iterator<N>(bs);
}

template < size_t N >
util::bitset_iterator<N> end( const std::bitset<N>& /* unused */ ) {
    return util::bitset_iterator<N>();
}

} // namespace std

#endif // BITSET_IT_HPP
