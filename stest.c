#include <windows.h>
#include <stdio.h>

#include "config.h"

// prototypes 
int FilesCreate(UINT NumberOffiles);
int ConnectWinHttp();

__int64 GetSitesListSize(){
    HANDLE hFile;
    BOOL   flStat;
    LARGE_INTEGER szFilesize;
   
    // open sites.txt for reading
    hFile = CreateFile(siteurl, 
		    0,
		    0,
		    NULL,
		    OPEN_EXISTING,
		    FILE_ATTRIBUTE_NORMAL,
		    NULL);

    if(hFile == INVALID_HANDLE_VALUE){
	    printf("failed to open url file error: %d\n", GetLastError());
	    return 0;
    }

    flStat = GetFileSizeEx(hFile, &szFilesize);
    if(FALSE == flStat){
	    // closing handle
	    CloseHandle(hFile);

	    printf("retrieveing file size failed with error: %d\n", GetLastError());
	    return 0;
    }

    CloseHandle(hFile);

    // return signed 
    return szFilesize.QuadPart;
}

int main(int argc, char *argv[]){
    UINT NumberOffiles = 5;
    int szTestFile;

    szTestFile = GetSitesListSize();
    if(szTestFile){
	    printf("url file size %db\n", szTestFile);

    	    // calling FileCreate for random file creation
    	    FilesCreate(NumberOffiles);

    	    // testing WinHTTP IOC
    	    ConnectWinHttp("www.microsoft.com");
    }else{
	    printf("error reading site url file, going out ...\n");
	    return -1;
    }

    return 0;
}
