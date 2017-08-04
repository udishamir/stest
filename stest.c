#include <windows.h>
#include <stdio.h>

int FilesCreate(UINT NumberOffiles);

int main(int argc, char *argv[]){
   // calling FileCreate for random file creation
   
   UINT NumberOffiles = 5;

   FilesCreate(NumberOffiles);

   return 0;
}

