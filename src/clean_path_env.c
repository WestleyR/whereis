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

#include "clean_path_env.h"

int clean_path_env(char** new_path, const char* path) {
  int p = 0;
  char* path_old = strdup(path);

  char *pathname = strtok(path_old, ":");
  while (pathname != NULL) {
    if (p == 0) {
      new_path[p] = (char*) malloc(50);
      strcpy(new_path[p], pathname);
      p++;
    }

    int match = 0;
    for (int i = 0; i < p; i++) {
      if (strcmp(new_path[i], pathname) == 0) {
        match = 1;
      }
    }
    if (match != 1) {
      new_path[p] = (char*) malloc(50);
      strcpy(new_path[p], pathname);
      p++;
    }

    pathname = strtok(NULL, ":");
  }

  return(p);
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
