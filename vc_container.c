
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.         vc_array.c ---*/
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

#include "pub_tool_basics.h"
#include "pub_tool_tooliface.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_xarray.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_libcbase.h"
#include "pub_tool_oset.h"

#include "vc_container.h"
#include "vc_debuginfo.h"
 
#define INIT_SIZE_ARRAY 1024
#define INDEX_NOT_FOUND -1

static ULong ID_counter = 0;

#define UNINITIALIZED_ID -1


void init_FPCounter(FPCounter **T) {
  (*T) = (FPCounter*)VG_(malloc)("fpcounter.init", sizeof(FPCounter));
  (*T)->size = 0;
  (*T)->capacity = INIT_SIZE_FPCOUNTER;
  (*T)->data = (FPCounterData)VG_(calloc)("fpcounter.data.init", (*T)->capacity, sizeof(ULong));
}

void free_FPCounter(FPCounter **T) {
  if ((*T)->data) {
    VG_(free)((*T)->data);
  }
  VG_(free)(*T);
}

ULong size_FPCounter(const FPCounter *T) {
  return T->size;
}

void increment_FPCounter(FPCounter *T) {
  T->size++;
  if (T->size >= T->capacity) {
    T->capacity *= 2;
    T->data = (ULong*)VG_(realloc)("fpcounter.resize", T->data, sizeof(ULong)*T->capacity);     
  }   
}


Word cmpKey_FnContainer(const void* a, const void* b);
Word cmpKey_FnContainer(const void* a, const void* b) {
  const ContainerObj* obj_a = (const ContainerObj*)a;
  const ContainerObj* obj_b = (const ContainerObj*)b;
  return VG_(strcmp)(obj_a->key, obj_b->key);
}

ContainerKey getKey(const DebugInfo *di) {

  tl_assert(di->dir);
  tl_assert(di->file);
  tl_assert(di->function);

  SizeT sizeDir = VG_(strlen)(di->dir);
  SizeT sizeFile = VG_(strlen)(di->file);
  SizeT sizeFun = VG_(strlen)(di->function);
  SizeT sizeKey = sizeDir + sizeFile + sizeFun + 2;
  
  HChar *key = VG_(malloc)("key", sizeof(HChar)*(sizeKey));
    
  VG_(sprintf)(key, "%s/%s:%s", di->dir, di->file, di->function);

  return key;
}

ContainerObj *ContainerObj_New(FnContainer *T, const ContainerKey *key,
			       const DebugInfo *di) {
  ContainerObj *newObj = VG_(OSetGen_AllocNode)(T, sizeof(ContainerObj));
  newObj->key = *key;  
  VG_(strcpy)(newObj->functionName, di->function);
  VG_(strcpy)(newObj->libName, di->lib);
  newObj->ID = ID_counter;
  ID_counter++;
  /* VG_(dmsg)("New Obj: %s, %s, %s, %llu\n", *key, di->lib, di->function, ID_counter); */
  return newObj;
}
 
void FnContainer_Init(FnContainer **T) {
  *T = VG_(OSetGen_Create)(/*keyoff*/0,
			  cmpKey_FnContainer,
			  VG_(malloc),
			  "fnctn.init",
			  VG_(free));
}

void FnContainer_Free(FnContainer **T) {
  VG_(OSetGen_Destroy)(*T);
}

Bool FnContainer_HasObj(const FnContainer *T, const ContainerKey *key) {
  return VG_(OSetGen_Contains)(T, key);  
}

inline UInt FnContainer_Size(const FnContainer *T) {
  return VG_(OSetGen_Size)(T);
}

ContainerObj *get_FnContainer(const FnContainer *T, const ContainerKey *key);
inline ContainerObj *get_FnContainer(const FnContainer *T, const ContainerKey *key) {
  return VG_(OSetGen_Lookup)(T, key);
}

ULong FnContainer_ID(const FnContainer *T, const ContainerKey *key) {
  ContainerObj *obj = get_FnContainer(T, key);
  if (obj == NULL) {
    VG_(tool_panic)("FnContainer is looking for unknown key");
  } else {
    return obj->ID;
  }
}

void FnContainer_Insert(FnContainer *T, ContainerObj *obj) {
  VG_(OSetGen_Insert)(T, obj);
}

void FnContainer_ResetIterator(FnContainer *T) {
  VG_(OSetGen_ResetIter)(T);
}

ContainerObj* FnContainer_Next(FnContainer *T) {
  return VG_(OSetGen_Next)(T);
}
