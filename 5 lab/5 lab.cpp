#include <iostream>
#include <conio.h>
#include <windows.h>
#include <thread>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
int timeSleep = 500;

enum Colors
{
    BLACK = 0,
    GREY = 7,
    BLUE = 9,
    GREEN = 10,
    RED = 12,
    WHITE = 15
};

Colors color1 = GREEN;
Colors color2 = WHITE;

unsigned GetUnsignedNum()
{
    unsigned num = 0;
    while (true)
    {
        char ch = _getch();

        if (ch == '\r')
            break;
        else if ('0' <= ch && ch <= '9')
        {
            printf("%c", ch);
            num += ch - '0';
            num *= 10;
        }
    }
    num /= 10;
    cout << '\n';
    return num;
}

void CopyArray(int array[], int array2[], int size)
{
    for (int i = 0; i < size; i++)
    {
        array2[i] = array[i];
    }
}

unsigned int MyRand(unsigned int seed)
{
    seed = seed * 214013 + 2531011;  // Линейный конгруэнтный метод
    return (seed / 80234) % 10;
}

void AddRandomValues(int nums[], int size)
{
    int seed = time(0);
    for (int i = 0; i < size; i++)
    {
        nums[i] = MyRand(seed);
        if (nums[i] == 0)
            nums[i] = 1;
        seed++;
    }
}

void Sort(int nums[], int size)
{
    int j = size - 1;
    bool isNotOrdered = true;
    while (isNotOrdered)
    {
        isNotOrdered = false;
        for (int i = 0; i < j; i++)
        {
            if (nums[i] > nums[i + 1])
            {
                int temp = nums[i];
                nums[i] = nums[i + 1];
                nums[i + 1] = temp;
                isNotOrdered = true;
            }
        }
        j--;
    }
}

void PrintArray(int nums[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << nums[i] << ' ';
    }
    cout << '\n';
}

void GetColor()
{
    cout << "GREEN = 1\n";
    cout << "BLUE  = 2\n";
    cout << "RED   = 3\n";
    cout << "Enter the animation color: ";

    char command;
    while (true)
    {
        command = _getch();
        switch (command)
        {
        case '1':
            printf("%c\n\n", command);
            color1 = GREEN;
            color2 = WHITE;
            return;
        case '2':
            printf("%c\n\n", command);
            color1 = BLUE;
            color2 = WHITE;
            return;
        case '3':
            printf("%c\n\n", command);
            color1 = RED;
            color2 = WHITE;
            return;
        }

    }
}

void CursorBlinking(bool isBlinking)
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = isBlinking;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
}

void ShowSpeed()
{
    SetConsoleTextAttribute(hConsole, WHITE);
    COORD cPos{ 0, 0 };
    SetConsoleCursorPosition(hConsole, cPos);
    cout << "Speed: " << timeSleep << " ms (UP arrows/DOWN to change)";
    SetConsoleTextAttribute(hConsole, GREEN);
}

void CheckButtons()
{
    while (true)
    {
        if ((GetAsyncKeyState(VK_UP) & 0x8000) != 0)
        {
            if (timeSleep < 2000)
            {
                timeSleep += 50;
                Sleep(150);
            }
        }
        else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) != 0)
        {
            if (timeSleep > 50)
            {
                timeSleep -= 50;
                Sleep(150);
            }
        }
        Sleep(10);
    }
}

void PrintArrayShape(int nums[], int size, int b, char ch)
{
    SetConsoleTextAttribute(hConsole, color1);
    int a = 0;
    for (int i = 0; i < size; i++)
    {
        if (i == b || i == b + 1)
            SetConsoleTextAttribute(hConsole, color2);
        COORD cPos;
        cPos.Y = 1;
        cPos.X = i + a;
        for (int j = 0; j < 10; j++)
        {
            cPos.Y++;
            SetConsoleCursorPosition(hConsole, cPos);

            if (cPos.Y > 11 - nums[i])
            {
                if (j + 1 == 10)
                {
                    cout << nums[i];
                    a++;
                    break;
                }
                cout << ch;
            }
            else
            {
                cout << ' ';
            }
        }
        SetConsoleTextAttribute(hConsole, color1);
    }
}

void PrintArrayShapeLastFrame(int nums[], int size, int b, char ch)
{
    SetConsoleTextAttribute(hConsole, color1);
    int a = 0;
    for (int i = 0; i < size; i++)
    {
        if (i < b)
            SetConsoleTextAttribute(hConsole, color2);
        else
            SetConsoleTextAttribute(hConsole, color1);
        COORD cPos;
        cPos.Y = 11 - nums[i];
        cPos.X = i + a;
        for (int j = 0; j < nums[i]; j++)
        {
            cPos.Y += 1;
            SetConsoleCursorPosition(hConsole, cPos);
            if (j + 1 == nums[i])
            {
                cout << nums[i];
                a++;
                break;
            }
            cout << ch;
        }
    }
}

void SortAnimation(int nums[], int size, char ch)
{
    thread th(CheckButtons);
    th.detach();

    int j = size - 1;
    bool isNotOrdered = true;

    while (isNotOrdered)
    {
        isNotOrdered = false;
        for (int i = 0; i < j; i++)
        {
            if (nums[i] > nums[i + 1])
            {
                ShowSpeed();
                PrintArrayShape(nums, size, i, ch);
                Sleep(timeSleep);
                int temp = nums[i];
                nums[i] = nums[i + 1];
                nums[i + 1] = temp;
                isNotOrdered = true;
            }
            ShowSpeed();
            PrintArrayShape(nums, size, i, ch);
            Sleep(timeSleep);
        }
        j--;
    }

    PrintArrayShape(nums, size, size, ch);
    for (int i = 0; i < size; i++)
    {
        PrintArrayShapeLastFrame(nums, size, i + 1, ch);
        Sleep(20);
    }

    timeSleep = 500;
}

int main()
{
    setlocale(LC_ALL, "");

    while (true)
    {
        cout << "This program visualizes the bubble sorting algorithm!\n\n";
        int size;
        while (true)
        {
            cout << "Enter the size of the array of numbers (up to 30): ";
            size = GetUnsignedNum();
            if (size > 30)
            {
                cout << "The array size is too large!\n\n";
            }
            else
            {
                cout << "\n";
                break;
            }
        }

        cout << "Enter the character that will display the columns: ";
        char ch = _getch();
        cout << ch << "\n\n";

        GetColor();

        int* nums{ new int[size] };
        int* nums2{ new int[size] };
        AddRandomValues(nums, size);
        CopyArray(nums, nums2, size);
        cout << "The generated array:\n";
        PrintArray(nums, size);
        cout << "\n";

        cout << "Press any button to start the animation\n";
        _getch();
        system("cls");
        CursorBlinking(false);
        SortAnimation(nums2, size, ch);
        SetConsoleTextAttribute(hConsole, GREY);

        cout << "\n\n";
        cout << "The generated array:\n";
        PrintArray(nums, size);
        cout << "\n";
        cout << "Sorted array:\n";
        PrintArray(nums2, size);
        cout << "\n";

        cout << "Press 1 to restart the program\n";
        cout << "Press 0 to exit the program\n";
        char command;
        while (true)
        {
            command = _getch();
            if (command == '1')
            {
                system("cls");
                break;
            }
            else if (command == '0')
            {
                return 0;
            }
        }
        CursorBlinking(true);
    }

    return 0;
}
