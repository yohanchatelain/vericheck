# Vericheck tool

Simple tool that counts the number of FP instructions exectuded
during the execution. Two counters for IEEE and Interflop operations.

## Exclusion

Vericheck excludes all FP operations of libc, libdl and dl by default.

## IEEE counter

For each function that does not belongs to interflop,
Vericheck counts the number of FP instructions executed.
Only arithmetic operations are counted, this includes {+,-,*,/}.

## Interflop counter

This counts the number of interflop instructions called during the execution.
One interflop operation corresponds to its elementary counterpart
in IEEE. Hence, _interflop_float_add(a, b) counts for 1 instruction,
no matter how many IEEE FP instructions are really executed.

## Output

At the end of the execution, Vericheck prints the number of FP instructions executed for each function.
Each line is formatted as :
* <lib> -> <dir>/<file>:<function> : <fp>
where
- lib:      the object name (library or binary) of the function
- dir:      the path to file that contains the function
- file:     the file that contains the function
- function: the name of the function
- fp:       the number of fp instructions executed

Exemple on `tests/test_kahan` from [Verificarlo](https://github.com/verificarlo/verificarlo.git) repository.

```bash
==22673== 
==22673== 1 IEEE functions visited
==22673== -------------------------
==22673== 	* /verificarlo/tests/test_kahan/test -> ???/???:fill_array : 100001
==22673== -------------------------
==22673== IEEE FP: 100001
==22673== 
==22673== 2 Interflop functions visited
==22673== -------------------------
==22673== 	* /home/yohan/local/lib/libinterflop_ieee.so.0.0.0 -> /verificarlo/src/backends/interflop-ieee/interflop_ieee.c:_interflop_add_float : 100000
==22673== 	* /home/yohan/local/lib/libinterflop_ieee.so.0.0.0 -> /verificarlo/src/backends/interflop-ieee/interflop_ieee.c:_interflop_sub_float : 300000
==22673== -------------------------
==22673== Interflop FP: 400000
==22673== 
==22673== IEEE FP ratio: 20%
==22673== Interflop FP ratio: 79%
```