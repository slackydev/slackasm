{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the FPU
[-----------------------------------------------------------------------------}

// ===========================================================================
// FILD - Load Integer To FPU Stack (And Convert To 80bit Float)

// FILDW m16
function TSlackASM._fildw(src: PInt16): TBytes; overload; 
begin
  Result := ToBytes([$DF, $05]) + addr_to_bytes(src);
end;

// FILDL m32
function TSlackASM._fildl(src: PInt32): TBytes; overload; 
begin
  Result := ToBytes([$DB, $05]) + addr_to_bytes(src);
end;

// FILDLL/FILDQ m64
function TSlackASM._fildq(src: PInt64): TBytes; overload; 
begin
  Result := ToBytes([$DF, $05]) + addr_to_bytes(src);
end;


// ===========================================================================
// FLD - Load Float To FPU Stack (And Convert To 80bit Float)

// FLD freg
function TSlackASM._fld(src: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$D9, $C0+Ord(src)]);
end;

// FLDS m64fp
function TSlackASM._flds(src: PSingle): TBytes; overload; 
begin
  Result := ToBytes([$D9, $05]) + addr_to_bytes(src);
end;

// FLDL m64fp
function TSlackASM._fldl(src: PDouble): TBytes; overload; 
begin
  Result := ToBytes([$DD, $05]) + addr_to_bytes(src);
end;


// ===========================================================================
// FISTP - Store Integer To Memory And Pop

// FISTPW m16
function TSlackASM._fistpw(dst: PInt16): TBytes; overload; 
begin
  Result := ToBytes([$66, $DF, $1D]) + addr_to_bytes(dst);
end;

// FISTPL m32
function TSlackASM._fistpl(dst: PInt32): TBytes; overload; 
begin
  Result := ToBytes([$DB, $1D]) + addr_to_bytes(dst);
end;

// FISTPQ m64
function TSlackASM._fistpq(dst: PInt64): TBytes; overload; 
begin
  Result := ToBytes([$DD, $3D]) + addr_to_bytes(dst);
end;


// ===========================================================================
// FSTP - Store Float To Memory And Pop

// FSTP %freg
function TSlackASM._fstp(dst: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$DD, $D8+Ord(dst)]);
end;

// FSTPS m32fp
function TSlackASM._fstps(dst: PSingle): TBytes; overload; 
begin
  Result := ToBytes([$D9, $1D]) + addr_to_bytes(dst);
end;

// FSTPL m64fp
function TSlackASM._fstpl(dst: PDouble): TBytes; overload; 
begin
  Result := ToBytes([$DD, $1D]) + addr_to_bytes(dst);
end;



// ===========================================================================
// FIST - Store Integer To Memory And Pop

// FISTW m16fp
function TSlackASM._fistw(dst: PInt16): TBytes; overload; 
begin
  Result := ToBytes([$66, $DF, $15]) + addr_to_bytes(dst);
end;

// FISTL m32fp
function TSlackASM._fistl(dst: PInt32): TBytes; overload; 
begin
  Result := ToBytes([$DB, $15]) + addr_to_bytes(dst);
end;


// ===========================================================================
// FST - Store Float To Memory

// FST %freg
function TSlackASM._fst(dst: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$DD, $D0+Ord(dst)]);
end;

// FSTS m32fp
function TSlackASM._fsts(dst: PSingle): TBytes; overload; 
begin
  Result := ToBytes([$D9, $15]) + addr_to_bytes(dst);
end;

// FSTL m64fp
function TSlackASM._fstl(dst: PDouble): TBytes; overload; 
begin
  Result := ToBytes([$DD, $15]) + addr_to_bytes(dst);
end;


// ===========================================================================
// CONSTANTS FOR::: ADD, SUB, SUBR, MUL
const
  _ARITH_OPC  = [$DC, $D8, $D8, $DC, $DE, $DE, $DE, $DA];

  _ADD_CONST  = [$C0, $C0, $05, $05, $C1, $C0, $05, $05];
  _SUB_CONST  = [$E8, $E0, $25, $25, $E9, $E8, $25, $25];
  _SUBR_CONST = [$E0, $E8, $2D, $2D, $E1, $E0, $2D, $2D];
  _MUL_CONST  = [$C8, $C8, $0D, $0D, $C9, $C8, $0D, $0D];
  _DIV_CONST  = [$F8, $F0, $35, $35, $F9, $F8, $35, $35];
  _DIVR_CONST = [$F0, $F8, $3D, $3D, $F1, $F0, $3D, $3D];
  


// ===========================================================================
// FADD - Floating Point Addition

// FADD %reg, %reg
function TSlackASM._fadd(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _ADD_CONST[0]+ord(dst)]))
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _ADD_CONST[1]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// FADD m32fp
function TSlackASM._fadds(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _ADD_CONST[2]]) + addr_to_bytes(mem);
end;

// FADD m64fp
function TSlackASM._faddl(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _ADD_CONST[3]]) + addr_to_bytes(mem);
end;

// ===========================================================================
// FADDP - Floating Point Addition with Pop
function TSlackASM._faddp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _ADD_CONST[4]]);
end;

function TSlackASM._faddp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _ADD_CONST[5]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FIADD - Integer Addition

// FIADD m16i
function TSlackASM._fiaddw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _ADD_CONST[6]]) + addr_to_bytes(mem);
end;

// FIADD m32i
function TSlackASM._fiaddl(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _ADD_CONST[7]]) + addr_to_bytes(mem);
end;



// ===========================================================================
// FSUB - Floating Point Subtraction

// FSUB %reg, %reg
function TSlackASM._fsub(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _SUB_CONST[0]+ord(dst)]))
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _SUB_CONST[1]+ord(dst)])) 
  else
    WriteLn('Invalid operand');
end;

// FSUB m32fp
function TSlackASM._fsubs(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _SUB_CONST[2]]) + addr_to_bytes(mem);
end;

// FSUB m64fp
function TSlackASM._fsubl(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _SUB_CONST[3]]) + addr_to_bytes(mem);
end;


// ===========================================================================
// FSUBP - Floating Point Subtraction with Pop
function TSlackASM._fsubp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _SUB_CONST[4]]);
end;

function TSlackASM._fsubp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _SUB_CONST[5]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FISUB - Integer Subtraction

// FISUB m16i
function TSlackASM._fisubw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _SUB_CONST[6]]) + addr_to_bytes(mem);
end;

// FISUB m32i
function TSlackASM._fisubl(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _SUB_CONST[7]]) + addr_to_bytes(mem);
end;




// ===========================================================================
// FSUBR - Reverse Floating Point Subtraction

// FSUBR %reg, %reg
function TSlackASM._fsubr(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _SUBR_CONST[0]+ord(dst)])) //uh? order incorrect?
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _SUBR_CONST[1]+ord(dst)])) 
  else
    WriteLn('Invalid operand');
end;

// FSUBR m32fp
function TSlackASM._fsubrs(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _SUBR_CONST[2]]) + addr_to_bytes(mem);
end;

// FSUBR m64fp
function TSlackASM._fsubrl(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _SUBR_CONST[3]]) + addr_to_bytes(mem);
end;

// ===========================================================================
// FSUBRP - Reverse Floating Point Addition with Pop
function TSlackASM._fsubrp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _SUBR_CONST[4]]);
end;

function TSlackASM._fsubrp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _SUBR_CONST[5]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FISUBR - Reverse Integer Subtraction

// FISUBR m16i
function TSlackASM._fisubrw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _SUBR_CONST[6]]) + addr_to_bytes(mem);
end;

// FISUBR m32i
function TSlackASM._fisubrl(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _SUBR_CONST[7]]) + addr_to_bytes(mem);
end;




// ===========================================================================
// FMUL - Floating Point Multiplication

// FMUL %reg, %reg
function TSlackASM._fmul(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _MUL_CONST[0]+ord(dst)]))
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _MUL_CONST[1]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// FMUL m32fp
function TSlackASM._fmuls(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _MUL_CONST[2]]) + addr_to_bytes(mem);
end;

// FMUL m64fp
function TSlackASM._fmull(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _MUL_CONST[3]]) + addr_to_bytes(mem);
end;

// ===========================================================================
// FMULP - Floating Point Multiplication with Pop
function TSlackASM._fmulp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _MUL_CONST[4]]);
end;

function TSlackASM._fmulp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _MUL_CONST[5]+ord(dst)])) //uh? order incorrect?
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FIMUL - Integer Multiplication

// FIMUL m16i
function TSlackASM._fimulw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _MUL_CONST[6]]) + addr_to_bytes(mem);
end;

// FIMUL m32i
function TSlackASM._fimull(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _MUL_CONST[7]]) + addr_to_bytes(mem);
end;




// ===========================================================================
// FDIV - Floating Point Division

// FDIV %reg, %reg
function TSlackASM._fdiv(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _DIV_CONST[0]+ord(dst)]))
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _DIV_CONST[1]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// FDIV m32fp
function TSlackASM._fdivs(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _DIV_CONST[2]]) + addr_to_bytes(mem);
end;

// FDIV m64fp
function TSlackASM._fdivl(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _DIV_CONST[3]]) + addr_to_bytes(mem);
end;

// ===========================================================================
// FDIVP - Floating Point Division with Pop
function TSlackASM._fdivp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _DIV_CONST[4]]);
end;

function TSlackASM._fdivp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _DIV_CONST[5]+ord(dst)])) //uh? order incorrect?
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FIDIV - Integer Division

// FIDIV m16i
function TSlackASM._fidivw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _DIV_CONST[6]]) + addr_to_bytes(mem);
end;

// FIDIV m32i
function TSlackASM._fidivl(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _DIV_CONST[7]]) + addr_to_bytes(mem);
end;




// ===========================================================================
// FDIVR - Reverse Floating Point Division

// FDIVR %reg, %reg
function TSlackASM._fdivr(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[0], _DIVR_CONST[0]+ord(dst)]))
  else if (Ord(dst) = 0) then
    Exit(ToBytes([_ARITH_OPC[1], _DIVR_CONST[1]+ord(dst)]))
  else
    WriteLn('Invalid operand');
end;

// FDIVR m32fp
function TSlackASM._fdivrs(mem: PSingle): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[2], _DIVR_CONST[2]]) + addr_to_bytes(mem);
end;

// FDIVR m64fp
function TSlackASM._fdivrl(mem: PDouble): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[3], _DIVR_CONST[3]]) + addr_to_bytes(mem);
end;

// ===========================================================================
// FDIVRP - Reverse Floating Point Division with Pop
function TSlackASM._fdivrp(): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[4], _DIVR_CONST[4]]);
end;

function TSlackASM._fdivrp(src, dst: TFPURegister): TBytes; overload; 
begin
  if (Ord(src) = 0) then
    Exit(ToBytes([_ARITH_OPC[5], _DIVR_CONST[5]+ord(dst)])) //uh? order incorrect?
  else
    WriteLn('Invalid operand');
end;

// ===========================================================================
// FIDIVR - Reverse Integer Division

// FIDIVR m16i
function TSlackASM._fidivrw(mem: PInt16): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[6], _DIVR_CONST[6]]) + addr_to_bytes(mem);
end;

// FIDIVR m32i
function TSlackASM._fidivrl(mem: PInt32): TBytes; overload; 
begin
  Result := ToBytes([_ARITH_OPC[7], _DIVR_CONST[7]]) + addr_to_bytes(mem);
end;





// ===========================================================================
// FABS - Absolute Value - Clear the sign of st0
function TSlackASM._fabs(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $E1]);
end;

// ===========================================================================
// FRNDINT - Round To Integer
function TSlackASM._frndint(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $FC]);
end;

// ===========================================================================
// FTST - Test Floating Point Value
function TSlackASM._ftst(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $E4]);
end;

// ===========================================================================
// FSQRT - Square Root
function TSlackASM._fsqrt(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $FA]);
end;

// ===========================================================================
// FCOS - Cosine
function TSlackASM._fcos(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $FF]);
end;

// ===========================================================================
// FSIN - Sine
function TSlackASM._fsin(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $FE]);
end;

// ===========================================================================
// FSINCOS - Sine and Consine
function TSlackASM._fsincos(): TBytes; overload; 
begin
  Result := ToBytes([$D9, $FB]);
end;







