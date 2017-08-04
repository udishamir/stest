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
#include <winhttp.h>
#include <stdio.h>

int main(){
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    BOOL  bResults = FALSE;
    HINTERNET  hSession = NULL, 
   	       hConnect = NULL,
    	       hRequest = NULL;


    // Use WinHttpOpen to obtain a session handle.
    hSession = WinHttpOpen(L"IOC HTTP Socket/1.0 Test Suite",
		      WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		      WINHTTP_NO_PROXY_NAME,
		      WINHTTP_NO_PROXY_BYPASS, 0);
    if(hSession){
      // we have valid session
      hConnect = WinHttpConnect(hSession, L"www.microsoft.com",
		      INTERNET_DEFAULT_HTTPS_PORT, 0);
    }

    if(hConnect){
      // we have valid connection handle
      hRequest = WinHttpOpenRequest( hConnect, L"GET", NULL,
		      INTERNET_DEFAULT_HTTPS_PORT, 0 ); // will be changed


    // creating http request 
     
    
    // we are done here
    goto cleanup;


cleanup:
    // Close any open handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(hSession) WinHttpCloseHandle(hSession);
}
