/*
 *  ___________      ___________
 *  \__    ___/____ /_   \   _  \   ____
 *    |    |  \__  \ |   /  /_\  \ /    \
 *    |    |   / __ \|   \  \_/   \   |  \
 *    |____|  (____  /___|\_____  /___|  /
 *                       \/           \/     \/
 *  This function utilize WinHTTP library \
 *  it will test IOC performance
 * 
 * 
*/

#include <windows.h>
#include <time.h>
#include <winhttp.h>
#include <stdio.h>

int ConnectWinHttp(WCHAR *address){
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

    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"IOC HTTP Socket/1.0 Test Suite",
		      WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		      WINHTTP_NO_PROXY_NAME,
		      WINHTTP_NO_PROXY_BYPASS, 0);
    if(hSession){
      // we have valid session
      hConnect = WinHttpConnect(hSession, address,
		      INTERNET_DEFAULT_HTTPS_PORT, 0);
    }
    if(hConnect){
      // we have valid connection handle
      hRequest = WinHttpOpenRequest(hConnect, L"GET", NULL,
		      NULL, WINHTTP_NO_REFERER,
                      WINHTTP_DEFAULT_ACCEPT_TYPES,
                      WINHTTP_FLAG_SECURE);
    }

    printf("sending GET request .... \n");
    // sending request
    if(hRequest){
       bResults = WinHttpReceiveResponse(hRequest, NULL);
    } 
    
    clock_t end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("total function time: %f\n", time_spent); 
    
    // we are done here
    goto cleanup;


cleanup:
    // Close any open handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(hSession) WinHttpCloseHandle(hSession);
}
