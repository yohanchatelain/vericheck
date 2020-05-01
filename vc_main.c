
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.          vc_main.c ---*/
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

#include "valgrind.h"

#include "vc_fpops.h"
#include "vc_container.h"
#include "vc_utils.h"
#include "vc_iesym.h"
#include "vc_debuginfo.h"

/* Total number of visited functions */
static ULong nbVisitedFuns = 0;

/* IEEE FP Counter */
static FPCounter* ieeeFPC = NULL;

/* Interflop FP Counter */
static FPCounter* ifFPC = NULL;

/* IEEE Functions Container */
static FnContainer *ieeeFNC = NULL;
/* Interflop Functions Container */
static FnContainer *ifFNC = NULL;

/* The helper that is called from the instrumented code. */
static VG_REGPARM(1)
void increment_detail(ULong* detail, ULong inc)
{
  (*detail) += inc;
}

/* A helper that adds the instrumentation for a detail.  */
static void instrument_detail(IRSB* sb, FPCounter *counter, ULong fun_no, ULong increment)
{
   IRDirty* di;
   IRExpr** argv;

   argv = mkIRExprVec_2( mkIRExpr_HWord( (HWord)&(counter->data[fun_no]) ),
			 mkIRExpr_HWord( (HWord)increment )
			 );
   di = unsafeIRDirty_0_N( 1, "increment_detail",
                              VG_(fnptr_to_fnentry)( &increment_detail ),
                              argv);
   addStmtToIRSB( sb, IRStmt_Dirty(di) );
}

static void vc_post_clo_init(void)
{
  FnContainer_Init(&ieeeFNC);
  FnContainer_Init(&ifFNC);
  init_FPCounter(&ieeeFPC);
  init_FPCounter(&ifFPC);
  init_ignored_libs_default();
}

/* Primitive operations that are used in Unop, Binop, Triop and Qop IRExprs.*/
static
Bool vc_isPrimops(IRExpr *expr) {
  switch(expr->tag) {
  case Iex_Qop:
  case Iex_Triop:
  case Iex_Binop:
  case Iex_Unop:
    return True;
  default:
    /* ignore */
    return False;
  }  
}

/* Return the op assiocated to a primitive operation */
/* Raise an error if expr is not a primitive operation */
static
IROp vc_getOp(IRExpr *expr) {
  switch(expr->tag) {
  case Iex_Qop:
    return expr->Iex.Qop.details->op;
  case Iex_Triop:
    return expr->Iex.Triop.details->op;
  case Iex_Binop:
    return expr->Iex.Binop.op;
  case Iex_Unop:
    return expr->Iex.Unop.op;
  default:
    VG_(tool_panic)("Not a primitive operation");
    return Iop_INVALID;
  }
}

/* Wrapper for instrumenting an expression */
/* Call the instrumentation with the right parameters */
/* depending on the instrumentation type (IEEE or INTERFLOP) */
static
void vc_instrumentExpr (IRSB* sb, const InstType it, const ULong funNo, const ULong inc)
{
  switch (it) {
  case INST_IEEE:
    instrument_detail(sb, ieeeFPC, funNo, inc);
    break;
  case INST_INTERFLOP:
    instrument_detail(sb, ifFPC, funNo, inc);
    break;
  default:
    VG_(tool_panic)("Unknown instrumentation type");
  }    
}

/* Return the function number associated to a debug information */
/* Only look the function name for the moment */
/* Could be extented to arbitrary stack trace level */
static
ULong get_funNo(FnContainer *T,
		const DebugInfo *di,
		FPCounter *FPC)

{
  ULong funNo;
  const ContainerKey key = getKey(di);
  if (FnContainer_HasObj(T, &key)) {
    funNo = FnContainer_ID(T, &key);
  } else {
    ContainerObj *obj = ContainerObj_New(T, &key, di);
    funNo = obj->ID;
    FnContainer_Insert(T, obj);
    increment_FPCounter(FPC);
  }
  return funNo;
}

/* Checks if the debug information has a function number */
static
Bool has_funNo(FnContainer *T,
	       const DebugInfo *di)

{
  ULong funNo;
  const ContainerKey key = getKey(di);
  if (FnContainer_HasObj(T, &key)) {
    return True;
  } else {
    return False;
  }
}

/* Wrapper that prints the StackTrace for the current tid */
static
void ppST(void) {
  VG_(get_and_pp_StackTrace)(VG_(get_running_tid)(), 256);
}


/* Instrumentation */
/* Three cases: */
/* - INST_IGNORE    : the statement is ignored */
/* - INST_IEEE      : the function does not belong to interflop. */
/*                    so each FP operations is instrumented. */
/*                    The instrumentation increments the FPCounter */
/*                    by the size of the operand (scalar or vector) */
/* - INST_INTERFLOP : the function belongs to interflop */
/*                    We only instrument the top-level function */
/*                    and not the FP themselves. Hence, _interflop_add_float */
/*		      counts as one FP operation no matter how many real FP */
/*                    are executed.
*/
static 
IRSB* vc_instrument ( VgCallbackClosure* closure,
                      IRSB* sbIn,
                      const VexGuestLayout* layout,
                      const VexGuestExtents* vge,
                      const VexArchInfo* archinfo_host,
                      IRType gWordTy, IRType hWordTy )
{

  UInt i;
  IRSB* sbOut = deepCopyIRSBExceptStmts(sbIn);

  const DebugInfo *di = getDebugInfo();
  DebugInfo *di_st = NULL;
  
  ULong funNo, sizeType;
  IROp op;
  InstType instType = get_InstType(di);
  
  /*Loop over instructions*/
  for (i = 0 ; i < sbIn->stmts_used ; i++) {
    IRStmt* st = sbIn->stmts[i];    
    switch (st->tag) {
    case Ist_IMark:
      if (instType == INST_INTERFLOP) {
	di_st = getDebugInfoAt(st->Ist.IMark.addr);
	if (di_st->isEntry && !has_funNo(ifFNC, di)) {
	  funNo = get_funNo(ifFNC, di, ifFPC);
	  vc_instrumentExpr(sbOut, instType, funNo, 1);
	}
      }
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
      break;
    case Ist_WrTmp:
      if ((instType == INST_IEEE) && vc_isPrimops(st->Ist.WrTmp.data)) {	
	op = vc_getOp(st->Ist.WrTmp.data);
	if (vc_isArithmeticOpF(op)) {
	  funNo = get_funNo(ieeeFNC, di, ieeeFPC);
	  sizeType = vc_getSizeArithmeticOp(op);
	  vc_instrumentExpr(sbOut, instType, funNo, sizeType);
	}
      }
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
      break;
    case Ist_Dirty: /* Call */
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
      break;
    case Ist_Exit:
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
      break;
    default:
      addStmtToIRSB(sbOut, sbIn->stmts[i]);
    }
  }

  return sbOut;
}

/* Count the total number of FP for a Counter */
static ULong countFP(FnContainer *FNC, FPCounter *FPC) {
  Word size_FNC = FnContainer_Size(FNC);
  tl_assert(size_FNC == FPC->size);

  ULong nb_fp_total = 0;

  FnContainer_ResetIterator(FNC);

  ContainerObj *it = NULL;
  while ( (it = FnContainer_Next(FNC)) ) {
    nb_fp_total += FPC->data[it->ID];
  }  
  return nb_fp_total;
}

/* Pretty printer for fp counter */
static void ppFP(const HChar *name, FnContainer *FNC, FPCounter *FPC) {
  Word size_FNC = FnContainer_Size(FNC);
  tl_assert(size_FNC == FPC->size);
  VG_(umsg)("%llu %s functions visited\n", FPC->size, name);
  VG_(umsg)("-------------------------\n");

  FnContainer_ResetIterator(FNC);

  ContainerObj *it = NULL;
  while ( (it = FnContainer_Next(FNC)) ) {
    VG_(umsg)("\t* %s -> %s : %llu\n", it->libName, it->key, FPC->data[it->ID]);
  }
}

static void vc_fini(Int exitcode)
{
  
  nbVisitedFuns = ieeeFPC->size + ifFPC->size;
  ULong ieeeFP = countFP(ieeeFNC, ieeeFPC);
  ULong ifFP = countFP(ifFNC, ifFPC);

  ppFP("IEEE", ieeeFNC, ieeeFPC);
  VG_(umsg)("-------------------------\n");
  VG_(umsg)("IEEE FP: %llu\n\n", ieeeFP);

  ppFP("Interflop", ifFNC, ifFPC);
  VG_(umsg)("-------------------------\n");
  VG_(umsg)("Interflop FP: %llu\n\n", ifFP);

  Int ieee_ratio = -1, if_ratio = -1;
  Float den_ratio = ieeeFP + ifFP;
  
  if (den_ratio > 0.0) {
    ieee_ratio = (ieeeFP / den_ratio) * 100;
    if_ratio = (ifFP / den_ratio) * 100;    
    VG_(umsg)("IEEE FP ratio: %d%%\n", ieee_ratio);
    VG_(umsg)("Interflop FP ratio: %d%%\n", if_ratio);
  } else {
    VG_(umsg)("No functions visited\n");
  }

  FnContainer_Free(&ieeeFNC);
  FnContainer_Free(&ifFNC);
}

static void vc_pre_clo_init(void)
{
   VG_(details_name)            ("Vericheck");
   VG_(details_version)         ("0.0.1");
   VG_(details_description)     ("Floating-point profiler");
   VG_(details_copyright_author)(
      "Copyright (C) 2020, and GNU GPL'd, by Yohan Chatelain.");
   VG_(details_bug_reports_to)  (VG_BUGS_TO);

   VG_(details_avg_translation_sizeB) ( 275 );

   VG_(basic_tool_funcs)        (vc_post_clo_init,
                                 vc_instrument,
                                 vc_fini);

   /* No needs, no core events to track */
}

VG_DETERMINE_INTERFACE_VERSION(vc_pre_clo_init)

/*--------------------------------------------------------------------*/
/*--- end                                                          ---*/
/*--------------------------------------------------------------------*/
