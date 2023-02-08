// DllTestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Game.h"
#include "DisplayWin.h"
#include <dxgi.h>

int main()
{
    UINT i = 0;
    IDXGIAdapter* pAdapter;
    IDXGIFactory1* pFactory;
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)(&pFactory));
    while (pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC desc;
        pAdapter->GetDesc(&desc);
        std::cout << desc.Description << std::endl;
        ++i;
    }
    std::cout << "Hello World!\n";
    DisplayWin display{ 800, 800 };
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file