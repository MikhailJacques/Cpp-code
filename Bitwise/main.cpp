#include <iostream>
using namespace std;

// https://www.geeksforgeeks.org/bitwise-operators-in-c-cpp/

void swap(int* xp, int* yp);
int findOdd(int arr[], int n);

int main() 
{
    // a = 5 (0000 0101), b = 9 (0000 1001)
    int a = 5, b = 9;

    // The result is 0000 0001 = 1
    cout << "a = " << a << "," << " b = " << b << endl;
    cout << "a & b = " << (a & b) << endl;

    // The result is 0000 1101 = 13
    cout << "a | b = " << (a | b) << endl;

    // The result is 0000 1100 = 12
    cout << "a ^ b = " << (a ^ b) << endl;

    // The result is 1111 1010 = -6
    cout << "~a = " << (~a) << endl;

    // 2's complement
    // We use 2's complement to calculate the binary of negative integers
    // The 2's complement of a number N gives -N.
    // In binary arithmetic, 1's complement changes 0 to 1 and 1 to 0.
    // And, if we add 1 to the result of the 1's complement, we get the 2's complement of the original number.
    // 
    // https://www.omnicalculator.com/math/twos-complement
    // 1. Reverse digits: ~1111 1010 = 0000 0101
    // 2. Add a unity: 0000 0101 + 1 = 0000 0110
    // 3. Convert to a decimal (starting from the right): 0000 0110 = 0·2⁰ + 1·2¹ + 1·2² + 0·2³ + 0·2⁴ + 0·2⁵ + 0·2⁶ + 0·2⁷ = 2 + 4 = 6
    // 4. As 6 is the absolute value of the initial (negative) binary, add a minus sign in front of it.
    // 5. 1111 1010 is -6 in two's complement binary notation.
    // 
    // 6. 1111 1010 in the two's complement representation is -6 in decimal notation and is the 2's complement of 0000 0110 (6)
    // 7. ~0000 0110 (6) = 1111 1001, 1111 1001 + 1 = 1111 1010 = -6
 
    // Turning two's complement to decimal
    // Convert this signed binary (1111 1010) into a decimal, like normal, but multiply the leading digit by -1 instead of 1, starting from the right
    // decimal = 0·2⁰ + 1·2¹ + 0·2² + 1·2³ + 1·2⁴ + 1·2⁵ + 1·2⁶ - 1·2⁷ = 0 + 2 + 0 + 8 + 16 + 32 + 64 - 128 = 6

    // The bitwise complement of 35 = -(35 + 1) = -36 i.e. ~35 = -36
    int num1 = 35;
    cout << "~(" << num1 << ") = " << (~num1) << endl;

    // The bitwise complement of -150 = -(-150 + 1) = -(-149) = 149 i.e. ~(-150) = 149
    int num2 = -150;
    cout << "~(" << num2 << ") = " << (~num2) << endl;

    // The result is 00010010
    cout << "b << 1" << " = " << (b << 1) << endl;

    // The result is 00000100
    cout << "b >> 1 " << "= " << (b >> 1) << endl;

    int arr[] = { 12, 12, 14, 90, 14, 14, 14 };
    int n = sizeof(arr) / sizeof(arr[0]);
    cout << "\nThe odd occurring element is  " << findOdd(arr, n) << "\n\n";

    // The right shift operator shifts all bits towards the right by a certain number of specified bits. It is denoted by >>.
    // When we shift any number to the right, the least significant bits are discarded, while the most significant bits are replaced by zeroes.

    // The left shift operator shifts all bits towards the left by a certain number of specified bits. It is denoted by <<.
    // When we shift any number to the left, the most significant bits are discarded, while the least significant bits are replaced by zeroes.

    int num = 212, i;

    // Shift Right Operation
    cout << "Shift right:" << "\n";

    // Using for loop for shifting num right from 0 bit to 3 bits 
    for (i = 0; i < 4; i++) 
    {
        cout << "212 >> " << i << " = " << (212 >> i) << "\n";
    }

    // Shift Left Operation
    cout << "\nShift left:" << "\n";

    // Using for loop for shifting num left from 0 bit to 3 bits
    for (i = 0; i < 4; i++) 
    {
        cout << "212 << " << i << " = " << (212 << i) << "\n";
    }

    // https://www.geeksforgeeks.org/swap-two-numbers-without-using-temporary-variable/
    // Swap two numbers without using a temporary variable using Bitwise XOR
    // The bitwise XOR operator can be used to swap two variables. 
    // The XOR of two numbers x and y returns a number that has all the bits as 1 wherever bits of x and y differ. 
    // For example, XOR of 10 (in binary 1010) and 5 (in binary 0101) is 1111, and XOR of 7 (0111) and 5 (0101) is (0010). 
    int x = 10, y = 5;

    cout << "\nBefore swapping: x=" << x << ", y=" << y;

    // Code to swap 'x' (1010) and 'y' (0101)
    x = x ^ y;  // x becomes 15 (1111)
    y = x ^ y;  // y becomes 10 (1010)
    x = x ^ y;  // x becomes 5 (0101)

    cout << "\nAfter swapping: x=" << x << ", y=" << y << "\n";

    x = 10, y = 5;

    cout << "\nBefore swapping: x=" << x << ", y=" << y;
    swap(&x, &y);
    cout << "\nAfter swapping: x=" << x << ", y=" << y << "\n";

    return 0;
}

// Function to return the only odd occurring element
int findOdd(int arr[], int n)
{
    int res = 0;

    for (int i = 0; i < n; i++)
    {
        res ^= arr[i];
    }

    return res;
}

void swap(int* xp, int* yp)
{
    // Check if the two addresses are same
    if (xp == yp)
        return;

    *xp = *xp ^ *yp;  // x becomes 15 (1111)
    *yp = *xp ^ *yp;  // y becomes 10 (1010)
    *xp = *xp ^ *yp;  // x becomes 5 (0101)
}