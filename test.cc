#include "lib.h"
#include <type_traits>
#include <iostream>

using namespace std;

template <typename Arg>
struct NotZero : integral_constant<bool, Arg::value != 0> {};

template <typename Arg>
struct Mul2 : Int<Arg::value * 2> {};

int main()
{
    using grid = List<List<_0,_0,_0,_0>, List<_0,_0,_0,_0>, List<_0,_0,_0,_0>, List<_0,_0,_0,_0>>;
    cout << typeid(grid).name() << endl;

    grid::print();

    using concat = Concat<List<_0, _2>, List<_4, _8>>::type;
    cout << typeid(concat).name() << endl;

    using add = AddToFront<_16, concat>::type;
    cout << typeid(add).name() << endl;

    cout << Length<grid>::value << endl;
    cout << Length<add>::value << endl;

    using reverse = Reverse<add>::type;
    cout << typeid(reverse).name() << endl;

    using replicate = Replicate<2, 3>::type;
    cout << typeid(replicate).name() << endl;

    using filter = Filter<NotZero, add>::type;
    cout << typeid(filter).name() << endl;

    using map = Map<Mul2, concat>::type;
    cout << typeid(map).name() << endl;

    using head = Head<map>::type;
    cout << typeid(head).name() << endl;
    using tail = Tail<map>::type;
    cout << typeid(tail).name() << endl;

    using g = List<List<_0,_2,_2,_0>, List<_4,_2,_0,_2>, List<_8,_4,_2,_8>, List<_0,_0,_0,_0>>;
    using transpose = Transpose<g>::type;
    cout << typeid(transpose).name() << endl;
    transpose::print();

    using get = Get<3, concat>::type;
    cout << typeid(get).name() << endl;

    cout << typeid(concat).name() << endl;
    concat::print();
    using set = Set<1, Int<6>, concat>::type;
    set::print();

    transpose::print();
    cout << serialize(set{}) << endl;
    cout << serialize(transpose{}) << endl;
}