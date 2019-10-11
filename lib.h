#pragma once 

#include <type_traits>
#include <iostream>
#include <iomanip>
#include <string>

using std::string;

template <int N>
struct Int : std::integral_constant<int, N> 
{
    using type = Int;
    using mul2 = Int<N*2>;

    static void print() { std::cout << std::setw(5) << N; }
    static std::string toString() {  return "_"+ std::to_string(N); }
};
using _0 = Int<0>;
using _2 = Int<2>;
using _4 = Int<4>;
using _8 = Int<8>;
using _16 = Int<16>;
using _32 = Int<32>;
using _64 = Int<64>;
using _128 = Int<128>;
using _256 = Int<256>;
using _512 = Int<512>;
using _1024 = Int<1024>;
using _2048 = Int<2048>;

template <typename...>
struct List {};
template <>
struct List<>
{
    using type = List;
    static void print() { std::cout << std::endl; }
    static std::string toString() { return ""; }
};
template <typename Head, typename... Tail>
struct List<Head, Tail...>
{
    using type = List;
    static void print() 
    { 
        Head::print();
        List<Tail...>::print();
    }
};

template <typename, typename>
struct Concat {};
template <typename... Elems1, typename... Elems2>
struct Concat<List<Elems1...>, List<Elems2...>> : List<Elems1..., Elems2...> {};

template <typename>
struct Length {};
template <typename... Elems>
struct Length<List<Elems...>> : std::integral_constant<int, sizeof...(Elems)> {};

template <typename, typename>
struct AddToFront {};
template <typename Elem, typename... Elems>
struct AddToFront<Elem, List<Elems...>> : List<Elem, Elems...> {};
template <typename, typename>
struct AddToEnd {};
template <typename Elem, typename... Elems>
struct AddToEnd<List<Elems...>, Elem> : List<Elems..., Elem> {};

template <typename>
struct Reverse {};
template <>
struct Reverse<List<>> : List<> {};
template <typename Head, typename... Tail>
struct Reverse<List<Head, Tail...>> : AddToEnd<typename Reverse<List<Tail...>>::type, Head>::type {};

template <int N, int Elem> 
struct Replicate : AddToFront<Int<Elem>, typename Replicate<(N-1), Elem>::type>::type {};
template <int Elem>
struct Replicate<0, Elem> : List<> {};

template <template<typename> class, typename>
struct Filter {};
template <template <typename> class F, typename Head, typename... Args>
struct Filter<F, List<Head, Args...>>
{
    using type = std::conditional_t< F<Head>::value, 
                                typename AddToFront<Head, typename Filter<F, List<Args...>>::type>::type, 
                                typename Filter<F, List<Args...>>::type>;
};
template <template <typename> class F>
struct Filter<F, List<>> : List<> {};

template <template<typename> class, typename>
struct Map {};
template <typename... Elems, template<typename> class F>
struct Map<F, List<Elems...>> : List<typename F<Elems>::type...> {};

template <typename>
struct Head {};
template <typename _Head, typename... Tail>
struct Head<List<_Head, Tail...>> :  _Head {};

template <typename>
struct Tail {};
template <typename Head, typename... _Tail>
struct Tail<List<Head, _Tail...>> : List<_Tail...> {};

template <typename Matrix>
struct Transpose 
{
    using head = typename Map<Head, Matrix>::type;
    using tail = typename Map<Tail, Matrix>::type;
    using type = typename AddToFront<head, typename Transpose<tail>::type>::type;
}; 
template <typename _Head, typename... Ls>
struct Transpose<List<List<_Head>, Ls...>> : List<List<_Head, typename Head<Ls>::type...>> {};

template <int N, typename List>
struct Get : Get<N-1, typename Tail<List>::type>::type {};
template <typename Head, typename... Tail>
struct Get<0, List<Head, Tail...>> : Head {};

template <int N, typename Elem, typename List>
struct Set {};
template <int N, typename Elem, typename Head, typename... Tail>
struct Set<N, Elem, List<Head, Tail...>> : AddToFront<Head, typename Set<N-1, Elem, List<Tail...>>::type>::type {};
template <typename Elem, typename Head, typename... Tail>
struct Set<0, Elem, List<Head, Tail...>> : AddToFront<Elem, List<Tail...>>::type {};

template <int A, int B, int C, int D>
static std::string serialize(List<Int<A>, Int<B>, Int<C>, Int<D>>)
{
    return "List<"+Int<A>::toString()+", "+Int<B>::toString()+", "+Int<C>::toString()+", "+Int<D>::toString()+">";
}
template <typename A, typename B, typename C, typename D>//, template <int> typename... B, template <int> typename... C, template <int> typename... D>
static std::string serialize(List<A, B, C, D>)
{
    return "using Grid = List<" + serialize(A{}) + ", " + serialize(B{}) +", "+ serialize(C{}) + ", "+ serialize(D{}) + ">;";
}