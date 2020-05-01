
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.     vc_debuginfo.c ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Vericheck, the FP profiler Valgrind tool,
   which does floating-point profiling.

   Copyright (C) 2020 Yohan Chatelain
      yohan.chatelain@gmail.com

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, see <http://www.gnu.org/licenses/>.

   The GNU General Public License is contained in the file COPYING.
*/

#include "vc_debuginfo.h"

#include "pub_tool_libcprint.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_threadstate.h"

#define UNK_STR "???"
#define UNK_LINE 0

DebugInfo* getDebugInfoTidAt(ThreadId tid, Addr addr) {

  DebugInfo *di = (DebugInfo*)VG_(malloc)("init.di", sizeof(DebugInfo));

  di->lib = UNK_STR;
  di->dir = UNK_STR;
  di->function = UNK_STR;
  di->file = UNK_STR;
  di->line = 0;
  di->isEntry = False;
  
  if (addr == 0) {
    VG_(get_StackTrace)(tid, &addr, 0, NULL, NULL, 0);
  }

  const HChar *lib;
  const HChar *dir;
  const HChar *file;
  const HChar *function;
  UInt line = 0;
  
  DiEpoch ep = VG_(current_DiEpoch)();
  Bool found_file_line = VG_(get_filename_linenum)(ep, addr, &file, &dir, &line);
  Bool found_function = VG_(get_fnname)(ep, addr, &function);
  Bool found_obj = VG_(get_objname)(ep, addr, &lib);

  if (found_obj) {
    di->lib = lib;
  }
  if (found_function) {
    di->function = function;
    di->isEntry = VG_(get_fnname_if_entry)(ep, addr, &function);
  }
    
  if (found_file_line) {
    di->dir = dir;
    di->file = file;
    di->line = line;
  }

  return di;
}

DebugInfo* getDebugInfoAt(Addr addr) {
  return getDebugInfoTidAt(VG_(get_running_tid)(), addr);  
}

DebugInfo* getDebugInfo(void) {
  return getDebugInfoAt(0);
}
