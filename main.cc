#include "lib.h"
#include "grid.h"
#include <iostream>
#include <fstream>
#include <utility>

using namespace std;

template <typename Arg>
struct NotZero : integral_constant<bool, Arg::value != 0> {};

template <typename>
struct ConcatAll {};
template <typename Head, typename... Tail>
struct ConcatAll<List<Head, Tail...>> : Concat<Head, typename ConcatAll<List<Tail...>>::type>::type {};
template <>
struct ConcatAll<List<>> : List<> {};

template <typename Matrix>
struct Check2048 
{
    template <typename _Int>
    struct Is2048 : is_same<_Int, Int<2048>> {};
    
    using zeroTiles = typename Filter<Is2048, typename ConcatAll<Matrix>::type>::type;
    constexpr static bool value = Length<zeroTiles>::value > 0;
};

template <typename L>
struct Merge
{
    template <typename Head>
    struct Combine : Head {};
    template <typename Head, typename Second, typename... Tail>
    struct Combine<List<Head, Second, Tail...>>
    {
        using type = std::conditional_t< Head::value == Second::value,
                                         typename AddToFront<typename Head::mul2, typename Combine<List<Tail...>>::type>::type, 
                                         typename AddToFront<Head, typename Combine<List<Second, Tail...>>::type>::type >;
    };

    using merged = typename Combine<typename Filter<NotZero, L>::type>::type; 
    using padding =  typename Replicate<Length<L>::value - Length<merged>::value, 0>::type; 
    using type = typename Concat<merged, padding>::type;
    
};

template <typename G>
struct MoveLeft : Map<Merge, G>::type {};
template <typename G>
struct MoveRight : Map<Reverse, typename Map<Merge, typename Map<Reverse, G>::type>::type>::type {};
template <typename G>
struct MoveUp : Transpose<typename MoveLeft<typename Transpose<G>::type>::type>::type {};
template <typename G>
struct MoveDown : Transpose<typename MoveRight<typename Transpose<G>::type>::type>::type {};

template <typename G, int X, int Y, typename Elem>
struct SetNumber : Set<Y, typename Set<X, Elem, typename Get<Y, G>::type>::type, G>::type {};

template <typename G, int X, int Y>
struct GetNumber : Get<X, typename Get<Y, G>::type>::type {};

    template <int X, int Y>
    struct Pos 
    {  
        using type = Pos;
        constexpr static int x = X, y = Y; 
    };

template <typename G>
struct GetZeros 
{


    template <typename Pos>
    struct IsZero 
    {
        constexpr static bool value = is_same_v<typename GetNumber<G, Pos::x, Pos::y>::type, Int<0>>;
    };

    using coordinates = List<   Pos<0, 0>, Pos<0, 1>, Pos<0, 2>, Pos<0, 3>,
                                Pos<1, 0>, Pos<1, 1>, Pos<1, 2>, Pos<1, 3>,
                                Pos<2, 0>, Pos<2, 1>, Pos<2, 2>, Pos<2, 3>,
                                Pos<3, 0>, Pos<3, 1>, Pos<3, 2>, Pos<3, 3>>;
    
    using type = typename Filter<IsZero, coordinates>::type;
};

template <typename G>
struct AddTile
{
    using zeros = typename GetZeros<G>::type;
    using toAdd = typename Get<RANDOM % Length<zeros>::value, zeros>::type;
    constexpr static int x = toAdd::x, y = toAdd::y;

    using type = typename SetNumber<G, x, y, Int<2>>::type; 
};

template <typename G>
struct CheckFail
{
    using moveLeft = typename GetZeros<typename MoveLeft<G>::type>::type;
    using moveRight = typename GetZeros<typename MoveRight<G>::type>::type;
    using moveUp = typename GetZeros<typename MoveUp<G>::type>::type;
    using moveDown = typename GetZeros<typename MoveDown<G>::type>::type;

    constexpr static bool value = (Length<moveLeft>::value + Length<moveRight>::value + Length<moveUp>::value + Length<moveDown>::value)<=0;
};

int main()
{
#ifdef LEFT
    using moved = MoveLeft<Grid>::type;
#elif RIGHT
    using moved = MoveRight<Grid>::type;
#elif UP
    using moved = MoveUp<Grid>::type;
#elif DOWN
    using moved = MoveDown<Grid>::type;
#else 
    using moved = List<Replicate<4, 0>::type, Replicate<4, 0>::type, Replicate<4, 0>::type, Replicate<4, 0>::type>;
#endif

    bool fail = false;

    if (!is_same_v<moved, Grid>) 
    {    
        using added = AddTile<moved>::type;
        fstream f("./grid.h", ios::out);
        f << serialize(added{}) << endl;
        f.close();
        added::print();
        fail = CheckFail<added>::value; 
    }
    else 
        moved::print();
    
    bool win = Check2048<moved>::value;

    if (win)
        cout << "You won! Enter \'make start\' to restart." << endl;
    else if (fail)
        cout << "Game over! Enter \'make start\' to restart." << endl;
}