//
//  paths.h
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

#ifndef WST_PATHS_H__
#define WST_PATHS_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "bool.h"
#include "catpath.h"

typedef struct wst_path_ctx wst_path_ctx;

wst_path_ctx* wst_get_path();
int wst_set_path_only(wst_path_ctx* ctx, bool path_only);
char* wst_whereis(wst_path_ctx* ctx, const char* prog_name);
int wst_free(wst_path_ctx* ctx);

#endif // WST_PATHS_H__

// vim: tabstop=2 shiftwidth=2 expandtab autoindent softtabstop=0
