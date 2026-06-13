// MT5Bridge.cpp - DLL واسط C++ برای Named Pipe
// کامپایل: Visual Studio 2019/2022 - Release x64
// Output: MT5Bridge.dll -> کپی به MQL5/Libraries/

#include <windows.h>
#include <string>

HANDLE hPipe = INVALID_HANDLE_VALUE;

extern "C" __declspec(dllexport)
bool __stdcall ConnectPipe(const wchar_t* pipeName)
{
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }

    hPipe = CreateFileW(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0, NULL,
        OPEN_EXISTING,
        0, NULL
    );

    return (hPipe != INVALID_HANDLE_VALUE);
}

extern "C" __declspec(dllexport)
bool __stdcall SendData(const char* data, int length)
{
    if (hPipe == INVALID_HANDLE_VALUE) return false;

    DWORD written = 0;
    return WriteFile(hPipe, data, (DWORD)length, &written, NULL)
           && (written == (DWORD)length);
}

extern "C" __declspec(dllexport)
int __stdcall ReceiveData(char* buffer, int bufferSize)
{
    if (hPipe == INVALID_HANDLE_VALUE) return -1;

    DWORD bytesRead = 0;
    BOOL ok = ReadFile(hPipe, buffer, (DWORD)bufferSize, &bytesRead, NULL);
    if (!ok) return -1;
    return (int)bytesRead;
}

extern "C" __declspec(dllexport)
void __stdcall DisconnectPipe()
{
    if (hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(hPipe);
        hPipe = INVALID_HANDLE_VALUE;
    }
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_DETACH) DisconnectPipe();
    return TRUE;
}
