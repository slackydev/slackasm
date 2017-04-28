{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the general purpose registers
[-----------------------------------------------------------------------------}

// ===========================================================================
// MOV - Move (Copy) Left Operand into Right Operand

// -- MOV %reg, %reg --
function TSlackASM._mov(x,y: TGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$88], y);
end;

// -- MOV %reg, mem --
function TSlackASM._mov(x: TGPRegister; y: Pointer): TBytes; overload; 
begin
  Result := x.EncodeMem([$88], y);
end;

// -- MOV mem, %reg --
function TSlackASM._mov(x: Pointer; y: TGPRegister): TBytes; overload; 
begin 
  Result := y.EncodeMem([$8A], x);
end;

// -- MOV (%reg), %reg --
function TSlackASM._mov(x: TPtrAtGPRegister; y: TGPRegister): TBytes; overload; 
begin
  Result := y.Encode([$8A], x.reg, $00);
end;

// -- MOV %reg, (%reg) --
function TSlackASM._mov(x: TGPRegister; y: TPtrAtGPRegister): TBytes; overload; 
begin
  Result := x.Encode([$88], y.reg, $00);
end;

// -- MOV imm, %reg --
function TSlackASM._mov(x: TImmediate; y: TGPRegister): TBytes; overload; 
begin
  Result := x.EncodeHC([$B0],[$B8],[$B8], y);
end;


// ===========================================================================
// MOVZX - Move (Copy) Left Operand into Right Operand With Zero-Extend

// -- MOVZX %reg, %reg --
function TSlackASM._movzx(x,y: TGPRegister): TBytes; overload; 
begin
  if (y.Size <= x.Size) or (y.Size < WORD_SIZE) then
    WriteLn('movzx: Incompatible Operands Size: dest = ', y.Size, ', src = ', x.Size)
  else
    Result := y.Encode0F([$0F,$B6], x);
end;

// -- MOVZX mem, %reg --
function TSlackASM._movzx(x: Pointer; y: TGPRegister; MemSize:Byte=BYTE_SIZE): TBytes; overload;
begin
  if (y.Size <= MemSize) or (y.Size < WORD_SIZE) then
    WriteLn('movzx: Incompatible Operands Size: dest = ', y.Size, ', src = ', MemSize)
  else
    Result := y.EncodeMem0F([$0F,$B6], x, MemSize);
end;



// ===========================================================================
// MOVSX - Move (Copy) Left Operand into Right Operand With Sign-Extend

// -- MOVSX %reg, %reg --
function TSlackASM._movsx(x,y: TGPRegister): TBytes; overload; 
begin
  if (y.Size <= x.Size) or (y.Size < WORD_SIZE) then
    WriteLn('movsx: Incompatible Operands Size: dest = ', y.Size, ', src = ', x.Size)
  else
    Result := y.Encode0F([$0F,$BE], x);
end;

// -- MOVSX mem, %reg --
function TSlackASM._movsx(x: Pointer; y: TGPRegister; MemSize:Byte=BYTE_SIZE): TBytes; overload; 
begin
  if (y.Size <= MemSize) or (y.Size < WORD_SIZE) then
    WriteLn('movsx: Incompatible Operands Size: dest = ', y.Size, ', src = ', MemSize)
  else
    Result := y.EncodeMem0F([$0F,$BE], x, MemSize);
end;




// ===========================================================================
// INC - Increment Operand
function TSlackASM._inc(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x);
end;

function TSlackASM._inc(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$FE], x, $05);
end;


// ===========================================================================
// DEC - Decrement Operand
function TSlackASM._dec(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$FE], x, $C8);
end;

function TSlackASM._dec(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$FE], x, $0D);
end;


// ===========================================================================
// CDQ - Convert Doubleword to Quadword (EDX:EAX = sign-extend of EAX)
function TSlackASM._cdq(): TBytes;
begin
  Result := [$99];
end;

function TSlackASM._cltq(): TBytes;
begin
  Result := Self._cdq();
end;


// ===========================================================================
// CMP - Compare 

// -- CMP %reg, %reg --
function TSlackASM._cmp(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$38], x);
end;

// -- CMP mem, %reg --
function TSlackASM._cmp(x: Pointer; y: TGPRegister): TBytes; overload;
begin
  Result := y.EncodeMem([$3A], x);
end;


// ===========================================================================
// Test - Compare 

// -- TEST %reg, %reg --
function TSlackASM._test(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$84], x);
end;

// -- TEST mem, %reg --
function TSlackASM._test(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$86], x);
end;



// ===========================================================================
// AND - Logical And

// -- AND %reg, %reg --
function TSlackASM._and(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$20], x);
end;

// -- AND mem, %reg --
function TSlackASM._and(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$22], x);
end;


// ===========================================================================
// OR - Logical Inclusive Or

// -- OR %reg, %reg --
function TSlackASM._or(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$08], x);
end;

// -- OR mem, %reg --
function TSlackASM._or(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$0A], x);
end;


// ===========================================================================
// XOR - Logical Exclusive OR

// -- OR %reg, %reg --
function TSlackASM._xor(x, y: TGPRegister): TBytes; overload;
begin 
  Result := y.Encode([$30], x);
end;

// -- OR mem, %reg --
function TSlackASM._xor(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$32], x);
end;


// ===========================================================================
// ADD - Addition

// -- ADD %reg, %reg --
function TSlackASM._add(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$00], x);
end;

// -- ADD mem, %reg --
function TSlackASM._add(x: Pointer; y: TGPRegister): TBytes; overload;
begin
  Result := y.EncodeMem([$02], x);
end;

// -- ADD imm, %reg --
function TSlackASM._add(x: TImmediate; y: TGPRegister): TBytes; overload;
begin                  {not sure why byte uses word prefix}
  Result := x.EncodeHC([$66,$83,$C0],[$81,$C0],[$81,$C0], y);
end;


// ===========================================================================
// ADC - Arithmetic Addition with Carry

// -- ADC %reg, %reg --
function TSlackASM._adc(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$10], x);
end;

// -- ADC mem, %reg --
function TSlackASM._adc(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$12], x);
end;


// ===========================================================================
// SUB - Subtract

// -- SUB %reg, %reg --
function TSlackASM._sub(x, y: TGPRegister): TBytes; overload;
begin
  Result := y.Encode([$28], x);
end;

// -- SUB mem, %reg --
function TSlackASM._sub(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$2A], x);
end;


// ===========================================================================
// SUB - Subtract With Borrow

// -- SBB %reg, %reg --
function TSlackASM._sbb(x, y: TGPRegister): TBytes; overload;
begin 
  Result := y.Encode([$18], x);
end;

// -- SBB mem, %reg --
function TSlackASM._sbb(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := y.EncodeMem([$1A], x);
end;


// ===========================================================================
// DIV - Unsigned division

// -- DIV %reg --
function TSlackASM._div(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F0);
end;

// -- DIV mem --
function TSlackASM._div(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$F6], x, $35);
end;


// ===========================================================================
// IDIV - Signed division

// -- IDIV %reg --
function TSlackASM._idiv(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $F8);
end;

// -- IDIV mem --
function TSlackASM._idiv(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$F6], x, $3D);
end;



// ===========================================================================
// MUL - Unsigned multiplication

// -- MUL %reg --
function TSlackASM._mul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E0);
end;

// -- MUL mem --
function TSlackASM._mul(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$F6], x, $25);
end;



// ===========================================================================
// IMUL - Signed Multiplication

// -- IMUL %reg --
function TSlackASM._imul(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$F6], x, $E8);
end;

// -- IMUL mem --
function TSlackASM._imul(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$F6], x, $2D);
end;

// -- IMUL %reg, %reg --
function TSlackASM._imul(x, y: TGPRegister): TBytes; overload;
begin
  if y.size = 1 then
  begin
    WriteLn('IMUL: Invalid operand size: 1');
    Exit(_nop);
  end;
  Result := y.Encode([$0E, $AF], x);
end;

// -- IMUL mem,  %reg --
function TSlackASM._imul(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  if y.Size = 1 then
  begin
    WriteLn('IMUL: Invalid operand size: 1');
    Exit(_nop);
  end;
    Result := y.EncodeMem([$0E,$AF], x);
end;



// ===========================================================================
// SAL - Shift Arithmetic Left

//SAL %cl, %reg
function TSlackASM._sal(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

//SAL %cl, %mem
function TSlackASM._sal(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $25);
end;


// ===========================================================================
// SAR - Shift Arithmetic Right

// -- SAR %reg --
function TSlackASM._sar(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $F8);
end;

// -- SAR mem --
function TSlackASM._sar(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $3D);
end;


// ===========================================================================
// SHL - Shift Left

// -- SHL %reg --
function TSlackASM._shl(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E0);
end;

// -- SHL mem --
function TSlackASM._shl(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $25);
end;



// ===========================================================================
// SHR - Shift Right

// -- SHL %reg --
function TSlackASM._shr(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $E8);
end;

// -- SHL mem --
function TSlackASM._shr(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $2D);
end;



// ===========================================================================
// ROL - Rotate Left

// -- ROL %reg --
function TSlackASM._rol(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C0);
end;

// -- ROL mem --
function TSlackASM._rol(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $05);
end;


// ===========================================================================
// ROR - Rotate Right

// -- ROR %reg --
function TSlackASM._ror(x: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode([$D2], x, $C8);
end;

// -- ROR mem --
function TSlackASM._ror(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := EAX.Convert(MemSize).EncodeMem([$D2], x, $0D);
end;




// ===========================================================================
// SETxx - Set Byte on Condition

// -- SETx %reg --
function TSlackASM._setx(opcode:E_SETxx; dst: TGPRegister): TBytes; overload;
begin
  Result := EAX.Encode0F([$0F, Ord(opcode)], dst.Convert(BYTE_SIZE));
end;

// -- SETx mem --
function TSlackASM._setx(opcode:E_SETxx; dst: Pointer): TBytes; overload;
begin
  Result := _AL.EncodeMem([$0F, Ord(opcode)], dst);
end;



