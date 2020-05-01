
/*--------------------------------------------------------------------*/
/*--- Vericheck: The FP profiler Valgrind tool.         vc_fpops.c ---*/
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
#include "pub_tool_libcprint.h"
#include "pub_tool_libcbase.h"
#include "libvex_ir.h"

#include "vc_fpops.h"

/* Binary32 */

/* Scalar */

/* Unary */
Bool vc_isScalarUnaryOpF32(const IROp op) {
  switch (op) {
  case Iop_NegF32:
  case Iop_AbsF32:
    return True;
  default:
    return False;
  }
}

/* Binary */
Bool vc_isScalarBinaryOpF32(const IROp op) {
  switch (op) {
  case Iop_AddF32:
  case Iop_SubF32:
  case Iop_MulF32:
  case Iop_DivF32:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isCmpOpF32(const IROp op) {
  switch (op) {
  case Iop_CmpF32:
    return True;
  default:
    return False;
  }
}

/* Ternary */
Bool vc_isTernaryOpF32(const IROp op) {
  switch (op) {
  case Iop_MAddF32:
  case Iop_MSubF32:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF32(const IROp op) {
  switch (op) {
  case Iop_SqrtF32:
  case Iop_RecpExpF32:
    return True;
  default:
    return False;
  }
}

/* Lowest-Lane-Only */
/* Vector with one active element */

/* Binary */
Bool vc_isLLOx4BinaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Add32F0x4:
  case Iop_Sub32F0x4:
  case Iop_Mul32F0x4:
  case Iop_Div32F0x4:
    return True;
  default:
    return False;
  }  
}

/* Comparisons */
Bool vc_isLLOx4CmpOpF32(const IROp op) {
  switch (op) {
  case Iop_CmpEQ32F0x4:
  case Iop_CmpLT32F0x4:
  case Iop_CmpLE32F0x4:
  case Iop_CmpUN32F0x4:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isLLOx4IntrinsicOpF32(const IROp op) {
  switch (op) {
  case Iop_RecipEst32F0x4:
  case Iop_Sqrt32F0x4:
  case Iop_RSqrtEst32F0x4:
    return True;
  default:
    return False;
  }
}

/* Vector */

/* x2 */

/* Unary */
Bool vc_isVectorx2UnaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Neg32Fx2:
  case Iop_Abs32Fx2:
    return True;
  default:
    return False;
  }
}

/* Binary */
Bool vc_isVectorx2BinaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Add32Fx2:
  case Iop_Sub32Fx2:
  case Iop_Mul32Fx2:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isVectorx2CmpOpF32(const IROp op) {
  switch (op) {
  case Iop_CmpEQ32Fx2:
  case Iop_CmpGT32Fx2:
  case Iop_CmpGE32Fx2:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF32(const IROp op) {
  switch (op) {
  case Iop_RecipEst32Fx2: /* Vector Reciprocal EStimated */
  case Iop_RecipStep32Fx2:
  case Iop_RSqrtEst32Fx2:
  case Iop_RSqrtStep32Fx2:
    return True;
  default:
    return False;
  }
}

/* x4 */

/* Unary */
Bool vc_isVectorx4UnaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Neg32Fx4:
  case Iop_Abs32Fx4:
    return True;
  default:
    return False;
  }
}

/* Binary */
Bool vc_isVectorx4BinaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Add32Fx4:
  case Iop_Sub32Fx4:
  case Iop_Mul32Fx4:
  case Iop_Div32Fx4:
  case Iop_Add32F0x4:
  case Iop_Sub32F0x4:
  case Iop_Mul32F0x4:
  case Iop_Div32F0x4:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isVectorx4CmpOpF32(const IROp op) {
  switch (op) {
  case Iop_CmpEQ32Fx4:
  case Iop_CmpLT32Fx4:
  case Iop_CmpLE32Fx4:
  case Iop_CmpUN32Fx4:
  case Iop_CmpGT32Fx4:
  case Iop_CmpGE32Fx4:
  case Iop_CmpEQ32F0x4:
  case Iop_CmpLT32F0x4:
  case Iop_CmpLE32F0x4:
  case Iop_CmpUN32F0x4:
    return True;
  default:
    return False;
  }
}


/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF32(const IROp op) {
  switch (op) {
  case Iop_Sqrt32Fx4:
  case Iop_RecipEst32Fx4:
  case Iop_RecipStep32Fx4:
  case Iop_Scale2_32Fx4:
  case Iop_Log2_32Fx4:
  case Iop_Exp2_32Fx4:
  case Iop_RSqrtStep32Fx4:
  case Iop_RecipEst32F0x4:
  case Iop_Sqrt32F0x4:
  case Iop_RSqrtEst32F0x4:
    return True;
  default:
    return False;
  }
}

/* x8 */

/* Unary*/

/* Binary */
Bool vc_isVectorx8BinaryOpF32(const IROp op) {
  switch (op) {
  case Iop_Add32Fx8:
  case Iop_Sub32Fx8:
  case Iop_Mul32Fx8:
  case Iop_Div32Fx8:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isVectorx8IntrinsicOpF32(const IROp op) {
  switch (op) {
  case Iop_Sqrt32Fx8:
  case Iop_RSqrtEst32Fx8:
  case Iop_RecipEst32Fx8:
    return True;
  default:
    return False;
  }
}

/* Binary64 */

/* Scalar */

/* Unary */
Bool vc_isScalarUnaryOpF64(const IROp op) {
  switch (op) {
  case Iop_NegF64:
  case Iop_AbsF64:
    return True;
  default:
    return False;
  }
}

/* Binary */
Bool vc_isScalarBinaryOpF64(const IROp op) {
  switch (op) {
  case Iop_AddF64:
  case Iop_SubF64:
  case Iop_MulF64:
  case Iop_DivF64:
  case Iop_AddF64r32:
  case Iop_SubF64r32:
  case Iop_MulF64r32:
  case Iop_DivF64r32:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isCmpOpF64(const IROp op) {
  switch (op) {
  case Iop_CmpF64:
    return True;
  default:
    return False;
  }
}

/* Ternary */
Bool vc_isTernaryOpF64(const IROp op) {
  switch (op) {
  case Iop_MAddF64:
  case Iop_MSubF64:
  case Iop_MAddF64r32:
  case Iop_MSubF64r32:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF64(const IROp op) {
  switch (op) {
  case Iop_SqrtF64:
  case Iop_AtanF64:       /* FPATAN,  arctan(arg1/arg2)       */
  case Iop_Yl2xF64:       /* FYL2X,   arg1 * log2(arg2)       */
  case Iop_Yl2xp1F64:     /* FYL2XP1, arg1 * log2(arg2+1.0)   */
  case Iop_PRemF64:       /* FPREM,   non-IEEE remainder(arg1/arg2)    */
  case Iop_PRemC3210F64:  /* C3210 flags resulting from FPREM, :: I32 */
  case Iop_PRem1F64:      /* FPREM1,  IEEE remainder(arg1/arg2)    */
  case Iop_PRem1C3210F64: /* C3210 flags resulting from FPREM1, :: I32 */
  case Iop_ScaleF64:      /* FSCALE,  arg1 * (2^RoundTowardsZero(arg2)) */
  case Iop_SinF64:        /* FSIN */
  case Iop_CosF64:        /* FCOS */
  case Iop_TanF64:        /* FTAN */
  case Iop_2xm1F64:       /* 2^arg - 1.0 */
  case Iop_RSqrtEst5GoodF64:
  case Iop_RecpExpF64:
    return True;
  default:
    return False;
  }
}

/* Lowest-Lane-Only */

/* Binary */
Bool vc_isLLOx2BinaryOpF64(const IROp op) {
  switch (op) {
  case Iop_Add64F0x2:
  case Iop_Sub64F0x2:
  case Iop_Mul64F0x2:
  case Iop_Div64F0x2:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isLLOx2CompOpF64(const IROp op) {
  switch (op) {
  case Iop_CmpEQ64F0x2:
  case Iop_CmpLT64F0x2:
  case Iop_CmpLE64F0x2:
  case Iop_CmpUN64F0x2:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isLLOx2IntrinsicOpF64(const IROp op) {
  switch (op) {
  case Iop_Sqrt64F0x2:
    return True;
  default:
    return False;
  }
}

/* Vector */

/* 2x */

/* Unary */
Bool vc_isVectorx2UnaryOpF64(const IROp op) {
  switch (op) {
  case Iop_Neg64Fx2:
  case Iop_Abs64Fx2:
    return True;
  default:
    return False;
  }
}

/* Binary */
Bool vc_isVectorx2BinaryOpF64(const IROp op) {
  switch (op) {
  case Iop_Add64Fx2:
  case Iop_Sub64Fx2:
  case Iop_Mul64Fx2:
  case Iop_Div64Fx2:
  case Iop_Add64F0x2:
  case Iop_Sub64F0x2:
  case Iop_Mul64F0x2:
  case Iop_Div64F0x2:
    return True;
  default:
    return False;
  }
}

/* Comparisons */
Bool vc_isVectorx2CompOpF64(const IROp op) {
  switch (op) {
  case Iop_CmpEQ64Fx2:
  case Iop_CmpLT64Fx2:
  case Iop_CmpLE64Fx2:
  case Iop_CmpUN64Fx2:
  case Iop_CmpEQ64F0x2:
  case Iop_CmpLT64F0x2:
  case Iop_CmpLE64F0x2:
  case Iop_CmpUN64F0x2:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF64(const IROp op) {
  switch (op) {
  case Iop_Sqrt64Fx2:
  case Iop_Scale2_64Fx2:
  case Iop_Log2_64Fx2:
  case Iop_RecipEst64Fx2:
  case Iop_RecipStep64Fx2:
  case Iop_RSqrtEst64Fx2:
  case Iop_RSqrtStep64Fx2:
  case Iop_Sqrt64F0x2:
    return True;
  default:
    return False;
  }
}

/* x4 */

/* Unary */

/* Binary */
Bool vc_isVectorx4BinaryOpF64(const IROp op) {
  switch (op) {
  case Iop_Add64Fx4:
  case Iop_Sub64Fx4:
  case Iop_Mul64Fx4:
  case Iop_Div64Fx4:
    return True;
  default:
    return False;
  }
}

/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF64(const IROp op) {
  switch (op) {
  case Iop_Sqrt64Fx4:
    return True;
  default:
    return False;
  }
}

/* Common functions */

/* Binary32 */

/* Arithmetics op tests funtions */

Bool vc_isScalarArithmeticOpF32(const IROp op) {
  if (vc_isScalarUnaryOpF32(op)) {
    return True;
  } else if (vc_isScalarBinaryOpF32(op)) {
    return True;
  } else if (vc_isTernaryOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorx2ArithmeticOpF32(const IROp op) {
  if (vc_isVectorx2UnaryOpF32(op)) {
    return True;
  } else if (vc_isVectorx2BinaryOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorx4ArithmeticOpF32(const IROp op) {
  if (vc_isVectorx4UnaryOpF32(op)) {
    return True;
  } else if (vc_isVectorx4BinaryOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorx8ArithmeticOpF32(const IROp op) {
  if (vc_isVectorx4BinaryOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isLLOArithmeticOpF32(const IROp op) {
  if (vc_isLLOx4BinaryOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorArithmeticOpF32(const IROp op) {
  if (vc_isVectorx2ArithmeticOpF32(op)) {
    return True;
  } else if (vc_isVectorx2ArithmeticOpF32(op)) {
    return True;
  } else if (vc_isVectorx4ArithmeticOpF32(op)) {
    return True;
  } else if (vc_isVectorx8ArithmeticOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isArithmeticOpF32(const IROp op) {
  if (vc_isScalarArithmeticOpF32(op)) {
    return True;
  } else if (vc_isVectorArithmeticOpF32(op)) {
    return True;
  } else {
    return False;
  }
}

/* Binary64 */

/* Arithmetics op tests funtions */

Bool vc_isScalarArithmeticOpF64(const IROp op) {
  if (vc_isScalarUnaryOpF64(op)) {
    return True;
  } else if (vc_isScalarBinaryOpF64(op)) {
    return True;
  } else if (vc_isTernaryOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorx2ArithmeticOpF64(const IROp op) {
  if (vc_isVectorx2UnaryOpF64(op)) {
    return True;
  } else if (vc_isVectorx2BinaryOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorx4ArithmeticOpF64(const IROp op) {
  if (vc_isVectorx4BinaryOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isVectorArithmeticOpF64(const IROp op) {
  if (vc_isVectorx2ArithmeticOpF64(op)) {
    return True;
  } else if (vc_isVectorx4ArithmeticOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isLLOArithmeticOpF64(const IROp op) {
  if (vc_isLLOx2BinaryOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isArithmeticOpF64(const IROp op) {
  if (vc_isScalarArithmeticOpF64(op)) {
    return True;
  } else if (vc_isVectorArithmeticOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

Bool vc_isArithmeticOpF(const IROp op) {
  if (vc_isArithmeticOpF32(op)) {
    return True;
  } else if (vc_isArithmeticOpF64(op)) {
    return True;
  } else {
    return False;
  }
}

/* Type operators */

Bool vc_isAddOp(const IROp op) {
  switch (op) {
  case Iop_AddF32:
  case Iop_Add32Fx2:
  case Iop_Add32Fx4:
  case Iop_Add32F0x4:
  case Iop_Add32Fx8:
  case Iop_AddF64:
  case Iop_AddF64r32:
  case Iop_Add64Fx2:
  case Iop_Add64F0x2:
  case Iop_Add64Fx4:
    return True;
  default:
    return False;
  }
}

Bool vc_isSubOp(const IROp op) {
  switch (op) {
  case Iop_SubF32:
  case Iop_Sub32Fx2:
  case Iop_Sub32Fx4:
  case Iop_Sub32F0x4:
  case Iop_Sub32Fx8:
  case Iop_SubF64:
  case Iop_SubF64r32:
  case Iop_Sub64Fx2:
  case Iop_Sub64F0x2:
  case Iop_Sub64Fx4:
    return True;
  default:
    return False;
  }
}

Bool vc_isMulOp(const IROp op) {
  switch (op) {
  case Iop_MulF32:
  case Iop_Mul32Fx2:
  case Iop_Mul32Fx4:
  case Iop_Mul32F0x4:
  case Iop_Mul32Fx8:
  case Iop_MulF64:
  case Iop_MulF64r32:
  case Iop_Mul64Fx2:
  case Iop_Mul64F0x2:
  case Iop_Mul64Fx4:
    return True;
  default:
    return False;
  }
}

Bool vc_isDivOp(const IROp op) {
  switch (op) {
  case Iop_DivF32:
  case Iop_Div32Fx4:
  case Iop_Div32F0x4:
  case Iop_Div32Fx8:
  case Iop_DivF64:
  case Iop_DivF64r32:
  case Iop_Div64Fx2:
  case Iop_Div64F0x2:
  case Iop_Div64Fx4:
    return True;
  default:
    return False;
  }
}

Bool vc_isFmaOp(const IROp op) {
  switch (op) {
  case Iop_MAddF32:
  case Iop_MSubF32:
  case Iop_MAddF64:
  case Iop_MSubF64:
  case Iop_MAddF64r32:
  case Iop_MSubF64r32:
    return True;
  default:
    return False;
  }
}

/* Return the size of the operands */
ULong vc_getSizeArithmeticOp(const IROp op) {
  if (vc_isScalarArithmeticOpF32(op) || vc_isScalarArithmeticOpF64(op)
      || vc_isLLOArithmeticOpF32(op) || vc_isLLOArithmeticOpF64(op)) {
    return 1;
  } else if (vc_isVectorx2ArithmeticOpF32(op) || vc_isVectorx2ArithmeticOpF64(op)) {
    return 2;
  } else if (vc_isVectorx4ArithmeticOpF32(op) || vc_isVectorx4ArithmeticOpF64(op)) {
    return 4;
  } else if (vc_isVectorx8ArithmeticOpF32(op)) {
    return 8;
  } else {
    VG_(tool_panic)("Unknown arithmetic operator size");
  }
}

/* Name functions */

const HChar *vc_getTypeNameOp(const IROp op) {
  if (vc_isArithmeticOpF32(op)) {
    return "b32";
  } else if (vc_isArithmeticOpF64(op)) {
    return "b64";
  } else {
    VG_(umsg)("Unkonw type name %u",op);
    return "ukn";
  }
}

const HChar *vc_getDimTypeNameOp(const IROp op) {
  if (vc_isScalarArithmeticOpF32(op) ||
      vc_isScalarArithmeticOpF64(op)) {
    return "x1";
  } else if (vc_isVectorx2ArithmeticOpF32(op) ||
	     vc_isVectorx2ArithmeticOpF64(op)) {
    return "x2";
  } else if (vc_isVectorx4ArithmeticOpF32(op) ||
	     vc_isVectorx4ArithmeticOpF64(op)) {
    return "x4";
  } else if (vc_isVectorx8ArithmeticOpF32(op)) {
    return "x8";
  } else {
    VG_(umsg)("Unkonw operation dimension %u",op);
    return "ukn";
  }
}

const HChar *vc_getArithmeticNameOp(const IROp op) {
  if (vc_isAddOp(op)) {
    return "add";
  } else if (vc_isSubOp(op)) {
    return "sub";
  } else if (vc_isMulOp(op)) {
    return "mul";
  } else if (vc_isDivOp(op)) {
    return "div";
  } else if (vc_isFmaOp(op)) {
    return "fma";
  } else {
    VG_(umsg)("Unknown operation type %u",op);
    return "unk";
  }
}

void vc_getNameOp(const IROp op, HChar *name) {
  const HChar *type = vc_getTypeNameOp(op);
  const HChar *dim = vc_getDimTypeNameOp(op);
  const HChar *sym = vc_getArithmeticNameOp(op);
  VG_(strcat)(name, type);
  VG_(strcat)(name, sym);
  VG_(strcat)(name, dim);
}

const HChar* vc_getRawNameOp(const IROp op) {
  switch (op) {
  case Iop_NegF32:
    return "NegF32";
  case Iop_AbsF32:
    return "AbsF32";
  case Iop_AddF32:
    return "AddF32";
  case Iop_SubF32:
    return "SubF32";
  case Iop_MulF32:
    return "MulF32";
  case Iop_DivF32:
    return "DivF32";
  case Iop_CmpF32:
    return "CmpF32";
  case Iop_MAddF32:
    return "MAddF32";
  case Iop_MSubF32:
    return "MSubF32";
  case Iop_SqrtF32:
    return "SqrtF32";
  case Iop_RecpExpF32:
    return "RecpExpF32";
  case Iop_Neg32Fx2:
    return "Neg32Fx2";
  case Iop_Abs32Fx2:
    return "Abs32Fx2";
  case Iop_Add32Fx2:
    return "Add32Fx2";
  case Iop_Sub32Fx2:
    return "Sub32Fx2";
  case Iop_Mul32Fx2:
    return "Mul32Fx2";
  case Iop_CmpEQ32Fx2:
    return "CmpEQ32Fx2";
  case Iop_CmpGT32Fx2:
    return "CmpGT32Fx2";
  case Iop_CmpGE32Fx2:
    return "CmpGE32Fx2";
  case Iop_RecipEst32Fx2:
    return "RecipEst32Fx2";
  case Iop_RecipStep32Fx2:
    return "RecipStep32Fx2";
  case Iop_RSqrtEst32Fx2:
    return "RSqrtEst32Fx2";
  case Iop_RSqrtStep32Fx2:
    return "RSqrtStep32Fx2";
  case Iop_Neg32Fx4:
    return "Neg32Fx4";
  case Iop_Abs32Fx4:
    return "Abs32Fx4";
  case Iop_Add32Fx4:
    return "Add32Fx4";
  case Iop_Sub32Fx4:
    return "Sub32Fx4";
  case Iop_Mul32Fx4:
    return "Mul32Fx4";
  case Iop_Div32Fx4:
    return "Div32Fx4";
  case Iop_Add32F0x4:
    return "Add32F0x4";
  case Iop_Sub32F0x4:
    return "Sub32F0x4";
  case Iop_Mul32F0x4:
    return "Mul32F0x4";
  case Iop_Div32F0x4:
    return "Div32F0x4";
  case Iop_CmpEQ32Fx4:
    return "CmpEQ32Fx4";
  case Iop_CmpLT32Fx4:
    return "CmpLT32Fx4";
  case Iop_CmpLE32Fx4:
    return "CmpLE32Fx4";
  case Iop_CmpUN32Fx4:
    return "CmpUN32Fx4";
  case Iop_CmpGT32Fx4:
    return "CmpGT32Fx4";
  case Iop_CmpGE32Fx4:
    return "CmpGE32Fx4";
  case Iop_CmpEQ32F0x4:
    return "CmpEQ32Fx4";
  case Iop_CmpLT32F0x4:
    return "CmpLT32F0x4";
  case Iop_CmpLE32F0x4:
    return "CmpLE32Fx4";
  case Iop_CmpUN32F0x4:
    return "CmpUN32F0x4";
  case Iop_Sqrt32Fx4:
    return "Sqrt32Fx4";
  case Iop_RecipEst32Fx4:
    return "RecipEst32Fx4";
  case Iop_RecipStep32Fx4:
    return "RecipStep32Fx4";
  case Iop_Scale2_32Fx4:
    return "Scale2_32Fx4";
  case Iop_Log2_32Fx4:
    return "Log2_32Fx4";
  case Iop_Exp2_32Fx4:
    return "Exp2_32Fx4";
  case Iop_RSqrtStep32Fx4:
    return "RSqrtStep32Fx4";
  case Iop_RecipEst32F0x4:
    return "RecipEst32F0x4";
  case Iop_Sqrt32F0x4:
    return "Sqrt32F0x4";
  case Iop_RSqrtEst32F0x4:
    return "RSqrtEst32F0x4";
  case Iop_Add32Fx8:
    return "Add32Fx8";
  case Iop_Sub32Fx8:
    return "Sub32Fx8";
  case Iop_Mul32Fx8:
    return "Mul32Fx8";
  case Iop_Div32Fx8:
    return "Div32Fx8";
  case Iop_Sqrt32Fx8:
    return "Sqrt32Fx8";
  case Iop_RSqrtEst32Fx8:
    return "RSqrtEst32Fx8";
  case Iop_RecipEst32Fx8:
    return "RecipEst32Fx8";
  case Iop_NegF64:
    return "NegF64";
  case Iop_AbsF64:
    return "AbsF64";
  case Iop_AddF64:
    return "AddF64";
  case Iop_SubF64:
    return "SubF64";
  case Iop_MulF64:
    return "MulF64";
  case Iop_DivF64:
    return "DivF64";
  case Iop_AddF64r32:
    return "AddF64r32";
  case Iop_SubF64r32:
    return "SubF64r32";
  case Iop_MulF64r32:
    return "MulF64r32";
  case Iop_DivF64r32:
    return "DivF64r32";
  case Iop_CmpF64:
    return "CmpF64";
  case Iop_MAddF64:
    return "MAddF64";
  case Iop_MSubF64:
    return "MSubF64";
  case Iop_MAddF64r32:
    return "MAddF64r32";
  case Iop_MSubF64r32:
    return "MSubF64r32";
  case Iop_SqrtF64:
    return "SqrtF64";
  case Iop_AtanF64:
    return "AtanF64";
  case Iop_Yl2xF64:
    return "Yl2xF64";
  case Iop_Yl2xp1F64:
    return "Yl2xp1F64";
  case Iop_PRemF64:
    return "PRemF64";
  case Iop_PRemC3210F64:
    return "PRemC3210F64";
  case Iop_PRem1F64:
    return "PRem1F64";
  case Iop_PRem1C3210F64:
    return "PRem1C3210F64";
  case Iop_ScaleF64:
    return "ScaleF64";
  case Iop_SinF64:
    return "SinF64";
  case Iop_CosF64:
    return "CosF64";
  case Iop_TanF64:
    return "TanF64";
  case Iop_2xm1F64:
    return "2xm1F64";
  case Iop_RSqrtEst5GoodF64:
    return "RSqrtEst5GoodF64";
  case Iop_RecpExpF64:
    return "RecpExpF64";
  case Iop_Neg64Fx2:
    return "Neg64Fx2";
  case Iop_Abs64Fx2:
    return "Abs64Fx2";
  case Iop_Add64Fx2:
    return "Add64Fx2";
  case Iop_Sub64Fx2:
    return "Sub64Fx2";
  case Iop_Mul64Fx2:
    return "Mul64Fx2";
  case Iop_Div64Fx2:
    return "Div64Fx2";
  case Iop_Add64F0x2:
    return "Add64F0x2";
  case Iop_Sub64F0x2:
    return "Sub64F0x2";
  case Iop_Mul64F0x2:
    return "Mul64F0x2";
  case Iop_Div64F0x2:
    return "Div64F0x2";
  case Iop_CmpEQ64Fx2:
    return "CmpEQ64Fx2";
  case Iop_CmpLT64Fx2:
    return "CmpLT64Fx2";
  case Iop_CmpLE64Fx2:
    return "CmpLE64Fx2";
  case Iop_CmpUN64Fx2:
    return "CmpUN64Fx2";
  case Iop_CmpEQ64F0x2:
    return "CmpEQ64F0x2";
  case Iop_CmpLT64F0x2:
    return "CmpLT64F0x2";
  case Iop_CmpLE64F0x2:
    return "CmpLE64F0x2";
  case Iop_CmpUN64F0x2:
    return "CmpUN64F0x2";
  case Iop_Sqrt64Fx2:
    return "Sqrt64Fx2";
  case Iop_Scale2_64Fx2:
    return "Scale2_64Fx2";
  case Iop_Log2_64Fx2:
    return "Log2_64Fx2";
  case Iop_RecipEst64Fx2:
    return "RecipEst64Fx2";
  case Iop_RecipStep64Fx2:
    return "RecipStep64Fx2";
  case Iop_RSqrtEst64Fx2:
    return "RSqrtEst64Fx2";
  case Iop_RSqrtStep64Fx2:
    return "RSqrtStep64Fx2";
  case Iop_Sqrt64F0x2:
    return "Sqrt64F0x2";
  case Iop_Add64Fx4:
    return "Add64Fx4";
  case Iop_Sub64Fx4:
    return "Sub64Fx4";
  case Iop_Mul64Fx4:
    return "Mul64Fx4";
  case Iop_Div64Fx4:
    return "Div64Fx4";
  case Iop_Sqrt64Fx4:
    return "Sqrt64Fx4";
  default:
    
    VG_(umsg)("Unknown op %u\n", op);
    return "unk";
  }
}
