#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>

#include "BigInt.hpp"

struct PolynomialPoint {
    BigInt x_coordinate;
    BigInt y_coordinate;
};

class ShamirSolver {
private:
    std::vector<PolynomialPoint> decoded_shares;

    BigInt convertBaseToDecimal(const std::string& value_string, int base) {
        BigInt decimal_result = 0;
        BigInt current_power = 1;

        for (int i = value_string.length() - 1; i >= 0; i--) {
            int digit_value;
            char current_char = value_string[i];

            if (current_char >= '0' && current_char <= '9') {
                digit_value = current_char - '0';
            } else if (current_char >= 'a' && current_char <= 'z') {
                digit_value = current_char - 'a' + 10;
            } else {
                throw std::invalid_argument("Invalid character detected in number string.");
            }

            if (digit_value >= base) {
                throw std::invalid_argument("A digit in the provided value exceeds the specified base.");
            }
            
            decimal_result += digit_value * current_power;
            current_power *= base;
        }
        return decimal_result;
    }

public:
    void addShare(const BigInt& x_value, const std::string& y_value_string, int y_base) {
        PolynomialPoint new_point;
        new_point.x_coordinate = x_value;
        new_point.y_coordinate = convertBaseToDecimal(y_value_string, y_base);
        this->decoded_shares.push_back(new_point);
    }

    BigInt solveForSecret() {
        BigInt final_secret = 0;
        int total_shares = this->decoded_shares.size();

        if (total_shares == 0) {
            std::cerr << "Warning: No shares provided to solve for the secret." << std::endl;
            return 0;
        }

        for (int j = 0; j < total_shares; ++j) {
            BigInt lagrange_numerator = 1;
            BigInt lagrange_denominator = 1;

            for (int i = 0; i < total_shares; ++i) {
                if (i == j) {
                    continue;
                }
                lagrange_numerator *= this->decoded_shares[i].x_coordinate;
                lagrange_denominator *= (this->decoded_shares[i].x_coordinate - this->decoded_shares[j].x_coordinate);
            }
            
            BigInt current_term = this->decoded_shares[j].y_coordinate * lagrange_numerator / lagrange_denominator;
            final_secret += current_term;
        }
        return final_secret;
    }

    void clearShares() {
        this->decoded_shares.clear();
    }
};

int main() {
    ShamirSolver solver_instance;

    std::cout << "Processing Test Case 1..." << std::endl;
    solver_instance.addShare(1, "4", 10);
    solver_instance.addShare(2, "111", 2);
    solver_instance.addShare(3, "12", 10);
    
    BigInt secret_result_one = solver_instance.solveForSecret();
    std::cout << "Reconstructed Secret for Test Case 1: " << secret_result_one << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    solver_instance.clearShares();

    std::cout << "Processing Test Case 2..." << std::endl;
    solver_instance.addShare(1, "13444211440455345511", 6);
    solver_instance.addShare(2, "aed7015a346d635", 15);
    solver_instance.addShare(3, "6aeeb69631c227c", 15);
    solver_instance.addShare(4, "e1b5e05623d881f", 16);
    solver_instance.addShare(5, "316034514573652620673", 8);
    solver_instance.addShare(6, "2122212201122002221120200210011020220200", 3);
    solver_instance.addShare(7, "20120221122211000100210021102001201112121", 3);

    BigInt secret_result_two = solver_instance.solveForSecret();
    std::cout << "Reconstructed Secret for Test Case 2: " << secret_result_two << std::endl;

    return 0;
}