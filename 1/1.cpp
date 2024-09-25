#include <array>
#include <iostream>
#include <cmath>
template <int Size>
constexpr int CalculateDeterminant(const std::array<std::array<int, Size>, Size> &mat, int currentSize, int M, int detValue = 0, int signMultiplier = 1)
{
    int currentColumn = M;
    while (currentColumn){
        detValue += CalculateDeterminant<Size>(mat, currentSize - 1, M - (currentColumn & -currentColumn)) * signMultiplier * mat[Size - currentSize][__builtin_ctz(currentColumn & -currentColumn)];
        signMultiplier = -signMultiplier;
        currentColumn &= (currentColumn - 1);
    }
    return (currentSize == 1) ? mat[Size - 1][__builtin_ctz(M)] : (Size % 2 == 0 ? -detValue : detValue);
}

template <int N>
constexpr int Det(const std::array<std::array<int, N>, N> &matrix)
{
    return CalculateDeterminant<N>(matrix, N, static_cast<int>(pow(2, N)) - 1);
}

int main()
{
    constexpr std::array<std::array<int, 3>, 3> matrix = {{{0, 1, 2},
                                                           {1, 2, 3},
                                                           {2, 3, 7}}};

    constexpr int result = Det<3>(matrix);
    std::cout << result << std::endl;
    return 0;
}