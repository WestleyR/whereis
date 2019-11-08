// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/whereis
// date: Nov 9, 2019
// version-1.0.1
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
#include <getopt.h>

#define SCRIPT_VERSION "v1.0.1, Nov 9, 2019"

void print_version() {
  printf("%s\n", SCRIPT_VERSION);
}

void print_usage(const char* script_name) {
  printf("Usage: %s [option] <command>\n", script_name);
  printf("\n");
  printf("Options:\n");
  printf("  --path-only, -m  only print the command path\n");
  printf("  --help, -h       print this help menu\n");
  printf("  --version, -V    print the version for %s\n", script_name);
  printf("\n");
  printf("Source code: https://github.com/WestleyR/whereis\n");
}

int find_command(char **path, int path_len, const char* prog_name, int path_only) {
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

int main(int argc, char** argv) {
  int opt = 0;
  int path_only = 1;

  static struct option long_options[] = {
    {"path-only", no_argument, 0, 'm'},
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, 0, 0}
  };

  while ((opt = getopt_long(argc, argv, "mhV", long_options, 0)) != -1) {
    switch (opt) {
      case 'm':
        path_only = 0;
        break;

      case 'V':
        print_version();
        return(0);

      case 'h':
        print_usage(argv[0]);
        return(0);

      default:
        return(22);
    }
  }

  char* pathenv = getenv("PATH");
  char spliter[] = ":";

  int p = 0;
  char** path;
  path = (char **) malloc(50);
  path[0] = (char*) malloc(50);
  if (path == NULL) {
    printf("malloc failed!\n");
    perror("malloc");
    return(1);
  }

  char *pathname = strtok(pathenv, spliter);
  while (pathname != NULL) {
    if (p == 0) {
      strcpy(path[p], pathname);
      p++;
    }

    int match = 0;
    for (int i = 0; i < p; i++) {
      if (strcmp(path[i], pathname) == 0) {
        match = 1;
      }
    }
    if (match != 1) {
      path[p] = (char*) malloc(50);
      strcpy(path[p], pathname);
      p++;
    }

    pathname = strtok(NULL, spliter);
  }

  if (optind < argc) {
    if (argc - optind > 1) {
      print_usage(argv[0]);
      return(22);
    }
    int found = find_command(path, p, argv[optind], path_only);
    if (found != 0) {
      return(1);
    }
  } else {
    print_usage(argv[0]);
    return(22);
  }

  free(path);

  return(0);
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
