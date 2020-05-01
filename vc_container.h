
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.         vc_array.h ---*/
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

#ifndef __VC_CONTAINER_H__
#define __VC_CONTAINER_H__

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_oset.h"

/* This module implements the containers used to store                         */
/* the visited functions and the floating-point counters.                      */
/*                                                                             */
/* It has two types:                                                           */
/*                                                                             */
/*   - The "FPCounter" type that count the number of floating-point            */
/*     operations for each number function (fun_no). This "fun_no"             */
/*     is a unique ID for each function given by the associated "FnContainer". */
/*     "FPCounter" implements a simple dynamic array.                          */
/*                                                                             */
/*   - The "FnContainer" type that is a "OSetGen".                             */
/*     It holds object that records information about visited function         */
/*     and gives a unique ID for each these functions.                         */
/*     It uses two types:                                                      */
/*     - "ContainerKey": Abstract type that identifes each object.             */
/*     - "ContainerObj": The object contained by the "FnContainer".            */
/*       It records 4 fields:                                                  */
/*       * key     : Key is unique for each object and is used for             */
/*                   retreiving the object in the set.                         */
/*       * function: Name of the function                                      */
/*       * lib     : Name of the object file (library or binary)               */
/*                   that contains the function                                */
/*       * ID      : A unique number that identifies the object.               */
/*                   This ID is used to index the "FPCounter".                 */
       
/*--------------------------------------------------------------------*/
/*--- Types                                                        ---*/
/*--------------------------------------------------------------------*/

#define MAX_NAME_LENGTH 256
#define INIT_SIZE_FPCOUNTER 1024

typedef ULong* FPCounterData;

/* Structure that implements a simple dynamic array */
typedef struct _FPCounter FPCounter;
struct _FPCounter {
  FPCounterData data;
  ULong size;
  ULong capacity;
};

/* Container that record visited functions */
typedef HChar* ContainerKey;
typedef struct _ContainerObj ContainerObj;
struct _ContainerObj {
  ContainerKey key;
  HChar functionName[MAX_NAME_LENGTH];
  HChar libName[MAX_NAME_LENGTH];
  ULong ID;
};

typedef OSet FnContainer; 

/*--------------------------------------------------------------------*/
/*--- Creating and destroying FPCounter                            ---*/
/*--------------------------------------------------------------------*/

/* - Init: allocates a FPCounter.                  */
/*   Parameters:                                   */
/*     T : A pointer to a pointer of FPCounter     */
/*                                                 */
/* - Free: frees a FPCounter and its internal data */
/*   Parameters:                                   */
/*     T : A pointer to a pointer of FPCounter     */

void init_FPCounter(FPCounter **T);
void free_FPCounter(FPCounter **T);

/*--------------------------------------------------------------------*/
/*--- Operations on FPCounter                                      ---*/
/*--------------------------------------------------------------------*/

/* - Size: The number of element in FPCounter        */
/*                                                   */
/* - Increment: Increment the size of the FPCounter. */
/*              Resizes it if size >= capacity       */
  
ULong size_FPCounter(const FPCounter *T);
void increment_FPCounter(FPCounter *T);

/*--------------------------------------------------------------------*/
/*--- Creating and destroying FnCounter                            ---*/
/*--------------------------------------------------------------------*/

/* - Init: allocates a FnContainer.                  */
/*   Parameters:                                     */
/*     T : A pointer to a pointer of FPContainer     */
/*                                                   */
/* - Free: frees a FnContainer and its internal data */
/*   Parameters:                                     */
/*     T : A pointer to a pointer of FnContainer     */

void FnContainer_Init(FnContainer **T);
void FnContainer_Free(FnContainer **T);

/*--------------------------------------------------------------------*/
/*--- Utils functions for FPCounter                                ---*/
/*--------------------------------------------------------------------*/

/* - getKey:           Returns the key associated with a debug information */
/* - ContainerObj_New: Creates a new container object and associates       */
/*                     an unique ID to it.                                 */

ContainerKey getKey(const DebugInfo *di);
ContainerObj* ContainerObj_New(FnContainer *T,
			       const ContainerKey *key,
			       const DebugInfo *di);

/*--------------------------------------------------------------------*/
/*--- Operations on FPCounter                                      ---*/
/*--------------------------------------------------------------------*/

/* - HasObj: Determines if an object with this key is in the container */
/* - Size: Returns the number of elements in the container             */
/* - ID: Returns the ID associated with the given key                  */
/*       Raises an error if the key is not in the container            */
/* - Insert: Inserts the object in the container                       */
/* - ResetIterator: Reset the iterator associated with the container   */
/* - Next: Returns the next element in the container and increments    */
/*         its internal iterator                                       */

Bool FnContainer_HasObj(const FnContainer *T, const ContainerKey *key);
UInt FnContainer_Size(const FnContainer *T);
ULong FnContainer_ID(const FnContainer *T, const ContainerKey *key);
void FnContainer_Insert(FnContainer *T, ContainerObj *obj);
void FnContainer_ResetIterator(FnContainer *T);
ContainerObj* FnContainer_Next(FnContainer *T);

#endif /* __VC_CONTAINER_H__ */
