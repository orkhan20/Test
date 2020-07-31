/***************************************************************************
 * This is a copy program for Linux systems.                               *
 * This program use Linux system functions.                                *
 *                                                                         *
 * For executing program you must compile as                               *
 * showed below:                                                           *
 *   -mkdir ~/myBin                                                        *
 *   -cd mkdir                                                             *
 *   -gcc cp.c -o cp   (cp.c must be in this directory)                    *
 *   -export $PATH="$HOME/myBin:$PATH"   (adding our directory to PATH)    *
 *                                                                         *
 *     written by ORKHAN ALIBAYLI                                          *
 ***************************************************************************/
 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void errorMsg(char *onWhat)
{
  fprintf(stderr, "An error occured on %s: %s\n", onWhat,strerror(errno));
}

int main(int argc, char **argv)
{
  int N=10;                  // number of bytes   calloc(N,sizeof(char))
  int op;                    // open() file descriptor
  int st;                    // return for stat()
  struct stat fileStat;      // status of file
  ssize_t writtenBytes;      // return for write()
  ssize_t readenBytes;       // return for read()
  char *buff;                // for reading and writing data
  char ch;                   // chars of string buffer
  int i=0;                   //index for loop

  buff=(char *)calloc(N,sizeof(char));
  if(buff==NULL)
  {
    errorMsg("calloc()");
  }


  //getting arguments from comand line
  if(argc!=3 && strcmp(argv[2],"--help")!=0)
  {
    printf("usage error:   cp [arg1] [arg2]\n");
  }


  //status of file
  st=stat(argv[1],&fileStat);
  if(st==-1)
  {
    errorMsg("stat()");
  }


  mode_t mdt=S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH;
  mdt=fileStat.st_mode & mdt;


  //opening file which will be copied
  op=open(argv[1],O_RDWR);
  if(op<0)
  {
    errorMsg("open()");
  }


  /*
    reading string from file to buffer.
    creating another memory block if needed
  */
  while(read(op,&ch,1)!=0)
  {

    if(i==N)
    {
      N*=2;
      buff=realloc(buff,N);
    }

    buff[i]=ch;

    i++;
  }

  //opening end file( target file for copying )
  op=open(argv[2],O_CREAT | O_WRONLY,mdt);
  if(op<0)
  {
    errorMsg("open()");
  }

  //writing string to target file
  writtenBytes=write(op,buff,i);
  if(writtenBytes==-1)
  {
    errorMsg("write()");
  }

}
