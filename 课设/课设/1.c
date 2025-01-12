#include <windows.h>
#include <stdio.h>

int main() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("C:\\Users\\LENOVO\\OneDrive\\桌面\\课设\\x64\\Debug\\*.mp3", &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        DWORD dwError = GetLastError(); // 获取错误代码
        printf("No MP3 files found or error occurred: %lu.\n", dwError);
    }
    else {
        do {
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                printf("%s\n", findFileData.cFileName);
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind); // 关闭查找句柄
    }

    return 0;
}