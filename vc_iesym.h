
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.         vc_iesym.h ---*/
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

#ifndef __VC_IENAME_H__
#define __VC_IENAME_H__

/* This module checks the included/excluded symbols */

#define IGNORED_LIBS_SIZE_DEFAULT 128

/* Enum that defines the type of instrumentation */
/* INST_IGNORE: ignore that function */
/* INST_IEEE:  function have IEEE fp operations */
/* INST_INTERFLOP: function belongs to an inteflop library */
typedef enum _InstType InstType;
enum _InstType {
		INST_IGNORE = 0,
		INST_IEEE,
		INST_INTERFLOP
};

void init_ignored_libs_default(void);
Bool isExcludedLib(const HChar *lib);
InstType get_InstType(const DebugInfo *di);

#endif /* __VC_IENAME_H__ */
