#include <iostream>
#include <string>
using namespace std;

#define MAXSIZE 200 // 最大位数

class int__
{
private:
    bool negative;
    short num[MAXSIZE];

    int__ addhelp(const int__ &x, const int__ &y) const; // 同号
    int__ subhelp(const int__ &x, const int__ &y) const; // 异号
    void setNegative(bool b);

public:
    int__() : negative(false), num{} {};
    int__(string &input);
    int__(const char *input);
    void operator=(string &input);
    void operator=(const char *input);
    int compareMagnitude(const int__ &other) const; // 比较绝对值大小
    int__ operator+(const int__ &other) const;
    int__ operator-(const int__ &other) const;
    int__ operator*(const int__ &other) const;
    int__ operator/(const int__ &other) const;
    friend ostream &operator<<(ostream &os, const int__ &x);
    friend istream &operator>>(istream &is, int__ &x);
    friend int__ operator!(const int__ &x);
    int getDigits() const; // 获取大整数的位数
};

int main()
{
    int__ x, y;
    cin >> x >> y;
    cout << x + y << endl;
    cout << x - y << endl;
    cout << x * y << endl;
    cout << x / y << endl;
    return 0;
}

int__ int__::addhelp(const int__ &x, const int__ &y) const
{
    int__ result;
    short carry = 0;
    for (int i = 0; i < MAXSIZE; i++)
    {
        short sum = x.num[i] + y.num[i] + carry;
        result.num[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry > 0)
    {
        cerr << "Error: Result exceeds maximum size (" << MAXSIZE << " digits)." << endl;
        exit(EXIT_FAILURE);
    }
    result.setNegative(x.negative);
    return result;
}

int__ int__::subhelp(const int__ &x, const int__ &y) const
{
    int cmpResult = x.compareMagnitude(y); // 比较绝对值
    int__ result, greater, less;
    switch (cmpResult)
    {
    case 0:
        return result;
        break;
    case 1:
        greater = x;
        less = y;
        break;
    case -1:
        greater = y;
        less = x;
        break;
    default:
        break;
    }
    for (int i = 0; i < MAXSIZE; i++)
    {
        if (greater.num[i] < less.num[i])
        {
            greater.num[i] += 10;
            greater.num[i + 1] -= 1;
        }
        result.num[i] = greater.num[i] - less.num[i];
    }
    result.setNegative(greater.negative);
    return result;
}

void int__::setNegative(bool b)
{
    this->negative = b;
}

int__::int__(string &input)
{
    *this = input;
}

int__::int__(const char *input)
{
    *this = input;
}

void int__::operator=(string &input)
{
    negative = (input[0] == '-');
    if (negative)
    {
        input.erase(0, 1);
    }
    // 将字符串表示的数字转换为大整数
    int len = input.length();

    // 检测输入是否超过最大位数
    if (len > MAXSIZE)
    {
        cerr << "Error: Input exceeds maximum size (" << MAXSIZE << " digits)." << endl;
        exit(EXIT_FAILURE);
    }

    int j = 0;
    for (int i = len - 1; i >= 0; i--)
    {
        // 检查是否是合法数字字符
        if (input[i] < '0' || input[i] > '9')
        {
            cerr << "Error: Invalid character in input." << endl;
            exit(EXIT_FAILURE);
        }
        num[j++] = input[i] - '0';
    }

    // 将剩余的位数初始化为0
    while (j < MAXSIZE)
    {
        num[j++] = 0;
    }
}

void int__::operator=(const char *input)
{
    string temp = (string)input;
    *this = temp;
}

int int__::compareMagnitude(const int__ &other) const
{
    for (int i = MAXSIZE - 1; i >= 0; i--)
    {
        if (num[i] > other.num[i])
        {
            return 1; // 当前对象的绝对值大于参数对象
        }
        else if (num[i] < other.num[i])
        {
            return -1; // 当前对象的绝对值小于参数对象
        }
    }

    return 0; // 绝对值相等
}

int__ int__::operator+(const int__ &other) const
{
    int__ temp;
    if (this->negative == other.negative)
    {
        temp = addhelp(*this, other);
    }
    else
    {
        temp = subhelp(*this, other);
    }
    return temp;
}

int__ int__::operator-(const int__ &other) const
{
    return *this + (!other);
}

int__ int__::operator*(const int__ &other) const
{
    int__ result;
    if (this->compareMagnitude(result) == 0 || other.compareMagnitude(result) == 0)
    {
        return result;
    }
    result.setNegative(this->negative != other.negative);
    int lenx = this->getDigits();
    int leny = other.getDigits();
    for (int i = 0; i < lenx; i++)
    {
        for (int j = 0; j < leny; j++)
        {
            if (i + j < MAXSIZE - 1)
            {
                result.num[i + j] += this->num[i] * other.num[j];
                if (i + j + 1 < MAXSIZE - 1)
                {
                    result.num[i + j + 1] += result.num[i + j] / 10;
                }
                else
                {
                    cerr << "Error: Result exceeds maximum size (" << MAXSIZE << " digits)." << endl;
                    exit(EXIT_FAILURE);
                }
                result.num[i + j] %= 10;
            }
            else
            {
                cerr << "Error: Result exceeds maximum size (" << MAXSIZE << " digits)." << endl;
                exit(EXIT_FAILURE);
            }
        }
    }
    return result;
}

int__ int__::operator/(const int__ &other) const
{
    int__ quotient;

    // 判断是否除以零
    if (other.compareMagnitude(quotient) == 0)
    {
        cerr << "Error: Division by zero." << endl;
        exit(EXIT_FAILURE);
    }

    // 判断符号
    quotient.setNegative(this->negative != other.negative);

    // 预处理，确保被除数不小于除数
    int__ dividend = *this;
    int__ divisor = other;
    if (dividend.compareMagnitude(divisor) < 0)
    {
        return quotient; // 结果为零
    }

    // 初始化变量
    int lenDividend = dividend.getDigits();
    int lenDivisor = divisor.getDigits();
    int lenQuotient = lenDividend - lenDivisor + 1;

    int__ tempDividend;
    int__ tempDivisor = divisor;
    tempDivisor.setNegative(false);

    for (int i = lenDividend - 1; i >= 0; i--)
    {
        tempDividend = tempDividend * int__("10");
        tempDividend.num[0] = dividend.num[i];

        int digit = 0;
        while (tempDividend.compareMagnitude(tempDivisor) >= 0)
        {
            tempDividend = tempDividend - tempDivisor;
            digit++;
        }

        quotient.num[i] = digit;
    }
    return quotient;
}

int int__::getDigits() const
{
    int i = MAXSIZE - 1;
    while (i >= 0 && num[i] == 0)
    {
        i--;
    }
    return (i == -1) ? 1 : i + 1;
}

ostream &operator<<(ostream &os, const int__ &x)
{
    int i = MAXSIZE - 1;
    while (i >= 0 && x.num[i] == 0)
    {
        i--;
    }
    if (i == -1)
    {
        os << 0;
    }
    else
    {
        if (x.negative)
        {
            os << '-';
        }
        for (; i >= 0; i--)
        {
            os << x.num[i];
        }
    }
    return os;
}

istream &operator>>(istream &is, int__ &x)
{
    string input;
    is >> input;
    x = input;
    return is;
}

int__ operator!(const int__ &x)
{
    int__ temp = x;
    temp.setNegative(!temp.negative);
    return temp;
}