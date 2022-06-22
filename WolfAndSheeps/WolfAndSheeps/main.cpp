/*
7.5. Волки и овцы (8)
 
Волки и овцы. На координатной плоскости заданы отрезками N волков и M овец. 
Пастух с ружьем находится в начале координат. 
Выстрел поражает всех животных по направлению полета пули. 
Найти наименьшее число выстрелов для того, чтобы убить всех волков и не тронуть овец.

Ввод из файла INPUT.TXT. В первой строке  задаются  через  пробел  значения N и M (1 <= N, M <= 10^3). 
В следующих N строках - целочисленные координаты начала (X1, Y1) и конца (X2, Y2) отрезка, соответствующего волку (-1000 <= X1, X2 <= 1000; 1 <= Y1, Y2 <= 1000). 
Аналогично в следующих M строках указывается положение овец.

Вывод в файл OUTPUT.TXT единственного целого числа либо сообщения “No solution”.

Пример
Ввод
2 1
1 1 2 3
-5 4 2 2
999 1000 1000 999
Вывод
1

Сибгатуллин Адель, ПС-21
*/

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

const std::string WOLF_VALUE = "wolf";
const std::string SHEEP_VALUE = "sheep";
const std::string BEGIN_BORDER = "begin";
const std::string END_BORDER = "end";

struct Point
{  
    Point(const std::string& borderType, const std::string& identification, double X)
    {
        this->BorderType = borderType;
        this->Identification = identification;
        this->X = X;
    }

    double X = 0;
    double BeginX = 0;
    double EndX = 0;
    int EndableIndex = -1;
    bool IsEndableIndexDefined = false;
    std::string BorderType = "UNDEFINED";
    std::string Identification = "UNDEFINED";
};

using Axis = std::vector<Point>;

void SortAxis(Axis& axis);
void InitFiles(std::ifstream& inputFile, std::ofstream& outputFile);
void FillWolfs(Axis& axis, std::istream& input, int wolfsCount);
void FillSheeps(Axis& axis, std::istream& input, int sheepsCount);
void DefineSequenceBorders(Axis& axis, int border);
int GetCountOfShots(Axis& axis);

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

    int wolfsCount = 0, sheepsCount = 0;
    Axis axis;

    inputFile >> wolfsCount >> sheepsCount;

    FillWolfs(axis, inputFile, wolfsCount);
    FillSheeps(axis, inputFile, sheepsCount);

    SortAxis(axis);
    DefineSequenceBorders(axis, 2 * (sheepsCount + wolfsCount));

    int shotsCount = GetCountOfShots(axis);
    
    if (shotsCount == -1)
    {
        outputFile << "No solution";
    }
    else
    {
        outputFile << shotsCount;
    }

    return 0;
}

void SortAxis(Axis& axis)
{
    std::sort(axis.begin(), axis.end(), [](const Point& left, const Point& right)
    {
        if (left.X != right.X)
        {
            return left.X < right.X;
        }

        if (left.BorderType != right.BorderType)
        {
            return left.BorderType == BEGIN_BORDER;
        }

        if (left.BorderType == BEGIN_BORDER)
        {
            return left.Identification == SHEEP_VALUE;
        }
    });
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

void FillWolfs(Axis& axis, std::istream& input, int wolfsCount)
{
    double firstPointX, firstPointY, secondPointX, secondPointY;

    for (int i = 0; i < wolfsCount; i++)
    {
        input >> firstPointX >> firstPointY >> secondPointX >> secondPointY;
        axis.push_back(Point(BEGIN_BORDER, WOLF_VALUE, std::min(firstPointX / firstPointY, secondPointX / secondPointY)));
        axis.push_back(Point(END_BORDER, WOLF_VALUE, std::max(firstPointX / firstPointY, secondPointX / secondPointY)));
        axis[axis.size() - 2].EndX = axis[axis.size() - 1].X;
        axis[axis.size() - 1].BeginX = axis[axis.size() - 2].X;
    }
}

void FillSheeps(Axis& axis, std::istream& input, int sheepsCount)
{
    double firstPointX, firstPointY, secondPointX, secondPointY;

    for (int i = 0; i < sheepsCount; i++)
    {
        input >> firstPointX >> firstPointY >> secondPointX >> secondPointY;
        axis.push_back(Point(BEGIN_BORDER, SHEEP_VALUE, std::min(firstPointX / firstPointY, secondPointX / secondPointY)));
        axis.push_back(Point(END_BORDER, SHEEP_VALUE, std::max(firstPointX / firstPointY, secondPointX / secondPointY)));
    }
}

void DefineSequenceBorders(Axis& axis, int border)
{
    for (int indexI = 0; indexI < border; indexI++)
    {
        if (axis[indexI].Identification != WOLF_VALUE || axis[indexI].BorderType != BEGIN_BORDER)
        {
            continue;
        }

        for (int indexJ = 0; indexJ < border; indexJ++)
        {
            if (axis[indexJ].Identification == WOLF_VALUE && axis[indexJ].BorderType == END_BORDER && axis[indexI].X == axis[indexJ].BeginX &&
                axis[indexI].EndX == axis[indexJ].X && !axis[indexJ].IsEndableIndexDefined)
            {
                axis[indexI].EndableIndex = indexJ;
                axis[indexJ].IsEndableIndexDefined = true;
                break;
            }
        }
    }
}

int GetCountOfShots(Axis& axis)
{
    int desiredSheeps = 0;
    int shotsCount = 0;
    int firstSheepIndex;
    double firstSheepPoint;
    bool isPossible = true;

    for (int index = 0; index < axis.size(); index++)
    {
        if (axis[index].Identification == SHEEP_VALUE && axis[index].BorderType == BEGIN_BORDER)
        {
            if (desiredSheeps == 0)
            {
                firstSheepIndex = index;
                firstSheepPoint = axis[index].X;
            }
            desiredSheeps++;

            continue;
        }
        
        if (axis[index].Identification == SHEEP_VALUE && axis[index].BorderType == END_BORDER)
        {
            desiredSheeps--;
            continue;
        }
        
        if (axis[index].Identification == WOLF_VALUE && axis[index].BorderType == END_BORDER)
        {
            if (desiredSheeps == 0)
            {
                for (int indexJ = index; indexJ >= 0; indexJ--)
                {
                    if (axis[indexJ].Identification == WOLF_VALUE && axis[indexJ].BorderType == BEGIN_BORDER)
                    {
                        axis[axis[indexJ].EndableIndex].BorderType = "DEAD";
                    }
                }
                shotsCount++;
            }
            else
            {
                if (axis[index].BeginX < firstSheepPoint)
                {
                    for (int indexJ = firstSheepIndex - 1; indexJ >= 0; indexJ--)
                    {
                        if (axis[indexJ].Identification == WOLF_VALUE && axis[indexJ].BorderType == BEGIN_BORDER)
                        {
                            axis[axis[indexJ].EndableIndex].BorderType = "DEAD";
                        }
                    }
                    shotsCount++;
                }
                else
                {
                    isPossible = false;
                    break;
                }
            }
        }
    }

    return isPossible ? shotsCount : -1;
}
