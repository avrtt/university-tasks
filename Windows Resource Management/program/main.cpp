#include <shlwapi.h>
#include <windows.h>
#include "../library/library.h"
#pragma comment(lib, "library.lib")
#pragma comment(lib, "shlwapi.lib")
#define BUFSIZE 128
char BUF[BUFSIZE];
volatile library_info libInfo;

// оконная процедура, обрабатывающая сообщения для окна программы
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC dc;

    switch (uMsg) {
    case WM_PAINT: // перерисовка окна
        dc = BeginPaint(hWnd, &ps); // инициализация контекста
        wnsprintf(BUF, BUFSIZE - 1, // вывод информации в буфер
            "Number of functional keys: %d",
            libInfo.funcKeysCount); // количество функциональных клавиш5
        TextOut(dc, 200, 50, BUF, lstrlen(BUF)); // вывод на экран (x=200, y=50)
        wnsprintf(BUF, BUFSIZE - 1,
            "L3 cache length: %d",
            (char)(libInfo.cpuidInfo)); // длина строки КЭШа
        TextOut(dc, 200, 70, BUF, lstrlen(BUF));
        EndPaint(hWnd, &ps); // завершение контекста
        break;
    case WM_DESTROY: // уничтожение окна (возвращается функцией ExitProcess)
        PostQuitMessage(EXIT_SUCCESS); // завершение работы
        break;
    default: // обработка любых других сообщений стандартным образом
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    return 0;
}

// функция, запускающаяся в рамках созданного потока
DWORD WINAPI ThreadFunc(void* lParam) {
    GetInfo((library_info*)lParam);
    return 0;
}

// точка входа графического приложения
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR CmdLine, int nCmdShow) {
    HANDLE hThread; // дескриптор потока
    DWORD IDThread; // идентификатор потока
    WNDCLASS wcl; // класс окна
    HWND hWnd; // дескриптор окна
    ATOM regReturn; // значение, возвращаемое функцией RegisterClass
    MSG msg; // сообщение

    // задание параметров окна
    wcl.hInstance = hInstance; // дескриптор
    wcl.lpszClassName = "KeyCounterWindow"; // название класса
    wcl.lpfnWndProc = WndProc; // функция-обработчик сообщений
    wcl.style = CS_VREDRAW | CS_HREDRAW; // стиль
    wcl.hIcon = NULL; // отключение пиктограммы
    wcl.hCursor = NULL; // отключение курсора
    wcl.lpszMenuName = NULL; // отключение меню
    wcl.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); // цвет фона
    regReturn = RegisterClass(&wcl); // регистрация класса

    // создание окна
    hWnd = CreateWindow(
        "KeyCounterWindow", // вышеуказанное название класса
        "Function Keys Counter", // заголовок окна
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, // стили окна
        0, 0, // координаты левого верхнего угла окна при выводе на экран
        600, 200, // ширина и высота окна
        HWND_DESKTOP, // рабочий стол как родительское окно
        NULL, // дочернее окно
        hInstance, // экземпляр приложения
        NULL); // указатель на данные создания окна

    // создание потока
    hThread = CreateThread(
        NULL, // запрет наследования дескриптора
        0, // начальный размер стека
        (LPTHREAD_START_ROUTINE)ThreadFunc, // указатель на функцию
        (LPVOID)&libInfo, // переменная, передаваемая потоку
        0, // флаг моментального запуска потока
        &IDThread); // переменная, в которую будет записан идентификатор потока

    // обработка ошибок при регистрации класса, создании окна или потока
    if (!regReturn || !hWnd || !hThread)
        return GetLastError();

    WaitForSingleObject(hThread, INFINITE); // бесконечное ожидание завершения потока
    CloseHandle(hThread); // удаление дескриптора потока
    ShowWindow(hWnd, SW_SHOWNORMAL); // отображение окна
    UpdateWindow(hWnd); // обновление окна

    // цикл, обрабатывающий сообщения
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg); // преобразование в ASCII
        DispatchMessage(&msg); // отправка сообщения в оконную процедуру
    }

    ExitProcess(msg.wParam); // завершение процесса и его потоков
}
