#include <windows.h>
#include <stdio.h>

#include "config.h"

// prototypes 
int FilesCreate(UINT NumberOffiles);
int ConnectWininet(LPCTSTR address);
BOOL SSLConnectWininet(char *address);

__int64 GetSitesListSize(){
    HANDLE hFile;
    BOOL   bSuccess;
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

    bSuccess = GetFileSizeEx(hFile, &szFilesize);
    if(FALSE == bSuccess){
	    // closing handle
	    CloseHandle(hFile);

	    printf("retrieving file size failed with error: %d\n", GetLastError());
	    return 0;
    }

    // close handle
    CloseHandle(hFile);

    // return signed 
    return szFilesize.QuadPart;
}

/* 
 * handle HTTP connection 
 * first reading the file and send each URL to the HTTP function
 * the idea is to initiate connection to as much different sites
 * and testing the impact of hooking
*/


int ReadSiteUrl(int szTestFile){
    HANDLE hFile;
    BOOL   bSuccess;
    DWORD dwFileSize;
    FILE  *fp;


    if((fp = fopen(siteurl, "r")) == NULL){
	    printf("failed to open URL file with error: %d\n", GetLastError());
	    return 0;
    }

    // we have the file size
    dwFileSize = szTestFile;

    // no general error
    if(dwFileSize != 0xFFFFFFFF){
	    char *pszFileText;

	    pszFileText = VirtualAlloc(NULL,
			    	dwFileSize,
				MEM_COMMIT,
				PAGE_READWRITE);

            if(pszFileText != NULL){
		    printf("[ HTTP and TLS connection  .... ]\n\n");

		    while((fgets(pszFileText, dwFileSize, fp)) != NULL){
			    // remove trailing \n
			    pszFileText[strlen(pszFileText) - 1] = '\0';
			    // SSL connection
			    SSLConnectWininet(pszFileText);
		    }

		   // close stream handle
		   fclose(fp);

	    	   // free memory
	   	   VirtualFree(pszFileText, dwFileSize, MEM_RELEASE);
	    }
    }

    return 1;
}

int main(int argc, char *argv[]){
    UINT NumberOffiles = 5;
    int szTestFile;
 
    // retrieving the URL file size
    szTestFile = GetSitesListSize();

    if(szTestFile){
    	    // calling FileCreate for random file creation
    	    FilesCreate(NumberOffiles);

    	    // testing WinHTTP IOC
	    ReadSiteUrl(szTestFile);

    }else{
	    printf("error reading site url file, going out ...\n");
	    return -1;
    }

    return 0;
}
