// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/whereis
// date: Nov 5, 2019
// version-1.0.0
//
// The Clear BSD License
//
// Copyright (c) 2019 WestleyR
// All rights reserved.
//
// This software is licensed under a Clear BSD License.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char** argv) {

  if (argc != 2) {
    printf("Usage: %s <command>\n", argv[0]);
    return(1);
  }

  char *path_str;
  path_str = (char*) malloc(5000);
  strcpy(path_str, getenv("PATH"));

  struct stat info;
  int found = 1;

  struct dirent *dir;
  DIR *d;

  char* prog_name = argv[1];

  while (path_str[0]) {
    size_t i = 0;
    for (; path_str[i] && path_str[i] != ':'; ++i);
    path_str[i] = '\0';
    d = opendir(path_str);
    if (d != NULL) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, prog_name) == 0) {
          char full_file_path[256];
          full_file_path[0] = '\0';

          strcat(full_file_path, path_str);
          strcat(full_file_path, "/");
          strcat(full_file_path, prog_name);
          printf("%s", full_file_path);
          if (lstat(full_file_path, &info) != 0) {
            perror("lstat");
            return(1);
          }
          if (S_ISLNK(info.st_mode)) {
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
    }
    path_str += i + 1;
  }

  if (found != 0) {
    return(1);
  }

  return(0);
}


// vim: tabstop=2 shiftwidth=2 expandtab
