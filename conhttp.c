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
    hSession = WinHttpOpen(L"Testing HTTP Socket/1.0",
		      WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
		      WINHTTP_NO_PROXY_NAME,
		      WINHTTP_NO_PROXY_BYPASS, 0);
    if(hSession){
      hConnect = WinHttpConnect(hSession, L"www.microsoft.com",
		      INTERNET_DEFAULT_HTTPS_PORT, 0);
    }

    if(hConnect){
      // we have connection handle
      WinHttpOpenRequest( hConnect, L"GET", NULL,



    // we are done here
    goto cleanup;


cleanup:
    // Close any open handles.
    if(hRequest) WinHttpCloseHandle(hRequest);
    if(hConnect) WinHttpCloseHandle(hConnect);
    if(hSession) WinHttpCloseHandle(hSession);
}
