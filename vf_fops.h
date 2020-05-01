
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

/******************************************/
/*                Binary32                */
/******************************************/

/*********************/
/*      Scalar       */
/*********************/

/* Unary */
Bool vc_isScalarUnaryOpF32(IROp op);

/* Binary */
Bool vc_isScalarBinaryOpF32(IROp op);

/* Comparisons */
Bool vc_isCmpOpF32(IROp op);

/* Ternary */
Bool vc_isTernaryOpF32(IROp op);

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF32(IROp op);

/*********************/
/*      Vector       */
/*********************/

/********/
/*  x2  */
/********/

/* Unary */
Bool vc_isVectorx2UnaryOpF32(IROp op);

/* Binary */
Bool vc_isVectorx2BinaryOpF32(IROp op);

/* Comparisons */
Bool vc_isVectorx2CmpOpF32(IROp op);

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF32(IROp op);

/********/
/*  x4  */
/********/

/* Unary */
Bool vc_isVectorx4UnaryOpF32(IROp op);

/* Binary */
Bool vc_isVectorx4BinaryOpF32(IROp op);

/* Comparisons */
Bool vc_isVectorx4CmpOpF32(IROp op);

/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF32(IROp op);

/********/
/*  x8  */
/********/

/* Unary*/

/* Binary */
Bool vc_isVectorx8BinaryOpF32(IROp op);

/* Intrinsics */
Bool vc_isVectorx8IntrinsicOpF32(IROp op);

/******************************************/
/*                Binary64                */
/******************************************/

/*********************/
/*      Scalar       */
/*********************/

/* Unary */
Bool vc_isScalarUnaryOpF64(IROp op);

/* Binary */
Bool vc_isScalarF64BinaryOp(IROp op);

/* Comparisons */
Bool vc_isCmpOpF64(IROp op);

/* Ternary */ 
Bool vc_isTernaryOpF64(IROp op);

/* Intrinsics */
Bool vc_isScalarIntrinsicOpF64(IROp op);

/*********************/
/*      Vector       */
/*********************/

/********/
/*  x2  */
/********/

/* Unary */
Bool vc_isVectorx2UnaryOpF64(IROp op);

/* Binary */
Bool vc_isVectorx2BinaryOpF64(IROp op);

/* Comparisons */
Bool vc_isVectorx2CompOpF64(IROp op);

/* Intrinsics */
Bool vc_isVectorx2IntrinsicOpF64(IROp op);

/********/
/*  x4  */
/********/

/* Unary */

/* Binary */
Bool vc_isVectorx4BinaryOpF64(IROp op);

/* Intrinsics */
Bool vc_isVectorx4IntrinsicOpF64(IROp op);

/* Utils functions */

Bool vc_isArithmeticOpF32(IROp op);
Bool vc_isArithmeticOpF64(IROp op);

#endif /* __VF_FOPS_H__ */
