{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the general purpose registers
[-----------------------------------------------------------------------------}

// ===========================================================================
// PUSH - Push Word or Doubleword Onto the Stack
function TSlackASM._push(x: TGPRegister): TBytes; overload; 
begin
  Assert(x.Size > 1, 'Illegal operand size');
  if x.size = 2 then Result := ToBytes([$66, $50+x.gpReg])
  else               Result := ToBytes([$50+x.gpReg]);
end;

function TSlackASM._push(x: TMemVar): TBytes; overload; 
begin
  Assert(x.Size > 1, 'Illegal operand size BYTE');
  case x.MemType of
    mtRegMem : Result := x.Encode([$FE], EAX.Convert(x.size), $30);
    mtPointer: Result := x.Encode([$FE], EAX.Convert(x.size), $35);
    mtStack  : Result := x.Encode([$FE], EAX.Convert(x.size), $05);
  end;
end;

function TSlackASM._push(x: TImmediate): TBytes; overload;
begin
  Assert(x.Size = 4, 'Illegal operand size BYTE');
  Result := [68] + x.slice(4);
end;



// ===========================================================================
// Pop - Pop a Value from the Stack
function TSlackASM._pop(x: TGPRegister): TBytes; overload;
begin
  Assert(x.Size > 1, 'Illegal operand size BYTE');
  if x.size = 2 then Result := ToBytes([$66, $58+x.gpReg])
  else               Result := ToBytes([$58+x.gpReg]);
end;

function TSlackASM._pop(x: TMemVar): TBytes; overload;
begin
  Assert(x.Size > 1, 'Illegal operand size BYTE');
  case x.MemType of
    mtRegMem : Result := x.Encode([$8F], EAX.Convert(x.size), $30);
    mtPointer: Result := x.Encode([$8F], EAX.Convert(x.size), $35);
    mtStack  : Result := x.Encode([$8F], EAX.Convert(x.size), $05);
  end;
end;




// ===========================================================================
// MOV - Move (Copy) Left Operand into Right Operand
function TSlackASM._mov(x,y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$88], y);
end;

function TSlackASM._mov(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$8A], y);
end;

function TSlackASM._mov(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := x.EncodeMem([$88], y);
end;

function TSlackASM._mov(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.EncodeHC([$B0],[$B8],[$B8], y);
end;




// ===========================================================================
// MOVZX - Move (Copy) Left Operand into Right Operand With Zero-Extend
function TSlackASM._movzx(x,y: TGPRegister): TBytes; overload; 
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := y.Encode([$0F,$B6], x, -1, 1);
end;

function TSlackASM._movzx(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := x.Encode([$0F,$B6], y, -1, 1);
end;



// ===========================================================================
// MOVSX - Move (Copy) Left Operand into Right Operand With Sign-Extend
function TSlackASM._movsx(x,y: TGPRegister): TBytes; overload; 
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((y.Size > x.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := y.Encode([$0F,$BE], x, -1, 1);
end;

function TSlackASM._movsx(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := x.Encode([$0F,$BE], y, -1, 1);
end;



// ===========================================================================
// LEA - Load Effective Address
function TSlackASM._lea(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$8D], y);
end;



// ===========================================================================
// INC - Increment Operand
function TSlackASM._inc(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x);
end;

function TSlackASM._inc(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$FE], EAX.Convert(x.size));
end;



// ===========================================================================
// DEC - Decrement Operand
function TSlackASM._dec(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x, $C8);
end;

function TSlackASM._dec(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$FE], EAX.Convert(x.size), Ord(x.MemType)+$08);
end;



// ===========================================================================
// CDQ - Convert Doubleword to Quadword (EDX:EAX = sign-extend of EAX)
function TSlackASM._cdq(): TBytes;
begin
  Result := [$99];
end;

function TSlackASM._cltq(): TBytes;
begin
  Result := self._cdq();
end;



// ===========================================================================
// Test - Logical Compare
function TSlackASM._test(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$84], y);
end;

function TSlackASM._test(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := x.EncodeMem([$84], y);
end;

function TSlackASM._test(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$86], y);
end;

function TSlackASM._test(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$F7, $C0], y, 1);
end;



// ===========================================================================
// CMP - Compare
function TSlackASM._cmp(x,y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$38], y);
end;

function TSlackASM._cmp(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := x.EncodeMem([$38], y);
end;

function TSlackASM._cmp(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$3A], y);
end;

function TSlackASM._cmp(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $F8], y, 1);
end;



// ===========================================================================
// AND - Logical And
function TSlackASM._and(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$20], y);
end;

function TSlackASM._and(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := x.EncodeMem([$20], y);
end;

function TSlackASM._and(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$22], y);
end;

function TSlackASM._and(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $E0], y, 1);
end;


// ===========================================================================
// OR - Logical Inclusive Or
function TSlackASM._or(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$08], y);
end;

function TSlackASM._or(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := x.EncodeMem([$08], y);
end;

function TSlackASM._or(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$0A], y);
end;

function TSlackASM._or(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $C8], y, 1);
end;


// ===========================================================================
// XOR - Logical Exclusive OR
function TSlackASM._xor(x, y: TGPRegister): TBytes; overload;
begin 
  Result := x.Encode([$30], y);
end;

function TSlackASM._xor(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := x.EncodeMem([$30], y);
end;

function TSlackASM._xor(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$32], y);
end;

function TSlackASM._xor(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $F0], y, 1);
end;



// ===========================================================================
// ADD - Addition
function TSlackASM._add(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$00], y);
end;

function TSlackASM._add(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := x.EncodeMem([$00], y);
end;

function TSlackASM._add(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$02], y);
end;

function TSlackASM._add(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $C0], y, 1);
end;


// ===========================================================================
// ADC - Arithmetic Addition with Carry
function TSlackASM._adc(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$10], y);
end;

function TSlackASM._adc(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := x.EncodeMem([$10], y);
end;

function TSlackASM._adc(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$12], y);
end;

function TSlackASM._adc(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $D0], y, 1);
end;


// ===========================================================================
// SUB - Subtract
function TSlackASM._sub(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$28], y);
end;

function TSlackASM._sub(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := x.EncodeMem([$28], y);
end;

function TSlackASM._sub(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$2A], y);
end;

function TSlackASM._sub(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $E8], y, 1);
end;


// ===========================================================================
// SUB - Subtract With Borrow
function TSlackASM._sbb(x, y: TGPRegister): TBytes; overload;
begin 
  Result := x.Encode([$18], y);
end;

function TSlackASM._sbb(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := x.EncodeMem([$18], y);
end;

function TSlackASM._sbb(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$1A], y);
end;

function TSlackASM._sbb(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80, $D8], y, 1);
end;


// ===========================================================================
// DIV - Unsigned division
function TSlackASM._div(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F0);
end;

function TSlackASM._div(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$30);
end;



// ===========================================================================
// IDIV - Signed division
function TSlackASM._idiv(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F8);
end;

function TSlackASM._idiv(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$38);
end;



// ===========================================================================
// MUL - Unsigned multiplication
function TSlackASM._mul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E0);
end;

function TSlackASM._mul(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// IMUL - Signed Multiplication
function TSlackASM._imul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E8);
end;

function TSlackASM._imul(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$28);
end;

function TSlackASM._imul(x,y: TGPRegister): TBytes; overload;
begin
  Assert(y.Size > 1, 'Illegal operand size');
  Result := y.Encode([$0E,$AF], x, -1, 0);
end;

function TSlackASM._imul(x: TMemVar; y: TGPRegister): TBytes; overload;
begin 
  Assert(y.Size > 1, 'Illegal operand size');
  Result := x.Encode([$0E,$AF], y, -1, 0);
end;

function TSlackASM._imul(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Assert(y.size <> 1, 'Illegal operand size BYTE');
  Result := x.EncodeEx([$68], y,y, 0, $C0);
end;


// ===========================================================================
// SAL - Shift Arithmetic Left

//SAL %cl, %reg
function TSlackASM._sal(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

//SAL %cl, %mem
function TSlackASM._sal(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// SAR - Shift Arithmetic Right

// -- SAR %reg --
function TSlackASM._sar(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $F8);
end;

// -- SAR mem --
function TSlackASM._sar(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$38);
end;



// ===========================================================================
// SHL - Shift Left

// -- SHL %reg --
function TSlackASM._shl(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

// -- SHL mem --
function TSlackASM._shl(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// SHR - Shift Right

// -- SHL %reg --
function TSlackASM._shr(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E8);
end;

// -- SHL mem --
function TSlackASM._shr(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$28);
end;



// ===========================================================================
// ROL - Rotate Left

// -- ROL %reg --
function TSlackASM._rol(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C0);
end;

// -- ROL mem --
function TSlackASM._rol(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$00);
end;


// ===========================================================================
// ROR - Rotate Right

// -- ROR %reg --
function TSlackASM._ror(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C8);
end;

// -- ROR mem --
function TSlackASM._ror(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$08);
end;




// ===========================================================================
// SETcc - Set Byte on Condition

// -- SETc %reg --
function TSlackASM._setc(opcode:E_SETxx; dst: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$0F, Ord(opcode)], dst.Convert(szBYTE), -1, 1);
end;

// -- SETc mem --
function TSlackASM._setc(opcode:E_SETxx; dst: TMemVar): TBytes; overload;
begin
  Result := _AL.EncodeMem([$0F, Ord(opcode)], dst, -1, 1);
end;



