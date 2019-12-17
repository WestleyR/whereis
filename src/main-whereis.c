// created by: WestleyR
// email: westleyr@nym.hush.com
// https://github.com/WestleyR/whereis
// date: Dec 17, 2019
// version-1.1.0
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

#include "whereis_cmd.h"
#include "clean_path_env.h"

#define SCRIPT_VERSION "v1.1.0, Dec 17, 2019"

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

  char* pathenv = getenv("PATH");

  char** path;
  path = (char **) malloc(50);
  path[0] = (char*) malloc(50);
  if (path == NULL) {
    printf("malloc failed!\n");
    return(1);
  }

  int plen = clean_path_env(path, pathenv);
  if (plen == -1) {
    fprintf(stderr, "Failed to clean path\n");
  }

  int not_found = 0;

  if (optind < argc) {
    for (int i = optind; i < argc; i++) {
      int found = whereis_cmdp(path, plen, argv[i], path_only);
      if (found != 0) {
        not_found = 1;
      }
    }
  } else {
    print_usage(argv[0]);
    return(22);
  }

  free(path);

  return(not_found);
}

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
