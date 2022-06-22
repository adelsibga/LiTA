/*
4.2. �������� (5)
���� ��������� ������ �������� ��� ������������ �� �����. ����� ���������� ����� �����, ������������� �� ���� �� ������� ����� ������������ �� ���������.

                       7
                   3        8
               8       1       6
           4       2        3      0

������ ��� �� ���� ���������� � ����������� ���� �� ��������� (����� ��� ������). ����������� ��������� �� ����� ����� �� -10^5 �� 10^5.
���� �� ����� INPUT.TXT. ������ ����� ���������� ���������� ����� ������������ N (N <= 300). ����� �������� ������ ������������.
����� � ���� OUTPUT.TXT. � ������ ������ ��������� ������������ ����� � ���������� �����. 
�� ������ ������ �������� ����� ������ ����� �� ������� ������������ �� ���������, ������ ���������� �����. ���� ������� ���������, ������� ����� �� ���.

������
����
4
7
3 8
8 1 6
4 2 3 0

�����
24
7 8 6 3

����������� �����, ��-21
*/

#include <fstream>
#include <vector>
#include <iostream>

constexpr int MAX = 300;

enum class MovingDirection
{
    Right,
    Left
};
using Matrix = std::vector<std::vector<long>>;

void InitFiles(std::ifstream& inputFile, std::ofstream& outputFile);
void WriteDesiredChooses(const Matrix& matrix, std::ostream& output, int trianglesLinesCount, int indexI, int indexJ, MovingDirection direction = MovingDirection::Right);
void FillMatrices(Matrix& originMatrix, Matrix& baseMatrix, std::istream& input, int trianglesLinesCount);
void CalculateBaseMatrix(Matrix& originMatrix, Matrix& baseMatrix, int trianglesLinesCount);

int main()
{
    std::ifstream inputFile;
    std::ofstream outputFile;

    try
    {
        InitFiles(inputFile, outputFile);
    }
    catch (const std::exception& exception)
    {
        std::cout << exception.what() << std::endl;
        return 1;
    }

    Matrix originMatrix(MAX, std::vector<long>(MAX));
    Matrix baseMatrix(MAX, std::vector<long>(MAX));

    int trianglesLinesCount;
    inputFile >> trianglesLinesCount;

    FillMatrices(originMatrix, baseMatrix, inputFile, trianglesLinesCount);
    CalculateBaseMatrix(originMatrix, baseMatrix, trianglesLinesCount);

    outputFile << baseMatrix[0][0] << std::endl;
    WriteDesiredChooses(baseMatrix, outputFile, trianglesLinesCount, 0, 0);

    return 0;
}

void InitFiles(std::ifstream& inputFile, std::ofstream& outputFile)
{
    inputFile.open("input.txt");
    outputFile.open("output.txt");

    if (!inputFile.is_open())
    {
        throw std::invalid_argument("Error, cannot open input file");
    }

    if (!outputFile.is_open())
    {
        throw std::invalid_argument("Error, cannot open output file");
    }
}

void WriteDesiredChooses(const Matrix& matrix, std::ostream& output, int trianglesLinesCount, int indexI, int indexJ, MovingDirection direction)
{
    if (indexI == trianglesLinesCount - 1)
    {
        if (direction == MovingDirection::Right)
        {
            output << std::max(matrix[indexI][indexJ], matrix[indexI][indexJ + 1]);
        }
        else
        {
            output << std::max(matrix[indexI][indexJ], matrix[indexI][indexJ - 1]);
        }

        return;
    }

    if (indexI < trianglesLinesCount - 1)
    {
        if (matrix[indexI + 1][indexJ] > matrix[indexI + 1][indexJ + 1])
        {
            output << matrix[indexI][indexJ] - matrix[indexI + 1][indexJ] << " ";
            WriteDesiredChooses(matrix, output, trianglesLinesCount, indexI + 1, indexJ, MovingDirection::Right);
        }
        else
        {
            output << matrix[indexI][indexJ] - matrix[indexI + 1][indexJ + 1] << " ";
            WriteDesiredChooses(matrix, output, trianglesLinesCount, indexI + 1, indexJ + 1, MovingDirection::Left);
        }
    }
}

void FillMatrices(Matrix& originMatrix, Matrix& baseMatrix, std::istream& input, int trianglesLinesCount)
{
    for (int indexI = 0; indexI < trianglesLinesCount; indexI++)
    {
        for (int indexJ = 0; indexJ <= indexI; indexJ++)
        {
            input>> originMatrix[indexI][indexJ];

            if (indexI == trianglesLinesCount - 1)
            {
                baseMatrix[indexI][indexJ] = originMatrix[indexI][indexJ];
            }
        }
    }
}

void CalculateBaseMatrix(Matrix& originMatrix, Matrix& baseMatrix, int trianglesLinesCount)
{
    for (int indexI = trianglesLinesCount - 2; indexI >= 0; indexI--)
    {
        for (int indexJ = 0; indexJ <= indexI; indexJ++)
        {
            baseMatrix[indexI][indexJ] = std::max(originMatrix[indexI][indexJ] + baseMatrix[indexI + 1][indexJ], originMatrix[indexI][indexJ] + baseMatrix[indexI + 1][indexJ + 1]);
        }
    }
}
