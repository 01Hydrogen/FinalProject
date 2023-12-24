#include "BigInt.hpp"

using namespace std;

/**
 * @brief Default constructor. Value is automatically set to zero, and sign is automatically positive.
 * 默认构造函数。值自动设为零，符号自动设为正。
 */
BigInt::BigInt()
{
    this->digits = vector<uint8_t>();
    this->digits.push_back(0);
    this->sign = true;
}

/**
 * @brief Initializes with a signed 64-bit integer.
 * 使用signed 64-bit integer进行初始化。
 *
 * @param value The signed 64-bit integer to initialize with.传入的signed 64-bit integer。
 */
BigInt::BigInt(int64_t value)
{
    this->digits = vector<uint8_t>();
    // Initialize sign
    // 初始化符号
    if (value >= 0)
    {
        this->sign = true;
    }
    else
    {
        this->sign = false;
        value = -value;
    }
    // Little-endian storage
    // 小端存储
    // If the input value is 0, then assign directly
    // 如果传入值为0则直接赋值
    if (value == 0)
    {
        this->sign = true;
        this->digits.push_back(0);
    }
    // Otherwise, split the input value and store
    // 否则拆分传入值并储存
    else
    {
        while (value != 0)
        {
            this->digits.push_back(static_cast<uint8_t>(value % 10));
            value /= 10;
        }
    }
}

/**
 * @brief Initializes from a string.
 * 从string初始化。
 *
 * @param value The string to initialize from. 传入的string。
 */
BigInt::BigInt(const string &value)
{
    this->digits = vector<uint8_t>();

    auto rend = value.rend();
    // Check the sign of the input number (string), start from index 1 to ignore the sign if it exists
    // 检测输入数字（字符串）的符号，如果有符号就从下标1开始来忽略符号
    if (value[0] == '-')
    {
        this->sign = false;
        rend--;
    }
    else if (value[0] == '+')
    {
        this->sign = true;
        rend--;
    }
    else
    {
        this->sign = true;
    }
    // Reverse traverse and store in little-endian
    // 反向遍历，并小端存储
    for (auto it = value.rbegin(); it != rend; ++it)
    {
        this->digits.push_back(static_cast<uint8_t>(*it - '0'));
    }
}

/**
 * @brief Copy constructor.
 * 拷贝构造函数。
 *
 * @param other The object to copy. 要拷贝的对象。
 */
BigInt::BigInt(const BigInt &other)
{
    // Creating a new object
    // 创建新对象
    BigInt newBigInt = BigInt();
    // Assignment
    // 赋值
    this->digits = other.getDigits();
    this->sign = other.getSign();
}

/**
 * @brief Overloaded assignment operator (String).
 * 重载赋值运算符（String）。
 *
 * @param other The string to assign from.赋值来源的string。
 * @return BigInt&
 */
BigInt &BigInt::operator=(const string &value)
{
    this->digits.clear();
    auto rend = value.rend();
    // Check the sign of the input number (string), start from index 1 to ignore the sign if it exists
    // 检测输入数字（字符串）的符号，如果有符号就从下标1开始来忽略符号
    if (value[0] == '-')
    {
        this->sign = false;
        rend--;
    }
    else if (value[0] == '+')
    {
        this->sign = true;
        rend--;
    }
    else
    {
        this->sign = true;
    }
    // Reverse traverse and store in little-endian
    // 反向遍历，并小端存储
    for (auto it = value.rbegin(); it != rend; ++it)
    {
        this->digits.push_back(static_cast<uint8_t>(*it - '0'));
    }
    return *this;
}

/**
 * @brief Overloaded assignment operator (int64_t).
 * 重载赋值运算符（int64_t）。
 *
 * @param other
 * @return BigInt&
 */
BigInt &BigInt::operator=(int64_t value)
{
    this->digits.clear();
    // Initialize sign
    // 初始化符号
    if (value >= 0)
    {
        this->sign = true;
    }
    else
    {
        this->sign = false;
        value = -value;
    }
    // Little-endian storage
    // 小端存储
    // If the input value is 0, then assign directly
    // 如果传入值为0则直接赋值
    if (value == 0)
    {
        this->digits.push_back(0);
    }
    // Otherwise, split the input value and store
    // 否则拆分传入值并储存
    else
    {
        while (value != 0)
        {
            this->digits.push_back(static_cast<uint8_t>(value % 10));
            value /= 10;
        }
    }
    return *this;
}

/**
 * @brief Overloaded assignment operator (BigInt).
 * 重载赋值运算符（BigInt）。
 *
 * @param other
 * @return BigInt&
 */
BigInt &BigInt::operator=(const BigInt &other)
{
    if (this == &other)
    {
        return *this;
    }
    this->digits = other.getDigits();
    this->sign = other.getSign();
    return *this;
}

/**
 * @brief += operation.
 * += 运算符。
 *
 * @param rhs Right-hand side. 右侧操作数。
 * @return BigInt&
 */
BigInt &BigInt::operator+=(const BigInt &rhs)
{
    // Addition with different signs
    // 异号相加
    if (this->sign != rhs.sign)
    {
        // lhs's absolute value is greater than or equal to rhs's
        // lhs 的绝对值大于等于 rhs 的绝对值
        if (isAbsGreaterOrEqual(*this, rhs))
        {
            subtractAbs(rhs);
        }
        // lhs's absolute value is less than rhs's
        // lhs 的绝对值小于 rhs 的绝对值
        else
        {
            // Create a copy of rhs
            // 创建 rhs 的副本
            BigInt temp = rhs; 
            temp.subtractAbs(*this);
            *this = std::move(temp);
            // The result has the same sign with rhs
            // 结果的符号与 rhs 相同
            this->sign = rhs.sign;
        }
    }
    // Addition with the same sign
    // 同号相加
    else
    {
        // Handling the same sign, whether both are positive or negative
        // 处理同号情况，无论是两个正数还是两个负数
        size_t maxLength = std::max(this->digits.size(), rhs.digits.size());
        // Carry
        // 进位标识
        bool carry = false;
        for (size_t i = 0; i < maxLength || carry; ++i)
        {
            // If lhs (left-hand operand, i.e., this) is the shorter one, append a digit to lhs
            // 如果lhs（左侧加数，即this）是长度较短的那个数，则每次给lhs补一位
            if (i == this->digits.size())
            {
                this->digits.push_back(0);
            }
            // The ternary operator: if the current index is within the bounds of rhs digits, then add the corresponding digit of lhs to rhs, otherwise add 0
            // 三则表达式：如果当前下标对于右侧加数没有下标越界，即右侧加数还没加完所有位时，则当前左侧加数对应位加右侧加数对应位，否则左侧加数对应位加0
            // Add the corresponding digit of lhs and rhs, plus 1 if there's a carry (true=1)
            // 左侧加数对应位与右侧加数对应位相加，有进位时额外+1（true=1）
            this->digits[i] += carry + (i < rhs.digits.size() ? rhs.digits[i] : 0);
            // Handling carry
            // 处理进位
            carry = (this->digits[i] >= 10);
            if (carry)
            {
                this->digits[i] -= 10;
            }
        }
    }
    // Remove leading zeros (mainly used for leading zeros generated by subtraction)
    // 删除前导零（主要用于减法时产生的前导零）
    while (this->digits.size() > 1 && this->digits.back() == 0)
    {
        this->digits.pop_back();
    }
    // If the final result is 0, forcefully set the sign to positive
    // 如果最终结果为0，把符号强制设置为+
    if ((this->digits.size() == 1) && (this->digits[0] == 0))
    {
        this->sign = true;
    }
    return *this;
}

/**
 * @brief -= operator, implemented as the inverse operation of +=.
 * -= 运算符，作为 += 的逆运算实现。
 *
 * @param rhs
 * @return BigInt&
 */
BigInt &BigInt::operator-=(const BigInt &rhs)
{
    return *this += -rhs;
}

/**
 * @brief *= operator, implemented by simulating manual multiplication.
 * *= 运算符，模拟手算乘法实现。
 *
 * @param rhs
 * @return BigInt&
 */
BigInt &BigInt::operator*=(const BigInt &rhs)
{
    // Create a new BigInt object as the result of multiplication
    // 创建一个新的 BigInt 对象作为乘法结果
    BigInt result;
    result.digits.resize(this->digits.size() + rhs.digits.size(), 0);

    // Perform multiplication operation
    // 执行乘法运算
    for (size_t i = 0; i < this->digits.size(); ++i)
    {
        int carry = 0;
        for (size_t j = 0; j < rhs.digits.size() || carry; ++j)
        {
            int current = result.digits[i + j] + this->digits[i] * (j < rhs.digits.size() ? rhs.digits[j] : 0) + carry;
            carry = current / 10;
            result.digits[i + j] = current % 10;
        }
    }

    // Remove leading zeros from the result
    // 删除结果中的前导零
    while (result.digits.size() > 1 && result.digits.back() == 0)
    {
        result.digits.pop_back();
    }

    // Handle the sign
    // 处理符号
    // The sign is positive by default when the result is 0
    // 结果为0时默认符号为正
    if (result.digits.size() == 1 && result.digits[0] == 0)
    {
        result.sign = true;
    }
    else
    {
        result.sign = !(this->sign ^ rhs.sign);
    }

    // Update the current object
    // 更新当前对象
    *this = std::move(result);
    return *this;
}

/**
 * @brief Negation operator.
 * 负号运算符。
 *
 * @return BigInt
 */
BigInt BigInt::operator-() const
{
    // Do nothing if it's 0
    // 如果为0则不做操作
    if (this->digits.size() == 1 && this->digits[0] == 0)
    {
        return *this;
    }
    // Otherwise, negate the sign
    // 否则符号取负
    else
    {
        BigInt result = BigInt(*this);
        result.sign = !this->sign;
        return result;
    }
}

/**
 * @brief == operator.
 * == 运算符。
 *
 * @param rhs Right-hand side右侧操作数 
 * @return true
 * @return false
 */
bool BigInt::operator==(const BigInt &rhs) const
{
    if (this->sign != rhs.sign)
    {
        return false;
    }
    else if (this->digits == rhs.digits)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief != operator.
 * != 运算符。
 *
 * @param rhs Right-hand side右侧操作数 
 * @return true
 * @return false
 */
bool BigInt::operator!=(const BigInt &rhs) const
{
    return !(*this == rhs);
}

/**
 * @brief < operator.
 * < 运算符。
 *
 * @param rhs Right-hand side右侧操作数
 * @return true
 * @return false
 */
bool BigInt::operator<(const BigInt &rhs) const
{
    // If the signs are different
    // 如果两数异号
    if (this->sign != rhs.sign)
    {
        if (this->sign)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    // If the signs are the same
    // 如果两数同号
    else
    {
        // For positive numbers or 0
        // 两数为正数或0
        if (this->sign)
        {
            // Compare the number of digits first, the number with more digits is definitely larger
            // 先比较位数，位数大的数必然更大
            if (this->size() > rhs.size())
            {
                return false;
            }
            else if ((this->size()) < rhs.size())
            {
                return true;
            }
            // If the number of digits is the same, compare digit by digit
            // 位数相同则一位一位比较
            else
            {
                // Use iterators for traversal
                // 使用迭代器进行遍历
                auto itRhs = rhs.digits.rbegin();
                for (auto it = this->digits.rbegin(); it != this->digits.rend(); it++)
                {
                    if (*it == *itRhs)
                    {
                        itRhs++;
                        continue;
                    }
                    else if (*it < *itRhs)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                // Reaching here means the two numbers are equal, so return false
                // 能走到这一步证明两数相等，所以return false
                return false;
            }
        }
        // For negative numbers
        else
        {
            // Compare the number of digits first, the number with more digits is actually smaller
            // 先比较位数，位数大的数必然更小
            if (this->size() > rhs.size())
            {
                return true;
            }
            else if ((this->size()) < rhs.size())
            {
                return false;
            }
            // If the number of digits is the same, compare digit by digit
            // 位数相同则一位一位比较
            else
            {
                // Use iterators for traversal
                // 使用迭代器进行遍历
                auto itRhs = rhs.digits.rbegin();
                for (auto it = this->digits.rbegin(); it != this->digits.rend(); it++)
                {
                    if (*it == *itRhs)
                    {
                        itRhs++;
                        continue;
                    }
                    if (*it < *itRhs)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                // Reaching here means the two numbers are equal, so return false
                // 能走到这一步证明两数相等，所以return false
                return false;
            }
        }
    }
}

/**
 * @brief > operator.
 * > 运算符。
 *
 * @param rhs
 * @return true
 * @return false
 */
bool BigInt::operator>(const BigInt &rhs) const
{
    // If the signs are different
    // 如果两数异号
    if (this->sign != rhs.sign)
    {
        if (this->sign)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    // If the signs are the same
    // 如果两数同号
    else
    {
        // For positive numbers or 0
        // 两数为正数或0
        if (this->sign)
        {
            // Compare the number of digits first, the number with more digits is definitely larger
            // 先比较位数，位数大的数必然更大
            if (this->size() > rhs.size())
            {
                return true;
            }
            else if ((this->size()) < rhs.size())
            {
                return false;
            }
            // If the number of digits is the same, compare digit by digit
            // 位数相同则一位一位比较
            else
            {
                // Use iterators for traversal
                // 使用迭代器进行遍历
                auto itRhs = rhs.digits.rbegin();
                for (auto it = this->digits.rbegin(); it != this->digits.rend(); it++)
                {
                    if (*it == *itRhs)
                    {
                        itRhs++;
                        continue;
                    }
                    else if (*it < *itRhs)
                    {
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                // Reaching here means the two numbers are equal, so return false
                // 能走到这一步证明两数相等，所以return false
                return false;
            }
        }
        // For negative numbers
        // 两数为负数
        else
        {
            // Compare the number of digits first, the number with more digits is actually smaller
            // 先比较位数，位数大的数必然更小
            if (this->size() > rhs.size())
            {
                return false;
            }
            else if ((this->size()) < rhs.size())
            {
                return true;
            }
            // If the number of digits is the same, compare digit by digit
            // 位数相同则一位一位比较
            else
            {
                // Use iterators for traversal
                // 使用迭代器进行遍历
                auto itRhs = rhs.digits.rbegin();
                // for (size_t i = this->digits.size(); i >= 0; i--)
                for (auto it = this->digits.rbegin(); it != this->digits.rend(); it++)
                {
                    // if (this->digits[i] == rhs.digits[i])
                    if (*it == *itRhs)
                    {
                        itRhs++;
                        continue;
                    }
                    // else if (this->digits[i] < rhs.digits[i])
                    if (*it < *itRhs)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                // Reaching here means the two numbers are equal, so return false
                // 能走到这一步证明两数相等，所以return false
                return false;
            }
        }
    }
}

/**
 * @brief <= operator, directly negates the > operator.
 * <= 运算符，直接取>运算符的负值。
 *
 * @param rhs
 * @return true
 * @return false
 */
bool BigInt::operator<=(const BigInt &rhs) const
{
    return !(*this > rhs);
}

/**
 * @brief >= operator, directly negates the < operator.
 * >= 运算符，直接取<运算符的负值。
 *
 * @param rhs
 * @return true
 * @return false
 */
bool BigInt::operator>=(const BigInt &rhs) const
{
    return !(*this < rhs);
}

/**
 * @brief Overloaded << operator, directly calls the toString method.
 * << 运算符重载，直接调用 toString 方法。
 *
 * @param os Output stream.输出流。
 * @param num Value to be output.输出的值。
 * @return ostream&
 */
ostream &operator<<(ostream &os, const BigInt &num)
{
    os << num.toString();
    return os;
}


/**
 * @brief Returns the number of digits.
 * 返回数字长度（位数）。
 *
 * @return BigInt::size_t
 */
size_t BigInt::size() const
{
    return this->digits.size();
}

/**
 * @brief Convert to string.
 * 输出为字符串。
 *
 * @return string Output string.输出字符串。
 */
string BigInt::toString() const
{
    // Initialize the output result
    // 初始化输出结果
    string output = "";
    // If it is a negative number, add a minus sign to the string
    // 如果是负数，则在字符串中加上负号
    if (!this->sign)
    {
        output.push_back('-');
    }
    // Construct the string
    // 构建字符串
    for (auto it = this->digits.rbegin(); it != this->digits.rend(); ++it)
    {
        output.push_back(static_cast<char>('0' + *it));
    }
    return output;
}

/**
 * @brief Digits getter
 *
 * @return vector<uint8_t>
 */
vector<uint8_t> BigInt::getDigits() const
{
    return this->digits;
}

/**
 * @brief Sign getter
 *
 * @return true
 * @return false
 */
bool BigInt::getSign() const
{
    return this->sign;
}

/**
 * @brief + operator, implemented using +=.
 * + 运算符，通过 += 实现。
 *
 * @param lhs
 * @param rhs
 * @return BigInt
 */
BigInt operator+(BigInt lhs, const BigInt &rhs)
{
    lhs += rhs;
    return lhs;
}

/**
 * @brief - operator, implemented using -=.
 * - 运算符，通过 -= 实现。
 *
 * @param lhs
 * @param rhs
 * @return BigInt
 */
BigInt operator-(BigInt lhs, const BigInt &rhs)
{
    lhs -= rhs;
    return lhs;
}

/**
 * @brief * operator, implemented using *=.
 * * 运算符，通过 *= 实现。
 *
 * @param lhs
 * @param rhs
 * @return BigInt
 */
BigInt operator*(BigInt lhs, const BigInt &rhs)
{
    lhs *= rhs;
    return lhs;
}

/**
 * @brief Utility function to compare absolute values.
 * Only called in case of different signs, so no need to consider the same sign.
 * 工具函数，比较绝对值大小。
 * 只会在两数异号的情况下调用，所以不需要考虑两数同号的情况。
 *
 * @param lhs
 * @param rhs
 * @return true
 * @return false
 */
bool BigInt::isAbsGreaterOrEqual(const BigInt &lhs, const BigInt &rhs)
{
    // Get absolute value, then compare
    // 先把两个数取绝对值，再比较绝对值大小
    if (lhs.sign)
    {
        return lhs >= -rhs;
    }
    else
    {
        return -lhs >= rhs;
    }
}

/**
 * @brief Utility function to subtract the smaller absolute value BigInt from the larger one.
 * 工具函数，从绝对值大的 BigInt 中减去绝对值小的 BigInt。
 *
 * @param rhs
 */
void BigInt::subtractAbs(const BigInt &rhs)
{
    bool borrow = false;
    for (size_t i = 0; i < this->digits.size(); ++i)
    {
        int subtraction = int(this->digits[i]) - (i < rhs.digits.size() ? int(rhs.digits[i]) : 0) - borrow;
        borrow = subtraction < 0;
        if (borrow)
        {
            subtraction += 10;
        }
        this->digits[i] = static_cast<uint8_t>(subtraction);
    }

    // Remove leading zeros from the result
    // 删除结果中的前导零
    while (this->digits.size() > 1 && this->digits.back() == 0)
    {
        this->digits.pop_back();
    }
}
