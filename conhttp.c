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

int ConnectWininet(LPCTSTR address){
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL, 
   	       hConnect = NULL,
    	       hRequest = NULL;

    PCTSTR rgpszAcceptTypes[] = {("/index.html"), NULL};

    double time_spent;

    // measure time
    clock_t begin = clock();

    // Use InternetOpen to obtain a session handle.
    hSession = InternetOpen("IOC HTTP Socket/1.0",
		      INTERNET_OPEN_TYPE_DIRECT,
		      NULL,
		      NULL,
		      0);
    if(hSession){
      // we have valid session
      hConnect = InternetConnect(hSession,
		      address,
		      INTERNET_DEFAULT_HTTP_PORT,
		      NULL,
		      NULL,
		      INTERNET_SERVICE_HTTP,
		      0,
		      0);
    }
    if(hConnect){
      // we have valid connection handle
      hRequest = HttpOpenRequest(hConnect,
		      NULL,
		      "/index.html",
		      NULL, 
		      NULL,
                      NULL,
                      0,
		      0);
    }

    printf("HTTP:: sending GET request .... \n");
    // sending request
    if(hRequest){
       bResults = HttpSendRequest(hRequest, 
		       NULL,
		       0,
		       NULL,
		       0);
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
