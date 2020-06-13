#ifndef __LIBCURL_HPP__
#define __LIBCURL_HPP__

#include <stdio.h>
#include <string.h>
#ifndef WIN32
#include <sys/time.h>
#endif
#include <stdlib.h>
#include <errno.h>
#include <curl/curl.h>
#include<bits/stdc++.h>

enum fcurl_type_e {
  CFTYPE_NONE = 0,
  CFTYPE_FILE = 1,
  CFTYPE_CURL = 2
};

//initialization 

struct fcurl_data
{
  enum fcurl_type_e type;     /* type of handle */ 
  union {
    CURL *curl;
    FILE *file;
  } handle;                   /* handle */ 
 
  char *buffer;               /* buffer to store cached data*/ 
  size_t buffer_len;          /* currently allocated buffers length */ 
  size_t buffer_pos;          /* end of data in buffer*/ 
  int still_running;          /* Is background url fetch still in progress */ 
};

typedef struct fcurl_data URL_FILE;
URL_FILE *url_fopen(const char *url, const char *operation);
static CURLM *multi_handle;




class FTPClient 
{ 

  public :
    static int url_fclose(URL_FILE *file);
    static int url_feof(URL_FILE *file);
    static size_t url_fread(void *ptr, size_t size, size_t nmemb, URL_FILE *file);
    static char *url_fgets(char *ptr, size_t size, URL_FILE *file);
    static void url_rewind(URL_FILE *file);
    static size_t write_callback(char *buffer,
                             size_t size,
                             size_t nitems,
                             void *userp);

    static int fill_buffer(URL_FILE *file, size_t want);
    static int use_buffer(URL_FILE *file, size_t want);	
    static size_t GetFilesList_response(void *ptr, size_t size, size_t nmemb, void *data);

};

#endif