#include <windows.h>
#include <stdio.h>

int FilesCreate(UINT NumberOffiles);
int ConnectWinHttp();

int main(int argc, char *argv[]){
   // calling FileCreate for random file creation
   
   UINT NumberOffiles = 5;

   FilesCreate(NumberOffiles);

   // testing WinHTTP IOC
   ConnectWinHttp("www.microsoft.com");

   return 0;
}

