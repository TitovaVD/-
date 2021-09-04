#pragma warning(disable : 4996)

#define WINVER 0x0A00
//#define PIPE_EXE_NAME _T("p64.exe")
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <fileapi.h>
#include <profileapi.h>
//#include <io.h> 
//#include <fcntl.h>

#define BUFSIZE MAX_PATH
#define FILESYSNAMEBUFSIZE MAX_PATH
#define SIZE_BUFF 256
#define MAX_LENGTH 255

using namespace std;

double WFreq;
__int64 Counter_Start = 0;
void Query()
{
    LARGE_INTEGER Frequency;
    if (!QueryPerformanceFrequency(&Frequency))
        cout << "����������� ��������� ������� QueryPerformanceFrequency()!\n";
    WFreq = double(Frequency.QuadPart);
    printf("\n������� ������� ��: %u ��\n", Frequency);
    QueryPerformanceCounter(&Frequency);
    Counter_Start = Frequency.QuadPart;
}

double GetCounter()
{
    LARGE_INTEGER Frequency;
    QueryPerformanceCounter(&Frequency);
    return ((Frequency.QuadPart - Counter_Start) * 1000000) / WFreq;
}

int main()
{
    HANDLE hFindVol;
    bool hCloseVol, hNextVol;
    //1.1 ����������� ������ ��:
    OSVERSIONINFO osVer = { 0 };
    osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osVer);
    const signed majornum = osVer.dwMajorVersion;
    const signed minornum = osVer.dwMinorVersion;
    const signed buildnum = osVer.dwBuildNumber;
    setlocale(LC_ALL, "Russian");
    printf("������ ��: v%d.%d.%d \n", majornum, minornum, buildnum);
   
    DWORD bufChar = SIZE_BUFF;
    TCHAR Buf[SIZE_BUFF];

    //1.2 ����������� ��������� ����������:
    GetSystemDirectory( Buf, SIZE_BUFF);
    printf("��������� ���������� : %s \n", Buf);

    //1.3 ����������� �������� ���������� � ����� ������������:
    GetComputerName(Buf, &bufChar);
    printf("�������� ����������: %s \n", Buf);

    GetUserName(Buf, &bufChar);
    printf("��� ������������: %s\n", Buf);

    //1.4 ����� ������������� �����:
    TCHAR buff[MAX_PATH];
    DWORD CBufLen = MAX_PATH;
    TCHAR name[MAX_PATH];
    CHAR VolumeName[MAX_PATH];

    HANDLE search = FindFirstVolume(name, BUFSIZE);
    ULARGE_INTEGER total, free;
   
    do {
            GetVolumePathNamesForVolumeName(name, buff, CBufLen, &CBufLen);
            printf("\n��� ����:");
            printf(name);
            printf("\n������ ���� � ����:");
            printf(buff);
            if (GetDiskFreeSpaceEx(name,&free ,&total,NULL) == 0)
            {
                printf("\n��� ����������\n");
            }
            else
            {
                cout << "\n����� ����:" << total.QuadPart;
                cout << "\n���������� ���������� �����:" << free.QuadPart<<"\n";
            }
    } while (FindNextVolume(search, name, BUFSIZE)==1);
    FindVolumeClose(search);
  
    DWORD dw_size;
    TCHAR sz_name[MAX_LENGTH];
    HKEY key;
    DWORD index = 0;
    DWORD retCode;
    DWORD buffSize = 8192;
    PPERF_DATA_BLOCK PerfData = (PPERF_DATA_BLOCK)malloc(buffSize);
    DWORD cbData = buffSize;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run",0, KEY_ALL_ACCESS, &key) == !ERROR_SUCCESS)
        printf("����������� ��������� ������� RegOpenKeyEx()!\n");
    else 
        printf("\n������ ��������:\n");

    while (1) 
    {
        dw_size = sizeof(sz_name);
        retCode = RegEnumValue(key, index, sz_name, &dw_size, NULL, NULL, NULL, NULL);
        if (retCode == ERROR_SUCCESS)
        {
            RegQueryValueEx(key, sz_name, NULL, NULL, (LPBYTE)PerfData, &cbData);
            printf("%d: %s:  %s\n", index + 1, sz_name, PerfData);
            index++;
        }
        else break;
    }
    RegCloseKey(key);
    Query();
    cout << "��������� ������� �� ���������� �� " << GetCounter() << "  ��� \n";
}
