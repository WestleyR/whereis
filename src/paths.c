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

  // This is the actral output of the whereis function. This
  // is automaticly freed when called wst_free().
  char* cmd_output;

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
  ctx->cmd_output = NULL;

  return ctx;
}

int wst_set_path_only(wst_path_ctx* ctx, bool path_only) {
  if (ctx != NULL) {
    ctx->path_only = path_only;
  }

  return 0;
}

int wst_free(wst_path_ctx* ctx) {
  for (int i = 0; i < ctx->path_len; i++) {
    free(ctx->paths[i]);
  }
  free(ctx->paths);
  free(ctx->cmd_output);
  free(ctx);

  return 0;
}

int append_pointer(char** str1, char* str2) {
  if (str2 == NULL) {
    return -1;
  }

  if (*str1 == NULL) {
    char* new_str1 = (char*) malloc(strlen(str2) + 2);
    if (new_str1 == NULL) return -1;

    // Copy file to str1
    unsigned int i = 0;
    while (*str2 != '\0') {
      new_str1[i] = *str2;
      str2++;
      i++;
    }

    *str1 = new_str1;

    return 0;
  }

  // Create a new pointer, this will be the pointer to the new str1
  char* new_str1 = (char*) malloc(strlen(*str1) + strlen(str2) + 2);
  if (new_str1 == NULL) return -1;

  strcpy(new_str1, *str1);
  free(*str1);

  size_t str1_len = strlen(new_str1) - 1;

  // Copy file to str1
  unsigned int i = str1_len + 1;
  while (*str2 != '\0') {
    new_str1[i] = *str2;
    str2++;
    i++;
  }

  // Remove the trailing '/', and add the null terminator
  if (new_str1[i-1] == '/') {
    new_str1[i-1] = '\0';
  } else {
    new_str1[i] = '\0';
  }

  // Set the return pointer
  *str1 = new_str1;

  return 0;
}

char* wst_whereis(wst_path_ctx* ctx, const char* prog_name) {
  struct stat info;
  int found = 1;
  struct dirent *dir;
  DIR *d;

  // Make sure the output pointer is NULL
  ctx->cmd_output = NULL;
  char* output_str = NULL;

  for (int i = 0; i < ctx->path_len; i++) {
    d = opendir(ctx->paths[i]);
    if (d != NULL) {
      while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, prog_name) == 0) {
          char full_file_path[256];
          full_file_path[0] = '\0';
          strcat(full_file_path, ctx->paths[i]);
          strcat(full_file_path, "/");
          strcat(full_file_path, prog_name);

          if (output_str != NULL) append_pointer(&output_str, "\n");

          if (ctx->path_only) {
            append_pointer(&output_str, full_file_path);
            //printf("%s", full_file_path);
          } else {
            append_pointer(&output_str, prog_name);
            append_pointer(&output_str, ": ");
            append_pointer(&output_str, full_file_path);
            //printf("%s: %s", prog_name, full_file_path);
          }
          if (lstat(full_file_path, &info) != 0) {
            perror("lstat");
            return NULL;
          }

          if (S_ISLNK(info.st_mode) && !ctx->path_only) {
            char symlink_path[512];
            //symlink_path = (char*) malloc(512); // TODO: fix this malloc
            ssize_t len = readlink(full_file_path, symlink_path, sizeof(symlink_path));
//            if (len != -1) {
//              symlink_path[len] = '\0';
//            } else {
//              printf("unable to fine link");
//            }
            //printf(" -> %s", symlink_path);
//            sprintf(output_str, "%s -> %s", output_str, symlink_path);
            append_pointer(&output_str, " -> ");
            append_pointer(&output_str, symlink_path);
//            free(symlink_path);
          }

          //printf("\n");
          found = 0;
        }
      }
      closedir(d);
      if (ctx->path_only && found == 0) break;
    }
  }

  ctx->cmd_output = output_str;

  return ctx->cmd_output;
}



// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
