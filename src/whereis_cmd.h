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

int whereis_cmdp(char **path, int path_len, const char* prog_name, int path_only);

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
