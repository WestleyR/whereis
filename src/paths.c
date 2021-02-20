//
//  paths.c
//  whereis - simple whereis command that works! (for macOS)
//
// Created by WestleyR 2019-12-17
// Source code: https://github.com/WestleyR/whereis
//
// Copyright (c) 2019-2021 WestleyR. All rights reserved.
// This software is licensed under a BSD 3-Clause Clear License.
// Consult the LICENSE file that came with this software regarding
// your rights to distribute this software.
//

#include "paths.h"

struct wst_path_ctx {
  char** paths;
  int path_len; // TODO: this should be path_count
  bool path_only; // TODO: make this work
};

wst_path_ctx* wst_get_path() {
  wst_path_ctx* ctx;
  ctx = (wst_path_ctx*) malloc(sizeof(wst_path_ctx));
  ctx->paths = (char**) malloc(30 * sizeof(char*));

  int p = 0;
  char* path_env = getenv("PATH");

  char *pathname = strtok(path_env, ":");
  while (pathname != NULL) {
    bool match = false;
    for (int i = 0; i < p; i++) {
      if (strcmp(ctx->paths[i], pathname) == 0) {
        match = true;;
      }
    }
    if (!match) {
      ctx->paths[p] = (char*) malloc(80);
      strcpy(ctx->paths[p], pathname);
      p++;
    }

    pathname = strtok(NULL, ":");
  }

  ctx->path_len = p;

  return ctx;
}

int wst_free(wst_path_ctx* ctx) {
  for (int i = 0; i < ctx->path_len; i++) {
    free(ctx->paths[i]);
  }
  free(ctx->paths);
  free(ctx);

  return 0;
}

char* wst_whereis(wst_path_ctx* ctx, const char* prog_name, bool path_only) {
  struct stat info;
  int found = 1;
  struct dirent *dir;
  DIR *d;

  for (int i = 0; i < ctx->path_len; i++) {
    printf("Opening: %s->%d\n", ctx->paths[i], i);
    d = opendir(ctx->paths[i]);
    if (d != NULL) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, prog_name) == 0) {
          char full_file_path[256];
          full_file_path[0] = '\0';
          strcat(full_file_path, ctx->paths[i]);
          strcat(full_file_path, "/");
          strcat(full_file_path, prog_name);
          if (path_only != 0) {
            printf("%s: %s", prog_name, full_file_path);
          } else {
            printf("%s", full_file_path);
          }
          if (lstat(full_file_path, &info) != 0) {
            perror("lstat");
            return NULL;
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

  return NULL;
}



// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
