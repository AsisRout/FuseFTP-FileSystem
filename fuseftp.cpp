#include "fuseftp.hpp"
#define FUSE_USE_VERSION 26
#define _FILE_OFFSET_BITS  64

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include<bits/stdc++.h>
#include <curl/curl.h>

#include "ftpclient.hpp"

using namespace std;

URL_FILE * handle;
FTPClient Fc;

class options {

public:
  const char *urlname;
  const char *contents;
} options;



bool isDirectory(string url)   //Check if URL is a Directory or not  ( ftp://192.168.1.100:2121// )
{
  if(url[url.length()-1]=='/')
    return true;
  return false;
}

bool isFile(string url) //Check if URL is a file or not ( ftp://192.168.1.100:2121/testfile.txt/* )
{
  if(url[url.length()-1]=='*')
    return true;
  return false;
}



int FuseFTP::fuse_getattr(const char *path, struct stat *stbuf)
{
   int res = 0;
     char * last_char = strrchr((char *)path,'*');
     memset(stbuf, 0, sizeof(struct stat));
     if (strcmp(path, "/") == 0 || last_char!=NULL) {
      stbuf->st_mode = S_IFDIR | 0755;
      stbuf->st_nlink = 2;
     } else {
      stbuf->st_mode = S_IFREG | 0444;
      stbuf->st_nlink = 1;
      stbuf->st_size = strlen(options.contents);
     }
  

  return res;
}

int FuseFTP::fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
       off_t offset, struct fuse_file_info *fi)
{
  (void) offset;
  (void) fi;
  string url = options.urlname;

  filler(buf, ".", NULL, 0);
  filler(buf, "..", NULL, 0);
  if(isDirectory(url))
  {
    filler(buf, options.contents, NULL, 0);
  }
  else
  {
    string err="Error:The URL is a file not a directory";
    filler(buf, err.c_str() ,NULL,0);

  }

  return 0;
}

int FuseFTP::fuse_read(const char *path, char *buf, size_t size, off_t offset,
          struct fuse_file_info *fi)
{
    size_t len;
    (void) fi;

    string url = options.urlname;

    if(isFile(url)){

      len = strlen(options.contents);
      if (offset < len) {
        if (offset + size > len)
            size = len - offset;
      memcpy(buf, options.contents + offset, size);
      } else
        size = 0;
    }
    else
    {
      string err="Error:The URL is a directory not a file";
      strcpy(buf,err.c_str());
      size = err.length();
    }

    return size;
}


FuseFTP fs;

static struct fuse_operations fuse_oper = {
  .getattr  = fs.fuse_getattr,
   .read    = fs.fuse_read,
  .readdir  = fs.fuse_readdir,
};



int CreateFTPConnection(const char* path)
{
  handle = url_fopen(path, "r");
  if(!handle) {
    cout<<"Couldn't url_fopen()\n";
    return 2;
  }
  return 1;
}

void CloseFTPConnection()
{
  Fc.url_fclose(handle);
}

int main(int argc, char *argv[])
{
    
   string url;
   url = argv[1];
   options.urlname = argv[1];
   
   url = url.substr(0,url.length()-2);  //Delete last 2 charachters of URL 
   
   const char* path = url.c_str();

   int conn=CreateFTPConnection(path);
   
   if(conn!=1)
   {
      return conn;
   }

   //Read Contents of URL
   char buffer[256];
   string cont;
   while(!Fc.url_feof(handle)) {
     
      Fc.url_fgets(buffer, sizeof(buffer), handle);
      cont += buffer;
    }
   options.contents = cont.c_str() ;
   
   CloseFTPConnection();

   char *argv1[] ={argv[0],argv[2]};
    
   argc =2;

   return fuse_main(argc, argv1, &fuse_oper, NULL);
}
