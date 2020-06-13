#ifndef __FILESYSTEM_HPP__
#define __FILESYSTEM_HPP__

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include<bits/stdc++.h>

class FuseFTP 
{
public:
  FuseFTP() {}

  ~FuseFTP() {}

public :  
  static int fuse_getattr (const char *, struct stat *);

  static int fuse_readdir(const char *path, void *buf,
                     fuse_fill_dir_t filler,
                     off_t offset, struct fuse_file_info *fi
                  );

  static int fuse_read(const char *path, char *buf, size_t size, off_t offset,
                  struct fuse_file_info *fi);
};

#endif