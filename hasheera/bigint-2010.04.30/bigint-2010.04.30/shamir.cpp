#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

// Include the BigInteger library you downloaded
#include "BigIntegerLibrary.hh"

// A structure to hold our decoded points (shares)
struct Point {
    BigInteger x;
    BigInteger y;
};

// Converts a string representation of a number from any base (2-36) to a BigInteger
BigInteger baseToDec(const std::string& value, int base) {
    BigInteger result = 0;
    BigInteger power = 1;

    for (int i = value.length() - 1; i >= 0; i--) {
        int digit;
        if (value[i] >= '0' && value[i] <= '9') {
            digit = value[i] - '0';
        } else if (value[i] >= 'a' && value[i] <= 'z') {
            digit = value[i] - 'a' + 10;
        } else {
            throw std::invalid_argument("Invalid character in number string");
        }

        if (digit >= base) {
            throw std::invalid_argument("Digit exceeds base");
        }
        
    result += BigInteger(digit) * power;
    power *= BigInteger(base);
    }
    return result;
}

// Implements Lagrange Interpolation to find f(0)
BigInteger findSecret(const std::vector<Point>& shares) {
    BigInteger secret = 0;
    int k = shares.size();

    for (int j = 0; j < k; ++j) {
        BigInteger numerator = 1;
        BigInteger denominator = 1;

        for (int i = 0; i < k; ++i) {
            if (i == j) {
                continue;
            }
            // Numerator product: Π x_i
            numerator *= shares[i].x;
            // Denominator product: Π (x_i - x_j)
            denominator *= (shares[i].x - shares[j].x);
        }

        // The division must be exact in Shamir's scheme
        BigInteger term = shares[j].y * numerator / denominator;
        secret += term;
    }

    return secret;
}

int main() {
    // --- Data from the second test case ---
    // k = 7
    std::vector<Point> shares;

    shares.push_back({1, baseToDec("13444211440455345511", 6)});
    shares.push_back({2, baseToDec("aed7015a346d635", 15)});
    shares.push_back({3, baseToDec("6aeeb69631c227c", 15)});
    shares.push_back({4, baseToDec("e1b5e05623d881f", 16)});
    shares.push_back({5, baseToDec("316034514573652620673", 8)});
    shares.push_back({6, baseToDec("2122212201122002221120200210011020220200", 3)});
    shares.push_back({7, baseToDec("20120221122211000100210021102001201112121", 3)});

    // Calculate the secret
    BigInteger secret = findSecret(shares);

    // Print the final answer
    std::cout << "The reconstructed secret (c) is: " << secret << std::endl;

    return 0;
}