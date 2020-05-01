
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.     vc_debuginfo.h ---*/
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

#ifndef __VC_DEBUGINFO_H__
#define __VC_DEBUGINFO_H__

#include "pub_tool_basics.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_stacktrace.h"
#include "pub_tool_debuginfo.h"

/* Structure that encapsulates debug info             */
/* - lib     : Name of the object (library or binary) */
/* - dir     : Path to the lib                        */
/* - file    : Name of the file                       */
/* - function: Name of the function                   */
/* - line    : Line of the instruction                */
/* - isEntry : True if it's the first instruction     */
/*             of the corresponding function          */
typedef struct _DebugInfo DebugInfo;
struct _DebugInfo {
  HChar *lib;
  HChar *dir;
  HChar *file;
  HChar *function;
  UInt *line;
  Bool isEntry;
};

DebugInfo* getDebugInfoTidAt(ThreadId tid, Addr addr);
DebugInfo* getDebugInfoAt(Addr addr);
DebugInfo* getDebugInfo(void);

#endif /* __VC_DEBUGINFO_H__ */
