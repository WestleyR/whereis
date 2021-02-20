//
//  main-whereis.c
//  whereis - simple whereis command that works! (for macOS)
//
// Created by WestleyR 2019-11-05
// Source code: https://github.com/WestleyR/whereis
//
// Copyright (c) 2019-2021 WestleyR. All rights reserved.
// This software is licensed under a BSD 3-Clause Clear License.
// Consult the LICENSE file that came with this software regarding
// your rights to distribute this software.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>

#include "paths.h"

#define SCRIPT_VERSION "1.1.0"

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

  //char** path = NULL;
  //int plen = wst_get_path(path);
  //if (plen == -1) {
  //  fprintf(stderr, "Failed to clean path\n");
  //}

  if (argc <= optind) {
    print_usage(argv[0]);
    return 22;
  }

  wst_path_ctx* path_ctx = wst_get_path();
  int not_found = 0;

  for (int i = optind; i < argc; i++) {
    char* cmd_path = wst_whereis(path_ctx, argv[i], path_only);
    if (cmd_path != NULL) {
      printf("%s\n", cmd_path);
      free(cmd_path);
    }
  }

  wst_free(path_ctx);

  return not_found;
}



// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
