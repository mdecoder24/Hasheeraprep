#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include "BigInt.hpp"

struct Point {
    BigInt x;
    BigInt y;
};

BigInt baseToDec(const std::string& value, int base) {
    BigInt result = 0;
    BigInt power = 1;

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
        
        result += digit * power;
        power *= base;
    }
    return result;
}

BigInt findSecret(const std::vector<Point>& shares) {
    BigInt secret = 0;
    int k = shares.size();

    for (int j = 0; j < k; ++j) {
        BigInt numerator = 1;
        BigInt denominator = 1;

        for (int i = 0; i < k; ++i) {
            if (i == j) {
                continue;
            }
            numerator *= shares[i].x;
            denominator *= (shares[i].x - shares[j].x);
        }

        BigInt term = shares[j].y * numerator / denominator;
        secret += term;
    }

    return secret;
}

int main() {
    std::cout << "Solving Test Case 1..." << std::endl;
    std::vector<Point> shares1;
    shares1.push_back({1, baseToDec("4", 10)});
    shares1.push_back({2, baseToDec("111", 2)});
    shares1.push_back({3, baseToDec("12", 10)});
    
    BigInt secret1 = findSecret(shares1);
    std::cout << "Secret for Test Case 1: " << secret1 << std::endl;
    std::cout << "-------------------------" << std::endl;

    std::cout << "Solving Test Case 2..." << std::endl;
    std::vector<Point> shares2;
    shares2.push_back({1, baseToDec("13444211440455345511", 6)});
    shares2.push_back({2, baseToDec("aed7015a346d635", 15)});
    shares2.push_back({3, baseToDec("6aeeb69631c227c", 15)});
    shares2.push_back({4, baseToDec("e1b5e05623d881f", 16)});
    shares2.push_back({5, baseToDec("316034514573652620673", 8)});
    shares2.push_back({6, baseToDec("2122212201122002221120200210011020220200", 3)});
    shares2.push_back({7, baseToDec("20120221122211000100210021102001201112121", 3)});

    BigInt secret2 = findSecret(shares2);
    std::cout << "Secret for Test Case 2: " << secret2 << std::endl;

    return 0;
}