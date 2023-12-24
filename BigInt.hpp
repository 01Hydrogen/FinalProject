using namespace std;

#include <vector>
#include <iostream>
#include <cstdint>
#include <stdexcept>
#include <string>

class BigInt
{
public:
    // Constructors 构造函数
    // Default constructor, initializes to zero 默认构造函数，初始化为零
    BigInt();
    // Initialize from a 64-bit signed integer从 64 位有符号整数初始化                 
    BigInt(int64_t value);  
    // Initialize from a string 从字符串初始化     
    BigInt(const string &value); 

    // Copy constructor and assignment operators拷贝构造函数和赋值运算符
    // Copy constructor 拷贝构造函数
    BigInt(const BigInt &other);
    // Assignment operator赋值运算符
    BigInt &operator=(const string &value); 
    BigInt &operator=(int64_t value);
    BigInt &operator=(const BigInt &other);

    // Compound assignment operators 复合赋值运算符
    BigInt &operator+=(const BigInt &rhs);
    BigInt &operator-=(const BigInt &rhs);
    BigInt &operator*=(const BigInt &rhs);

    // Unary operator 一元运算符
    // Negation operator 负号运算符，改变数的符号
    BigInt operator-() const; 

    // Comparison operators 比较运算符
    bool operator==(const BigInt &rhs) const;
    bool operator!=(const BigInt &rhs) const;
    bool operator<(const BigInt &rhs) const;
    bool operator>(const BigInt &rhs) const;
    bool operator<=(const BigInt &rhs) const;
    bool operator>=(const BigInt &rhs) const;

    // Insertion operator 输入输出运算符
    friend std::ostream &operator<<(std::ostream &os, const BigInt &num);

    // Other auxiliary methods 其他辅助方法
    // Returns the length of the number (number of digits) 返回数字的长度（位数）
    size_t size() const;  
    // Converts BigInt to a string 将 BigInt 转换为字符串             
    string toString() const;  
    // Digits getter
    vector<uint8_t> getDigits() const; 
    // Sign getter
    bool getSign() const; 

private:
    // Sign of the integer数的符号
    bool sign;   
    // Stores each digit of the integer存储数字的每一位                
    vector<uint8_t> digits; 
    // Comparation of the absolute values绝对值比较
    bool isAbsGreaterOrEqual(const BigInt &lhs, const BigInt &rhs); 
    // Subtracts the smaller absolute value BigInt from the larger absolute value BigInt 从绝对值大的 BigInt 中减去绝对值小的 BigInt
    void subtractAbs(const BigInt &rhs);                            
};

// Arithmetic operators 算术运算符
BigInt operator+(BigInt lhs, const BigInt &rhs); 
BigInt operator-(BigInt lhs, const BigInt &rhs); 
BigInt operator*(BigInt lhs, const BigInt &rhs); 