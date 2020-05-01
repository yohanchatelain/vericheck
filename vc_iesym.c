
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.         vc_iesym.c ---*/
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

#include "pub_tool_libcassert.h"
#include "pub_tool_machine.h"
#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_stacktrace.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_debuginfo.h"
#include "pub_tool_threadstate.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_seqmatch.h"

#include "vc_iesym.h"
#include "vc_utils.h"
#include "vc_debuginfo.h"

static ULong nbIgnoredLibs = 0;
static const HChar* ignoredLibsDefault[] = {"*/ld-*",
					    "*/libc-*",
					    "*libdl-*",
					    "/libmpfr.*"};

static ULong nbIgnoredFunctions = 0;
static const HChar* ignoredFunctionsDefault[] = {"???","__libc*","_start"};

/* op = {add, sub, mul, div} */
/* type = {float, double}    */
/* _interflop_{op}_{type}    */
static const HChar* interflopIncludedLibs[] = {"*/libinterflop_*"};
static const SizeT nbInterflopIncludedLibs = SIZE_ARRAY(interflopIncludedLibs);

static const HChar* interflopExcludedFiles[] = {"logger.c",
					       "options.c",
						"tinymt64.*",
						"printf_specifier.c"};

static const SizeT nbInterflopExcludedFiles = SIZE_ARRAY(interflopExcludedFiles);
 
static const HChar* interflopExcludedFunctions[] = {"*logger*",
						    "_set_seed_default",
						    "init_context",
						    "interflop_init",
						    "parse_opt",
						    "print_information_header",
						    "_mca_inexact*",
						    "tinymt64*",
						    "vfc_init",
						    "vfc_atexit",
						    "_floatadd",
						    "_floatsub",
						    "_floatmul",
						    "_floatdiv",
						    "_doubleadd",
						    "_doublesub",
						    "_doublemul",
						    "_doublediv",
						    "_fast_pow2_binary64",
						    "_fast_pow2_binary128"};

static const SizeT nbInterflopExcludedFunctions = SIZE_ARRAY(interflopExcludedFunctions);

static const HChar* interflopIncludedFunctions[] = {"*_interflop_*"};
static const SizeT nbInterflopIncludedFunctions = SIZE_ARRAY(interflopIncludedFunctions);

static const HChar* interflopExcludedLibs[] = {"**"};
static const SizeT nbInterflopExcludedLibs = SIZE_ARRAY(interflopIncludedLibs);


static HChar** ignoredLibs = NULL;
static HChar** ignoredFunctions = NULL;

void init_ignored_libs_default() {
  Int i;

  nbIgnoredLibs = sizeof(ignoredLibsDefault)/sizeof(HChar*);
  ignoredLibs = (HChar**)VG_(malloc)("init.ign.libs", nbIgnoredLibs*sizeof(HChar*));
  for (i = 0; i < nbIgnoredLibs; i++) {
    ignoredLibs[i] = (HChar*)VG_(malloc)("init.ign.libs.str",sizeof(HChar));
    VG_(strcpy)(ignoredLibs[i], ignoredLibsDefault[i]);
  }

  /* VG_(dmsg)("Ignored libs initiliazed with:\n"); */
  /* for (i = 0; i < nbIgnoredLibs; i++) { */
  /*   VG_(dmsg)("... %s\n",ignoredLibs[i]); */
  /* } */
  
  nbIgnoredFunctions = sizeof(ignoredFunctionsDefault)/sizeof(HChar*);
  ignoredFunctions = (HChar**)VG_(malloc)("init.ign.fun", nbIgnoredFunctions*sizeof(HChar*));
  for (i = 0; i < nbIgnoredFunctions; i++) {
    ignoredFunctions[i] = (HChar*)VG_(malloc)("init.ign.fun.str",sizeof(HChar));
    VG_(strcpy)(ignoredFunctions[i], ignoredFunctionsDefault[i]);
  }

  /* VG_(dmsg)("Ignored functions initiliazed with:\n"); */
  /* for (i = 0; i < nbIgnoredFunctions; i++) { */
  /*   VG_(dmsg)("... %s\n",ignoredFunctions[i]); */
  /* } */

  
}

Bool isStrInList(HChar **libList, const SizeT size, const HChar *lib, const HChar *dmsg);
Bool isStrInList(HChar **libList, const SizeT size, const HChar *lib, const HChar *dmsg) {
  Int i;
  Bool isIn = False;  
  for (i = 0; i < size; i++) {
    isIn |= VG_(string_match)(libList[i], lib); 
    if (dmsg) {
      VG_(dmsg)("(%s) [%s] %s in %s\n", dmsg,
		(VG_(string_match)(libList[i], lib)) ? "found" : "not found",
		libList[i], lib);
    }
 }
  return isIn;
}

InstType get_InstType(const DebugInfo *di) {

  if (VG_(strcmp)(di->function,"???") == 0) {
    return INST_IGNORE;
  }
  
  /* Check if the lib must be ignored */
  Bool isIgnoredLib = isStrInList(ignoredLibs,
				  nbIgnoredLibs,
				  di->lib,
				  NULL);
  if (isIgnoredLib) {
    return INST_IGNORE;
  }  

  /* Check if the function must be ignored */
  Bool isIgnoredFunction = isStrInList(ignoredFunctions,
				       nbIgnoredFunctions,
				       di->function, NULL);
  if (isIgnoredFunction) {
    return INST_IGNORE;
  }  

  /* Check that is not an excluded file of interflop */  
  Bool isExcludedFile = isStrInList(interflopExcludedFiles,
				    nbInterflopExcludedFiles,
				    di->file, NULL);
  if (isExcludedFile) {
    return INST_IGNORE;
  }

  /* Check that is not an excluded function of interflop */  
  Bool isInterflopExcludedFunction = isStrInList(interflopExcludedFunctions,
						 nbInterflopExcludedFunctions,
						 di->function, NULL);
  if (isInterflopExcludedFunction) {
    return INST_IGNORE;
  }
    

  /* Check if the function belongs to interflop */
  Bool isInterflopIncludedFunction = isStrInList(interflopIncludedFunctions,
					 nbInterflopIncludedFunctions,
					 di->function, NULL);
  
  
  if (isInterflopIncludedFunction && di->isEntry) {
    return INST_INTERFLOP;
  } 

  /* Check if the lib belongs to interflop */
  Bool isInterflopIncludedLib = isStrInList(interflopIncludedLibs,
  					    nbInterflopIncludedLibs,
  					    di->lib, NULL);
  if (isInterflopIncludedLib) {
    return INST_INTERFLOP;
  }

  return INST_IEEE;
  
}   

