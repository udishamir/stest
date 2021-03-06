/*
 * ___________      ___________          
 * \__    ___/____ /_   \   _  \   ____  
 *   |    |  \__  \ |   /  /_\  \ /    \ 
 *   |    |   / __ \|   \  \_/   \   |  \
 *   |____|  (____  /___|\_____  /___|  /
 *                      \/           \/     \/ 
 * This function create random files, each file will have different size in order to simulate\
 * real time system behavior. That's a very naive assumption but should serve the purpose of load 
 *
 */

#include <windows.h>
#include <io.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUFSIZE 1024

// generating random numbers
int GetRandNum(){ 

    uint64_t rnd;
    rnd = rand();
    rnd = (rnd << 32) | rand();

    // enforce limits of value between 10000000 and 99999999
    rnd = (rnd % (99999999 - 10000000)) + 1000000;
    
    // lets try to leave some memory space
    return rnd;
}

int FilesCreate(UINT NumberOffiles){
    HANDLE hFile     = INVALID_HANDLE_VALUE;
    HANDLE hTempFile = INVALID_HANDLE_VALUE; 

    BOOL fSuccess  = FALSE;
    BOOL ErrorSetFilePointer  = FALSE;
    BOOL ErrorSetEndOfFile = FALSE;
    DWORD dwRetVal = 0;
    DWORD FLAGS	   = 0;
    UINT uRetVal   = 0;
    UINT uCounter  = 0;

    DWORD dwBytesRead    = 0;
    DWORD dwBytesWritten = 0; 
    DWORD dwBytestoWrite = 0; 

    DWORD BytesPerSector = 0; 
    DWORD Size = 0; // data buffer size

    TCHAR szTempFileName[MAX_PATH];  
    TCHAR lpTempPathBuffer[MAX_PATH];
    char  chBuffer[BUFSIZE]; 
    char  *buffer;
    void  *BufferAligned;
    double time_spent;
    UINT  rand;

    /*
     * Seed the random-number generator with the current time so that
     * the numbers will be different every time we run.
     * needs to be out of the for loop, no need to re-seed
    */
    srand((unsigned)time(NULL));

    printf("[ IO test .... ]\n\n");

    // number of iteration //    
    for(uCounter = 0; uCounter <= NumberOffiles; uCounter++){

	// being stopwatch
	clock_t begin = clock();

    	/* 
     	* Retrieve the temp file directory, according to Microsoft 
     	* its not guaranteed.
    	*/

   	dwRetVal = GetTempPath(MAX_PATH, lpTempPathBuffer);
   	if (dwRetVal > MAX_PATH || (dwRetVal == 0)){
	        printf("failed to extract system temp file directory, error: %d\n",\
					GetLastError());

		return -1;
   	}


   	uRetVal = GetTempFileName(lpTempPathBuffer,   // directory for tmp files
                         TEXT("STEST"),     	     // temp file name prefix 
                         0,                	    // create unique name 
                         szTempFileName);          // buffer for name 

   	if(uRetVal == 0){
        	printf("failed to retrieve temp file name, error: %d\n",\
			GetLastError());
	   	return -1;
   	}

	// get random number that will determined the file size
	rand = GetRandNum();

        // allocating a buffer
	buffer = VirtualAlloc(NULL,
		        rand, // random number
		        MEM_COMMIT,
		        PAGE_READWRITE);

        if(buffer == NULL){
		printf("failed to allocate memory size:%lld with error:%d\n",\
			rand, GetLastError());
		return -1;
	}

        // Setting flags
	FLAGS = FILE_FLAG_WRITE_THROUGH;
	// Creating file
	hFile = CreateFile(szTempFileName,
	              GENERIC_WRITE,
		      0,
		      NULL,
		      CREATE_ALWAYS,
		      FLAGS,   
		      NULL);

        if(hFile == INVALID_HANDLE_VALUE){
		printf("failed to CreateFile: %s with error: %d\n",\
			szTempFileName, GetLastError());
   
		VirtualFree(buffer,
		       rand,
		       MEM_RELEASE);

		return -1;
    
	}


	// Create random stream
        UINT i = 0;
	for(i = 0; i <= rand; i++){
		buffer[i] = GetRandNum();
	}

	// Write stream to file
	dwBytestoWrite = strlen(buffer);
	fSuccess = WriteFile(hFile,
			buffer,
			dwBytestoWrite,
			&dwBytesWritten,
			NULL);
	if(FALSE == fSuccess){
		printf("WriteFile failed with error: %d\n",
			GetLastError());
	}

	/*
	// extend the file size
	ErrorSetFilePointer = SetFilePointer(hFile,
			             rand, 
				     0,
				     FILE_BEGIN);

	if(FALSE == ErrorSetFilePointer){
		printf("SetFilePointer failed with error: %d\n", GetLastError());
	}

	// move pointer to the end of the file
        ErrorSetEndOfFile = SetEndOfFile(hFile);
	if(FALSE == ErrorSetEndOfFile){
		printf("SetEndOfFile failed with error: %d\n", GetLastError());
	}*/

	// cleaning after each iteration 
    	VirtualFree(buffer,
	       rand,
	       MEM_RELEASE);

    	CloseHandle(hFile);

	// stop the stopwatch
	clock_t end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("total function time: %f\n", time_spent);

	// remove the file, we are done here
	if((DeleteFile(szTempFileName)) == 0){
		printf("failed to delete: %s with the following error: %d\n",
				szTempFileName, GetLastError());
	}
    }
    
    return 0;
}
