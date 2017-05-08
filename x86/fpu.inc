{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the FPU
[-----------------------------------------------------------------------------}

// ===========================================================================
// FLD - Load Float To FPU Stack (And Convert To 80bit Float)
function _fld(src: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$D9, $C0+Ord(src)]);
end;

function _fld(src: TMemVar): TBytes; overload;
begin
  case src.Size of
    f32: Result := src.FPUEncode([$D9]);
    f64: Result := src.FPUEncode([$DD]);
  end;
end;


// ===========================================================================
// FILD - Load Integer To FPU Stack (And Convert To 80bit Float)
function _fild(src: TMemVar): TBytes; overload;
begin
  case src.Size of
    i16: Result := src.FPUEncode([$DF]);
    i32: Result := src.FPUEncode([$DB]);
    i64: Result := src.FPUEncode([$DF], $28);
  end;
end;



// ===========================================================================
// FST - Store Float To Memory
function _fst(dst: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$DD, $D0+Ord(dst)]);
end;

function _fst(dst: TMemVar): TBytes; overload;
begin
  case dst.size of
    f32: Result := dst.FPUEncode([$D9], $10);
    f64: Result := dst.FPUEncode([$DD], $10);
  end;
end;



// ===========================================================================
// FIST - Store Integer To Memory
function _fist(dst: TMemVar): TBytes;
begin
  case dst.size of
    i16: Result := dst.FPUEncode([$DF], $10, True);
    i32: Result := dst.FPUEncode([$DB], $10);
  end;
end;



// ===========================================================================
// FSTP - Store Float To Memory And Pop
function _fstp(dst: TFPURegister): TBytes; overload; 
begin
  Result := ToBytes([$DD, $D8+Ord(dst)]);
end;

function _fstp(dst: TMemVar): TBytes; overload;
begin
  case dst.size of 
    f32: Result := dst.FPUEncode([$D9], $18);
    f64: Result := dst.FPUEncode([$DD], $18);
  end;
end;


// ===========================================================================
// FISTP - Store Integer To Memory And Pop
function _fistp(dst: TMemVar): TBytes;
begin
  case dst.Size of
    i16: Result := dst.FPUEncode([$DF], $18, True);
    i32: Result := dst.FPUEncode([$DB], $18);
    i64: Result := dst.FPUEncode([$DF], $38);
  end;
end;


// ===========================================================================
// FISTTP - Store Integer To Memory (Truncated) And Pop
function _fisttp(dst: TMemVar): TBytes;
begin
  case dst.Size of
    i16: Result := dst.FPUEncode([$DF], $0D, True);
    i32: Result := dst.FPUEncode([$DB], $0D);
    i64: Result := dst.FPUEncode([$DD], $0D);
  end;
end;


// ===========================================================================
// FFREE - Free the register operand (Marks as Unused)
function _ffree(reg: TFPURegister): TBytes; 
begin
  Result := ToBytes([$DD, $C0+Ord(reg)]);
end;




// ===========================================================================
// CONSTANTS FOR::: ADD, SUB, SUBR, MUL, DIV, DIVR
const     
  {$IFDEF LAPE}   
             // reg-reg | pop |   mem  | int-mem 
  fpu_opcode = [$DC,$D8,  $DE,  $DC,$D8, $DE,$DA];
  fpu_memidx = 2;
  
  ADD_OFFSET  = [$C0, $C0, {mem} $00];
  SUB_OFFSET  = [$E8, $E0, {mem} $20];
  SUBR_OFFSET = [$E0, $E8, {mem} $28];
  MUL_OFFSET  = [$C8, $C8, {mem} $08];
  DIV_OFFSET  = [$F8, $F0, {mem} $30];
  DIVR_OFFSET = [$F0, $F8, {mem} $38];
  {$ELSE}
                                   // reg-reg | pop |   mem  | int-mem 
  fpu_opcode: array [0..6] of Byte = ($DC,$D8,  $DE,  $DC,$D8, $DE,$DA);
  fpu_memidx = 2;
  
  ADD_OFFSET:  array [0..2] of Byte = ($C0, $C0, {mem} $00);
  SUB_OFFSET:  array [0..2] of Byte = ($E8, $E0, {mem} $20);
  SUBR_OFFSET: array [0..2] of Byte = ($E0, $E8, {mem} $28);
  MUL_OFFSET:  array [0..2] of Byte = ($C8, $C8, {mem} $08);
  DIV_OFFSET:  array [0..2] of Byte = ($F8, $F0, {mem} $30);
  DIVR_OFFSET: array [0..2] of Byte = ($F0, $F8, {mem} $38);
  {$ENDIF}
  
  

// ===========================================================================
// FADD - Floating Point Addition
function _faddp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], ADD_OFFSET[0]+ord(dst)]);
end;

function _fadd(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], ADD_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], ADD_OFFSET[1]+ord(src)])
end;

function _fadd(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], ADD_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], ADD_OFFSET[fpu_memidx]);
  end;
end;

function _fiadd(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], ADD_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], ADD_OFFSET[fpu_memidx]);
  end;
end;



// ===========================================================================
// FSUB - Floating Point Subtraction
function _fsubp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], SUB_OFFSET[0]+ord(dst)]);
end;

function _fsub(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], SUB_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], SUB_OFFSET[1]+ord(src)])
end;

function _fsub(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], SUB_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], SUB_OFFSET[fpu_memidx]);
  end;
end;

function _fisub(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], SUB_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], SUB_OFFSET[fpu_memidx]);
  end;
end;




// ===========================================================================
// FSUBR - Reverse Floating Point Subtraction
function _fsubrp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], SUBR_OFFSET[0]+ord(dst)]);
end;

function _fsubr(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], SUBR_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], SUBR_OFFSET[1]+ord(src)])
end;

function _fsubr(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], SUBR_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], SUBR_OFFSET[fpu_memidx]);
  end;
end;

function _fisubr(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], SUBR_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], SUBR_OFFSET[fpu_memidx]);
  end;
end;




// ===========================================================================
// FMUL - Floating Point Multiplication
function _fmulp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], MUL_OFFSET[0]+ord(dst)]);
end;

function _fmul(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], MUL_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], MUL_OFFSET[1]+ord(src)])
end;

function _fmul(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], MUL_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], MUL_OFFSET[fpu_memidx]);
  end;
end;

function _fimul(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], MUL_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], MUL_OFFSET[fpu_memidx]);
  end;
end;




// ===========================================================================
// FDIV - Floating Point Division
function _fdivp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], DIV_OFFSET[0]+ord(dst)]);
end;

function _fdiv(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], DIV_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], DIV_OFFSET[1]+ord(src)])
end;

function _fdiv(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], DIV_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], DIV_OFFSET[fpu_memidx]);
  end;
end;

function _fidiv(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], DIV_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], DIV_OFFSET[fpu_memidx]);
  end;
end;




// ===========================================================================
// FDIVR - Reverse Floating Point Division
function _fdivrp(dst: TFPURegister=st1): TBytes; overload; 
begin
  Result := ToBytes([fpu_opcode[2], DIVR_OFFSET[0]+ord(dst)]);
end;

function _fdivr(src, dst: TFPURegister): TBytes; overload; 
begin
  Assert((Ord(src) = 0) or (Ord(dst) = 0), 'Invalid Operand (Src or Dst must be st0)'); 
  if      (Ord(src) = 0) then Result := ToBytes([fpu_opcode[0], DIVR_OFFSET[0]+ord(dst)])
  else if (Ord(dst) = 0) then Result := ToBytes([fpu_opcode[1], DIVR_OFFSET[1]+ord(src)])
end;

function _fdivr(mem: TMemVar): TBytes; overload;
begin
  case mem.Size of 
    f32: Result := mem.FPUEncode([fpu_opcode[3]], DIVR_OFFSET[fpu_memidx]);
    f64: Result := mem.FPUEncode([fpu_opcode[4]], DIVR_OFFSET[fpu_memidx]);
  end;
end;

function _fidivr(mem: TMemVar): TBytes; 
begin
  case mem.Size of 
    i16: Result := mem.FPUEncode([fpu_opcode[5]], DIVR_OFFSET[fpu_memidx]);
    i32: Result := mem.FPUEncode([fpu_opcode[6]], DIVR_OFFSET[fpu_memidx]);
  end;
end;





{=============================================================================}
// Functions bellow
{=============================================================================}

// ===========================================================================
// FTst - Test Floating Point Value
function _ftst(): TBytes; 
begin
  Result := ToBytes([$D9, $E4]);
end;

// ===========================================================================
// FXam - Examine Floating Point Value
function _fxam(): TBytes; 
begin
  Result := ToBytes([$D9, $E5]);
end;

// ===========================================================================
// FAbs - Absolute Value - Clear the sign of st0
function _fabs(): TBytes;
begin
  Result := ToBytes([$D9, $E1]);
end;

// ===========================================================================
// FRndInt - Round To Integer
function _frndint(): TBytes;
begin
  Result := ToBytes([$D9, $FC]);
end;

// ===========================================================================
// FSqrt - Square Root
function _fsqrt(): TBytes;
begin
  Result := ToBytes([$D9, $FA]);
end;

// ===========================================================================
// FCos - Cosine
function _fcos(): TBytes;
begin
  Result := ToBytes([$D9, $FF]);
end;

// ===========================================================================
// FSin - Sine
function _fsin(): TBytes;
begin
  Result := ToBytes([$D9, $FE]);
end;

// ===========================================================================
// FSinCos - Sine and Consine
function _fsincos(): TBytes;
begin
  Result := ToBytes([$D9, $FB]);
end;

// ===========================================================================
// FPTan - Partial Tangent
function _fptan(): TBytes;
begin
  Result := ToBytes([$D9, $F2]);
end;

// ===========================================================================
// FPATan - Partial Arctangent
function _fpatan(): TBytes;
begin
  Result := ToBytes([$D9, $F3]);
end;

// ===========================================================================
// FPRem - Partial Remainder (Round to Zero)
function _fprem(): TBytes;
begin
  Result := ToBytes([$D9, $F8]);
end;

// ===========================================================================
// FPRem1 - Partial Remainder (Round to Nearest)
function _fprem1(): TBytes;
begin
  Result := ToBytes([$D9, $F5]);
end;

// ===========================================================================
// FScale - Scale
function _fscale(): TBytes;
begin
  Result := ToBytes([$D9, $FD]);
end;

// ===========================================================================
// FXtract - Extract Exponent and Mantissa
function _fxtract(): TBytes;
begin
  Result := ToBytes([$D9, $F4]);
end;

// ===========================================================================
// FYL2X - Compute "st1 * log_2(st0)" - Store in st1
function _fyl2x(): TBytes;
begin
  Result := ToBytes([$D9, $F1]);
end;

// ===========================================================================
// FYL2XP1 - Compute "st1 * log_2(st0 + 1.0)" - Store in st1
function _fyl2xp1(): TBytes;
begin
  Result := ToBytes([$D9, $F9]);
end;




