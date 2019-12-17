// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/whereis
// date: Dec 17, 2019
// version-1.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#include "whereis_cmd.h"

int whereis_cmdp(char **path, int path_len, const char* prog_name, int path_only) {
  struct stat info;
  int found = 1;
  struct dirent *dir;
  DIR *d;

  for (int i = 0; i < path_len; i++) {
    d = opendir(path[i]);
    if (d != NULL) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, prog_name) == 0) {
          char full_file_path[256];
          full_file_path[0] = '\0';
          strcat(full_file_path, path[i]);
          strcat(full_file_path, "/");
          strcat(full_file_path, prog_name);
          if (path_only != 0) {
            printf("%s: %s", prog_name, full_file_path);
          } else {
            printf("%s", full_file_path);
          }
          if (lstat(full_file_path, &info) != 0) {
            perror("lstat");
            return(1);
          }
          if (S_ISLNK(info.st_mode) && path_only != 0) {
            char symlink_path[126];
            ssize_t len = readlink(full_file_path, symlink_path, sizeof(symlink_path));
            if (len != -1) {
              symlink_path[len] = '\0';
            } else {
              printf("unable to fine link");
            }
            printf(" -> %s", symlink_path);
          }
          printf("\n");
          found = 0;
        }
      }
      closedir(d);
      if (path_only == 0 && found == 0) break;
    }
  }

  return(found);
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
