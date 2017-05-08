{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the general purpose registers
[-----------------------------------------------------------------------------}

// ===========================================================================
// PUSH - Push Word or Doubleword Onto the Stack
function _push(x: TGPRegister): TBytes; overload; 
begin
  Assert(x.Size > 1, 'Illegal operand size');
  if x.size = 2 then Result := ToBytes([$66, $50+x.gpReg])
  else               Result := ToBytes([$50+x.gpReg]);
end;

function _push(x: TMemVar): TBytes; overload; 
begin
  Assert(x.Size > 1, 'Illegal operand size BYTE');
  case x.MemType of
    mtRegMem : Result := x.Encode([$FE], EAX.Convert(x.size), $30);
    mtPointer: Result := x.Encode([$FE], EAX.Convert(x.size), $35);
    mtStack  : Result := x.Encode([$FE], EAX.Convert(x.size), $05);
  end;
end;

function _push(x: TImmediate): TBytes; overload;
begin
  Assert(x.Size = 4, 'Illegal operand size BYTE');
  Result := ToBytes([68]) + x.slice(4);
end;



// ===========================================================================
// Pop - Pop a Value from the Stack
function _pop(x: TGPRegister): TBytes; overload;
begin
  Assert(x.Size > 1, 'Illegal operand size BYTE');
  if x.size = 2 then Result := ToBytes([$66, $58+x.gpReg])
  else               Result := ToBytes([$58+x.gpReg]);
end;

function _pop(x: TMemVar): TBytes; overload;
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
function _mov(x,y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$88], y);
end;

function _mov(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$8A], y);
end;

function _mov(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := y.EncodeR([$88], x);
end;

function _mov(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.EncodeHC([$B0],[$B8],[$B8], y);
end;




// ===========================================================================
// MOVZX - Move (Copy) Left Operand into Right Operand With Zero-Extend
function _movzx(x,y: TGPRegister): TBytes; overload; 
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := y.Encode([$0F,$B6], x, -1, 1);
end;

function _movzx(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := x.Encode([$0F,$B6], y, -1, 1);
end;



// ===========================================================================
// MOVSX - Move (Copy) Left Operand into Right Operand With Sign-Extend
function _movsx(x,y: TGPRegister): TBytes; overload; 
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((y.Size > x.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := y.Encode([$0F,$BE], x, -1, 1);
end;

function _movsx(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  if (x.Size = y.Size) then Exit(_mov(x,y));
  Assert((x.Size < y.Size) and (y.Size >= szWORD), 'Illegal operand size');
  Result := x.Encode([$0F,$BE], y, -1, 1);
end;



// ===========================================================================
// LEA - Load Effective Address
function _lea(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$8C], y);
end;



// ===========================================================================
// INC - Increment Operand
function _inc(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x);
end;

function _inc(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$FE], EAX.Convert(x.size));
end;



// ===========================================================================
// DEC - Decrement Operand
function _dec(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x, $C8);
end;

function _dec(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$FE], EAX.Convert(x.size), Ord(x.MemType)+$08);
end;



// ===========================================================================
// CDQ - Convert Doubleword to Quadword (EDX:EAX = sign-extend of EAX)
function _cdq(): TBytes;
begin
  Result := ToBytes([$99]);
end;

function _cltq(): TBytes;
begin
  Result := _cdq();
end;


// ===========================================================================
// XCHG - Exchange Register/Memory with Register [incomplete]
function _xchg(x, y: TGPRegister): TBytes; overload;
begin
  Assert(x.Size = y.Size, 'Illegal operation');
  if (x.gpReg=gpEAX) and (x.Size = 4) then
    Result := ToBytes([$90+y.gpReg])
  else if (x.gpReg=gpEAX) and (x.Size = 2) then
    Result := ToBytes([$66, $90+y.gpReg])
  else
  begin
    Result := x.Encode([$86], y);
  end;
end;



// ===========================================================================
// Test - Logical Compare
function _test(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$84], y);
end;

function _test(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := y.EncodeR([$84], x);
end;

function _test(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$86], y);
end;

function _test(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$F7], y, $C0);
end;



// ===========================================================================
// CMP - Compare
function _cmp(x,y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$38], y);
end;

function _cmp(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := y.EncodeR([$38], x);
end;

function _cmp(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$3A], y);
end;

function _cmp(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $F8);
end;



// ===========================================================================
// AND - Logical And
function _and(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$20], y);
end;

function _and(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := y.EncodeR([$20], x);
end;

function _and(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$22], y);
end;

function _and(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $E0);
end;


// ===========================================================================
// OR - Logical Inclusive Or
function _or(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$08], y);
end;

function _or(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := y.EncodeR([$08], x);
end;

function _or(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$0A], y);
end;

function _or(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $C8);
end;


// ===========================================================================
// XOR - Logical Exclusive OR
function _xor(x, y: TGPRegister): TBytes; overload;
begin 
  Result := x.Encode([$30], y);
end;

function _xor(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := y.EncodeR([$30], x);
end;

function _xor(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$32], y);
end;

function _xor(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $F0);
end;



// ===========================================================================
// ADD - Addition
function _add(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$00], y);
end;

function _add(x: TGPRegister; y: TMemVar): TBytes; overload; 
begin
  Result := y.EncodeR([$00], x);
end;

function _add(x: TMemVar; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$02], y);
end;

function _add(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $C0);
end;


// ===========================================================================
// ADC - Arithmetic Addition with Carry
function _adc(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$10], y);
end;

function _adc(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := y.EncodeR([$10], x);
end;

function _adc(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$12], y);
end;

function _adc(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $D0);
end;


// ===========================================================================
// SUB - Subtract
function _sub(x, y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$28], y);
end;

function _sub(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := y.EncodeR([$28], x);
end;

function _sub(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$2A], y);
end;

function _sub(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $E8);
end;


// ===========================================================================
// SUB - Subtract With Borrow
function _sbb(x, y: TGPRegister): TBytes; overload;
begin 
  Result := x.Encode([$18], y);
end;

function _sbb(x: TGPRegister; y: TMemVar): TBytes; overload;
begin
  Result := y.EncodeR([$18], x);
end;

function _sbb(x: TMemVar; y: TGPRegister): TBytes; overload;
begin
  Result := x.Encode([$1A], y);
end;

function _sbb(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$80], y, $D8);
end;


// ===========================================================================
// DIV - Unsigned division
function _div(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F0);
end;

function _div(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$30);
end;



// ===========================================================================
// IDIV - Signed division
function _idiv(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F8);
end;

function _idiv(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$38);
end;



// ===========================================================================
// MUL - Unsigned multiplication
function _mul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E0);
end;

function _mul(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// IMUL - Signed Multiplication
function _imul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E8);
end;

function _imul(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$F6], EAX.Convert(x.size), Ord(x.MemType)+$28);
end;

function _imul(x,y: TGPRegister): TBytes; overload;
begin
  Assert(y.Size > 1, 'Illegal operand size');
  Result := y.Encode([$0E,$AF], x, -1, 0);
end;

function _imul(x: TMemVar; y: TGPRegister): TBytes; overload;
begin 
  Assert(y.Size > 1, 'Illegal operand size');
  Result := x.Encode([$0E,$AF], y, -1, 0);
end;

function _imul(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Assert(y.size <> 1, 'Illegal operand size BYTE');
  Result := x.EncodeEx([$68], y,y, $C0);
end;


// ===========================================================================
// SAL - Shift Arithmetic Left

//SAL %cl, %reg
function _sal(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

//SAL %cl, %mem
function _sal(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// SAR - Shift Arithmetic Right

// -- SAR %reg --
function _sar(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $F8);
end;

// -- SAR mem --
function _sar(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$38);
end;



// ===========================================================================
// SHL - Shift Left

// -- SHL %reg --
function _shl(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

// -- SHL mem --
function _shl(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$20);
end;



// ===========================================================================
// SHR - Shift Right

// -- SHL %reg --
function _shr(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E8);
end;

// -- SHL mem --
function _shr(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$28);
end;



// ===========================================================================
// ROL - Rotate Left

// -- ROL %reg --
function _rol(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C0);
end;

// -- ROL mem --
function _rol(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$00);
end;


// ===========================================================================
// ROR - Rotate Right

// -- ROR %reg --
function _ror(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C8);
end;

// -- ROR mem --
function _ror(x: TMemVar): TBytes; overload;
begin
  Result := x.Encode([$D2], EAX.Convert(x.size), Ord(x.MemType)+$08);
end;




// ===========================================================================
// SETcc - Set Byte on Condition

// -- SETc %reg --
function _setc(opcode:E_SETxx; dst: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$0F, Ord(opcode)], dst.Convert(szBYTE), -1, 1);
end;

// -- SETc mem --
function _setc(opcode:E_SETxx; dst: TMemVar): TBytes; overload;
begin
  Result := dst.EncodeR([$0F, Ord(opcode)], _AL, -1, 1);
end;



