# BigInt Calculator Documentation

## Overview
This document describes the implementation of the BigInt calculator, which is capable of handling large integer calculations. **The code is extensively documented with detailed bilingual comments**, providing step-by-step explanations of the specific implementations. Each function, logical block, and significant line of code is accompanied by comments in both English and Chinese, ensuring clarity and ease of understanding.

## File Structure
- `BigInt.hpp`: The header file for the BigInt class.
- `BigInt.cpp`: Implementation of the BigInt class.
- `demo.cpp`: The main driver code for the application, which contains demonstration mode and file input mode. The file input mode automatically varifies the input.

## Main Components

### BigInt Class
The `BigInt` class provides functionalities to handle large integer calculations.

#### Member variables
- `private bool sign` : Sign of the integer.
- `private vector<uint8_t> digits` : Stores each digit of the integer.

#### Constructor
- `BigInt()`: Default constructor, initializes to zero. 
- `BigInt(int64_t value)` : Initialize from a 64-bit signed integer. Little-endian storage.
- `BigInt(const string &value)` : Initialize from a string. Little-endian storage.
- `BigInt(const BigInt &other)` : Copy constructor.

#### Public Methods
- `string toString() const`: Converts the BigInt to a string.
- `size_t size() const`: Returns the number of digits in the BigInt.
- `vector<uint8_t> getDigits() const`: Retrieves the digits of the BigInt.
- `bool getSign() const`: Returns the sign of the BigInt.

#### Operator Overloading
- `BigInt &operator=(const string &value)` : Assign with a string.
- `BigInt &operator=(int64_t value)` : Assign with a 64-bit integer.
- `BigInt &operator=(const BigInt &other)` : Assign with another BigInt.
- `BigInt &operator+=(const BigInt &rhs)` : Implemented as a member function. If two operands have the same sign, add digits by digits. Otherwise, compare their absolute values to calculate the sign, then substract digits by digits.
- `BigInt &operator-=(const BigInt &rhs)` : Implemented as `operand1 += -operand2`
- `BigInt &operator*=(const BigInt &rhs)` : Implemented by simulating manual multiplication.
- `BigInt operator-() const` : Unary negation operator, return a new BigInt with different sign.
- `BigInt operator+(BigInt lhs, const BigInt &rhs)` : Implmented using +=.
- `BigInt operator-(BigInt lhs, const BigInt &rhs)` : Implmented using -=.
- `BigInt operator*(BigInt lhs, const BigInt &rhs)` : Implmented using *=.
- `bool operator==(const BigInt &rhs) const` : Compare the signs, then call the == operator of vector to compare the digits.
- `bool operator!=(const BigInt &rhs) const` : Implemented as !(==).
- `bool operator<(const BigInt &rhs) const` : Compare the signs, then compare digit by digit.
- `bool operator>(const BigInt &rhs) const` : Compare the signs, then compare digit by digit.
- `bool operator<=(const BigInt &rhs) const` : Implemented as !(>).
- `bool operator>=(const BigInt &rhs) const` : Implemented as !(<).

## Usage
- To use file input mode, provide the file path as an argument.
- To enter demo mode, run the program without any arguments.

### Demo Mode
The `demoMode` function demonstrates the capabilities of the BigInt calculator.

### File Input Mode
The `fileInputMode` function allows the program to read and process equations from a file.

## Input File Syntax

The input file must adhere to the following syntax rules:
- Each line should contain only one operation.
- The operation must follow the syntax `operator(operand1,operand2)` for binary operators, or `operator(operand1)` for unary operators.

## Error Handling in File Input

When an error is encountered in the input file, the program will display an error message indicating the line number and the reason for the error. This feature helps in quickly identifying and rectifying issues within the input file.