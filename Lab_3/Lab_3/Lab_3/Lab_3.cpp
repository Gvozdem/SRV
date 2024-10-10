#include <iostream>
#include <conio.h>
#include <stdio.h>

int main()
{
    clock_t start, end;
    start = clock();
    std::string message = "the program is still alive!";
    char exitKey = 'q';
    char changeMessageKey = 'c';

    std::cout << "Press any key to start\n";
    char x = _getch();
    std::cout << "start - " << x << std::endl;

    while (!_kbhit())
    {
        end = clock();
        if ((double)(end - start) / CLOCKS_PER_SEC >= 1)
        {
            std::cout << message << std::endl;
            start = clock();
        }

        if (_kbhit() && _getch() == changeMessageKey)
        {
            message = "The message has been changed!";
        }

        if (_kbhit() && _getch() == exitKey)
        {
            std::cout << "Exiting the program..." << std::endl;
            break;
        }
    }

    return 0;
}