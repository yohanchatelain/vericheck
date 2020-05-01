
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

#ifndef __VC_FOPS_H__
#define __VF_FOPS_H__

#include "libvex_ir.h"

/* Arithmetic operations include {+,-,*,/} */

/******************************************/
/*                Binary32                */
/******************************************/

/*********************/
/*      Scalar       */
/*********************/

/* Unary */
Bool vc_isScalarUnaryOpF32(const IROp op);

/* Binary */
Bool vc_isScalarBinaryOpF32(const IROp op);

/* Comparisons */
Bool vc_isCmpOpF32(const IROp op);

/* Ternary */
Bool vc_isTernaryOpF32(const IROp op);

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF32(const IROp op);

/*********************/
/* Lowest-Lane-Only  */
/*********************/

/* Binary */
Bool vc_isLLOx4BinaryOpF32(const IROp op);

/* Comparisons */
Bool vc_isLLOx4CmpOpF32(const IROp op);

/* Intrinsics */
Bool vc_isLLOx4IntrinsicOpF32(const IROp op);

/*********************/
/*      Vector       */
/*********************/

/********/
/*  x2  */
/********/

/* Unary */
Bool vc_isVectorx2UnaryOpF32(const IROp op);

/* Binary */
Bool vc_isVectorx2BinaryOpF32(const IROp op);

/* Comparisons */
Bool vc_isVectorx2CmpOpF32(const IROp op);

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF32(const IROp op);

/********/
/*  x4  */
/********/

/* Unary */
Bool vc_isVectorx4UnaryOpF32(const IROp op);

/* Binary */
Bool vc_isVectorx4BinaryOpF32(const IROp op);

/* Comparisons */
Bool vc_isVectorx4CmpOpF32(const IROp op);

/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF32(const IROp op);

/********/
/*  x8  */
/********/

/* Unary*/

/* Binary */
Bool vc_isVectorx8BinaryOpF32(const IROp op);

/* Intrinsics */
Bool vc_isVectorx8IntrinsicOpF32(const IROp op);

/******************************************/
/*                Binary64                */
/******************************************/

/*********************/
/*      Scalar       */
/*********************/

/* Unary */
Bool vc_isScalarUnaryOpF64(const IROp op);

/* Binary */
Bool vc_isScalarBinaryOpF64(const IROp op);

/* Comparisons */
Bool vc_isCmpOpF64(const IROp op);

/* Ternary */ 
Bool vc_isTernaryOpF64(const IROp op);

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF64(const IROp op);

/*********************/
/* Lowest-Lane-Only  */
/*********************/

/* Binary */
Bool vc_isLLOx2BinaryOpF64(const IROp op);

/* Comparisons */
Bool vc_isLLOx2CompOpF64(const IROp op);

/* Intrinsics */
Bool vc_isLLOx2IntrinsicOpF64(const IROp op);

/*********************/
/*      Vector       */
/*********************/

/********/
/*  x2  */
/********/

/* Unary */
Bool vc_isVectorx2UnaryOpF64(const IROp op);

/* Binary */
Bool vc_isVectorx2BinaryOpF64(const IROp op);

/* Comparisons */
Bool vc_isVectorx2CompOpF64(const IROp op);

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF64(const IROp op);

/********/
/*  x4  */
/********/

/* Unary */

/* Binary */
Bool vc_isVectorx4BinaryOpF64(const IROp op);

/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF64(const IROp op);

/* Utils functions */

Bool vc_isScalarArithmeticOpF32(const IROp op);
Bool vc_isVectorx2ArithmeticOpF32(const IROp op);
Bool vc_isVectorx4ArithmeticOpF32(const IROp op);
Bool vc_isVectorx8ArithmeticOpF32(const IROp op);
Bool vc_isLLOArithmeticOpF32(const IROp op);
Bool vc_isVectorArithmeticOpF32(const IROp op);
Bool vc_isArithmeticOpF32(const IROp op);

Bool vc_isScalarArithmeticOpF64(const IROp op);
Bool vc_isVectorx2ArithmeticOpF64(const IROp op);
Bool vc_isVectorx4ArithmeticOpF64(const IROp op);
Bool vc_isLLOArithmeticOpF64(const IROp op);
Bool vc_isVectorArithmeticOpF64(const IROp op);
Bool vc_isArithmeticOpF64(const IROp op);
Bool vc_isArithmeticOpF(const IROp op);

Bool vc_isAddOp(const IROp op);
Bool vc_isSubOp(const IROp op);
Bool vc_isMulOp(const IROp op);
Bool vc_isDivOp(const IROp op);
Bool vc_isFmaOp(const IROp op);

/* Returns the size of the operands */
/* 1 for Scalar or LLO */
/* N for VectorxN */
ULong vc_getSizeArithmeticOp(const IROp op);

const HChar *vc_getTypeNameOp(const IROp op);
const HChar *vc_getDimTypeNameOp(const IROp op);
const HChar *vc_getArithmeticNameOp(const IROp op);
void vc_getNameOp(const IROp op, HChar *name);
const HChar *vc_getRawNameOp(const IROp op);

#endif /* __VF_FOPS_H__ */
