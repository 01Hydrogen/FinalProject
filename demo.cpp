#include "BigInt.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Utility function to display user's equation.
 * 工具函数，显示用户算式。
 *
 * @param os Output stream.输出流。
 * @param operands Operands.操作数。
 * @param op Operator.运算符。 
 */
void equationOutput(ostream &os, vector<BigInt> operands, char op)
{
    os << operands[0] << " " << op << " " << operands[1] << " = ";
}

/**
 * @brief Parses and executes an equation.
 * 解析并执行算式。
 *
 * @param line The line of input.输入行。
 * @param lineNumber The line number in the input file.文件中的行号。
 */
void parseAndExecute(const string &line, int lineNumber)
{
    try
    {
        // If the line is empty, throw an exception.
        // 空文件则抛出异常
        if (line.empty())
        {
            throw runtime_error("Empty line in line " + lineNumber);
        }

        // Operator
        // 运算符
        char op = line[0];
        // Check for legality of the operator.
        // 运算符合法性判断
        if (!(line[0] == '+' || line[0] == '-' || line[0] == '*'))
        {
            throw runtime_error("Illegal input or operator");
        }
        // Extract contents within parentheses.
        // 提取括号内的内容
        string operandsStr = line.substr(2, line.size() - 3);

        stringstream ss(operandsStr);
        string operand;
        vector<BigInt> operands;

        // Parse operands.
        // 解析操作数
        while (getline(ss, operand, ','))
        {
            // Check for empty string.
            // 检查是否为空字符串
            if (operand.empty())
            {
                throw std::runtime_error("Empty operand");
            }

            // Check for validity (only digits and an optional single negative sign).
            // 检查合法性（仅数字和可选的单个负号）
            bool isNegative = (operand[0] == '-');
            if (isNegative && operand.size() == 1)
            {
                throw std::runtime_error("Invalid operand: '-' without number");
            }
            if (isNegative && operand[1] == '0')
            {
                // If the user inputs -0, treat it as 0.
                // 如果用户输入了-0，则按照0来处理
                if (operand.size() == 2)
                {
                    operand = "0";
                }
                else
                {
                    throw std::runtime_error("Invalid operand: negative number starts with 0");
                }
            }
            for (size_t i = isNegative ? 1 : 0; i < operand.size(); ++i)
            {
                if (!std::isdigit(operand[i]))
                {
                    throw std::runtime_error("Invalid character in operand");
                }
            }
            // Initialize as a BigInt and store.
            // 初始化为BigInt并储存
            operands.push_back(BigInt(operand));
        }

        // Check the number and type of operands.
        // 检查操作数数量和类型
        if ((op == '+' || op == '*') && operands.size() != 2)
        {
            throw runtime_error("Invalid number of operands");
        }
        // The - operator can be unary or binary.
        //-运算符可以是一元的，也可以是二元的
        if (op == '-' && (operands.size() < 1 || operands.size() > 2))
        {
            throw runtime_error("Invalid number of operands");
        }

        // Execute the operation.
        // 执行运算
        BigInt result;
        switch (op)
        {
        case '+':
            equationOutput(cout, operands, op);
            result = operands[0] + operands[1];
            break;
        case '-':
            // Unary - operator
            // 一元-运算符
            if (operands.size() == 1)
            {
                cout << op << " " << operands[0] << " = ";
                result = -operands[0];
            }
            // Binary - operator
            // 二元-运算符
            else
            {
                equationOutput(cout, operands, op);
                result = operands[0] - operands[1];
            }
            break;
        case '*':
            equationOutput(cout, operands, op);
            result = operands[0] * operands[1];
            break;
        default:
            throw runtime_error("Unsupported operator");
        }

        cout << result << endl;
    }
    catch (const exception &e)
    {
        cerr << "Error in line " << lineNumber << ": " << e.what() << endl;
    }
}

/**
 * @brief File input mode.
 * 文件输入模式。
 *
 * @param filePath The path of the file.文件路径。
 */
void fileInputMode(const string &filePath)
{
    ifstream file(filePath);
    if (!file)
    {
        cerr << "Error opening file: " << filePath << endl;
        return;
    }
    string line;
    int lineNumber = 1;
    while (getline(file, line))
    {
        parseAndExecute(line, lineNumber++);
    }
}


/**
 * @brief Demonstration mode.
 * 演示模式。
 *
 */
void demoMode()
{
    cout<<"Default constructor: "<<BigInt()<<endl;
    cout<<"Constructor that takes signed 64-bit integer: "<<BigInt(-234343246)<<endl;
    cout<<"Constructor that takes string: "<<BigInt("-23948723487901461543613452341325325")<<endl;
    cout<<"Constructor that takes another BigInt: "<<BigInt(BigInt("22222222222244444444"))<<endl<<endl;
    
    cout<<"Addition: "<<endl;
    cout<<"234326685623523 + 980927189936952374194 = "<<BigInt(234326685623523)+BigInt("980927189936952374194")<<endl;
    BigInt test1=BigInt(234);
    test1+=BigInt(-556);
    cout<<"test1=234, test1 += (-556): "<<test1<<endl<<endl;

    cout<<"Subtraction: "<<endl;
    cout<<"7897013827597535246 - 2187454325 = "<<BigInt(7897013827597535246)-BigInt(2187454325)<<endl;
    test1-=BigInt(-31415926);
    cout<<"test1 -= (-31415926): "<<test1<<endl<<endl;

    cout<<"Multiplication: "<<endl;
    cout<<"212353526236 * (-3462930817434286) = "<< BigInt(212353526236)*BigInt(-3462930817434286)<<endl;
    test1*=BigInt(-2);
    cout<<"test1 *= (-2): "<<test1<<endl<<endl;

    cout<<"Negation: "<<endl;
    cout<<"-test1 = "<<-test1<<endl<<endl;

    cout<<"Comparison: "<<endl;
    cout<<boolalpha<<"422 == 345: "<<(BigInt(422)==BigInt(345))<<endl;
    cout<<boolalpha<<"24 != 24: "<<(BigInt(24)==BigInt(24))<<endl;
    cout<<boolalpha<<"5 < 250: "<<(BigInt(5)<BigInt(250))<<endl;
    cout<<boolalpha<<"10 <= 10: "<<(BigInt(10)<=BigInt(10))<<endl;
    cout<<boolalpha<<"343 > -919: "<<(BigInt(343)>BigInt(-919))<<endl;
    cout<<boolalpha<<"-13 >= -15: "<<(BigInt(-13)>=BigInt(-15))<<endl<<endl;

    cout<<"Assignment: "<<endl;
    BigInt test2=BigInt(2);
    cout<<"test2 = "<<test2<<endl;
    test2=-256;
    cout<<"Assign with a 64-bit integer: test2 = -256:"<<test2<<endl;
    test2="9090";
    cout<<"Assign with a string: test2 = \"9090\": "<<test2<<endl;
    test2=test1;
    cout<<"Assign with another BigInt: test2 = test1: "<<test2<<endl<<endl;



}

int main(int argc, char *argv[])
{
    string mode;
    cout << "Enter mode (enter 1 for demo mode, 2 for file mode): ";
    cin >> mode;

    if (mode == "2")
    {
        string filePath;
        if (argc > 1)
        {
            filePath = argv[1];
        }
        else
        {
            cout << "Enter filePath: ";
            cin >> filePath;
        }
        fileInputMode(filePath);
    }
    else if (mode == "1")
    {
        demoMode();
    }
    else
    {
        cerr << "Invalid mode selected." << endl;
    }

    cout << "Press any key to exit." << endl;
    cin.get();
    cin.get();
    return 0;
}

