/*
 *  ___________      ___________
 *  \__    ___/____ /_   \   _  \   ____
 *    |    |  \__  \ |   /  /_\  \ /    \
 *    |    |   / __ \|   \  \_/   \   |  \
 *    |____|  (____  /___|\_____  /___|  /
                   \/           \/     \/
 *  This function utilize Wininet library \
 *  it will test IOC performance
 * 
 * 
*/

#include <windows.h>
#include <time.h>
#include <wininet.h>
#include <stdio.h>

BOOL SSLConnectWininet(char *address){
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL, 
   	       hConnect = NULL,
    	       hRequest = NULL;

    double time_spent;

    // measure time
    clock_t begin = clock();

    // Use InternetOpen to obtain a session handle.
    hSession = InternetOpen("IOC HTTP Socket/1.0",
		      INTERNET_OPEN_TYPE_PRECONFIG,
		      NULL,
		      NULL,
		      0);

    if(hSession == NULL){
        printf("InternetOpen failed with error: %d\n", GetLastError());
        return FALSE;
    }

    hConnect = InternetConnect(hSession,
		      address,
		      INTERNET_DEFAULT_HTTPS_PORT,
		      NULL,
		      NULL,
		      INTERNET_SERVICE_HTTP,
		      INTERNET_FLAG_SECURE,
		      0);

    if(hConnect == NULL){
	printf("InternetConnect  failed with error: %d\n", GetLastError());
        return FALSE;
    }

   hRequest = HttpOpenRequest(hConnect,
		      "GET",
		      "",
		      HTTP_VERSION, 
		      "",
                      0,
                      INTERNET_FLAG_SECURE|
		      INTERNET_FLAG_DONT_CACHE|
		      INTERNET_FLAG_IGNORE_CERT_CN_INVALID,
		      0);

   if(hRequest == NULL){
       printf("InternetOpen failed with error: %d\n", GetLastError());
       return FALSE;
   }

   if(!HttpSendRequest(hRequest, 0, 0, 0, 0) ){
       		printf("HttpSendRequest failed with error: %d\n",
				GetLastError());

		return FALSE;
   }

  clock_t end = clock();
  time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

  printf("total function time: %f\n", time_spent); 
    
  // we are done here
  goto cleanup;


cleanup:
    // Close any open handles.
    if(hRequest) InternetCloseHandle(hRequest);
    if(hConnect) InternetCloseHandle(hConnect);
    if(hSession) InternetCloseHandle(hSession);
}
