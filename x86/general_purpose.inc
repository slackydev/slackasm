{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the general purpose registers
[-----------------------------------------------------------------------------}

// ===========================================================================
// MOV - Move (Copy) Left Operand into Right Operand

// -- MOV %reg, %reg --
function TSlackASM._mov(x, y: TGPRegister): TBytes; overload; 
begin 
  Result := _reg_operand(RegBase([$88], y), x,y);
end;

// -- MOV %reg, mem --
function TSlackASM._mov(x: TGPRegister; y: Pointer): TBytes; overload; 
begin 
  Result := _mem_operand(RegBase([$88], x), y,x);
end;

// -- MOV mem, %reg --
function TSlackASM._mov(x: Pointer; y: TGPRegister): TBytes; overload; 
begin 
  Result := _mem_operand(RegBase([$8A], y), x,y);
end;

// ===========================================================================
// MOVZX - Move (Copy) Left Operand into Right Operand With Zero-Extend

// -- MOVZX mem, %reg --
function TSlackASM._movzx(x,y: TGPRegister): TBytes; overload; 
begin
  if (x.Size = BYTE_SIZE) and (y.Size = WORD_SIZE) then
    Result := _reg_operand([$66,$0F,$B6], x,y)
  else if (x.Size = BYTE_SIZE) and (y.Size = LONG_SIZE) then
    Result := _reg_operand([$0F,$B6], x,y)
  else if (x.Size = WORD_SIZE) and (y.Size = LONG_SIZE) then
    Result := _reg_operand([$0F,$B7], x,y)
  else
    WriteLn('movzx: Incompatible Operands Sizes: dest = ', y.Size, ', src = ', x.Size);
end;

// -- MOVZX mem, %reg --
function TSlackASM._movzx(x: Pointer; y: TGPRegister; MemSize:Byte=BYTE_SIZE): TBytes; overload; 
begin
  if (MemSize = BYTE_SIZE) and (y.Size = WORD_SIZE) then
    Result := _mem_operand([$66,$0F,$B6], x,y)
  else if (MemSize = BYTE_SIZE) and (y.Size = LONG_SIZE) then
    Result := _mem_operand([$0F,$B6], x,y)  
  else if (MemSize = WORD_SIZE) and (y.Size = LONG_SIZE) then
    Result := _mem_operand([$0F,$B7], x,y)
  else
    WriteLn('movzx: Incompatible Operands Sizes: dest = ', y.Size, ', src = ', MemSize);
end;


// ===========================================================================
// INC - Increment Operand
function TSlackASM._inc(x: TGPRegister): TBytes; overload;
begin
  if x.Size <> 1 then
    Result := RegBase([$40+x.gpReg], x, -1)
  else
    Result := [$FE] + RegBase([$C1+x.gpReg], x);
end;

function TSlackASM._inc(x: PInt32): TBytes; overload;
begin
  Result := _mem_operand_no_reg([$FF,$05], x);
end;


// ===========================================================================
// DEC - Decrement Operand
function TSlackASM._dec(x: TGPRegister): TBytes; overload;
begin
  if x.Size <> 1 then
    Result := RegBase([$48+x.gpReg], x, -1)
  else
    Result := [$FE] + RegBase([$C8+x.gpReg], x);
end;

function TSlackASM._dec(x: PInt32): TBytes; overload;
begin
  Result := _mem_operand_no_reg([$FF,$0D], x);
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
  Result := _reg_operand(RegBase([$38], y), x,y); 
end;

// -- CMP mem,  %reg --
function TSlackASM._cmp(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$3A], y), x,y); 
end;


// ===========================================================================
// AND - Logical And

// -- AND %reg, %reg --
function TSlackASM._and(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$20], y), x,y); 
end;

// -- AND mem,  %reg --
function TSlackASM._and(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$22], y), x,y); 
end;


// ===========================================================================
// OR - Logical Inclusive Or

// -- OR %reg, %reg --
function TSlackASM._or(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$08], y), x,y); 
end;

// -- OR mem,  %reg --
function TSlackASM._or(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$0A], y), x,y); 
end;


// ===========================================================================
// XOR - Logical Exclusive OR

// -- OR %reg, %reg --
function TSlackASM._xor(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$30], y), x,y); 
end;

// -- OR mem,  %reg --
function TSlackASM._xor(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$32], y), x,y); 
end;


// ===========================================================================
// ADD - Addition

// -- ADD %reg, %reg --
function TSlackASM._add(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$00], y), x,y); 
end;

// -- ADD mem,  %reg --
function TSlackASM._add(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$02], y), x,y); 
end;


// ===========================================================================
// ADC - Arithmetic Addition with Carry

// -- ADC %reg, %reg --
function TSlackASM._adc(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$10], y), x,y); 
end;

// -- ADC mem,  %reg --
function TSlackASM._adc(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$12], y), x,y); 
end;


// ===========================================================================
// SUB - Subtract

// -- SUB %reg, %reg --
function TSlackASM._sub(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$28], y), x,y); 
end;

// -- SUB mem,  %reg --
function TSlackASM._sub(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$2A], y), x,y); 
end;


// ===========================================================================
// SUB - Subtract With Borrow

// -- SBB %reg, %reg --
function TSlackASM._sbb(x, y: TGPRegister): TBytes; overload;
begin 
  Result := _reg_operand(RegBase([$18], y), x,y); 
end;

// -- SBB mem,  %reg --
function TSlackASM._sbb(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  Result := _mem_operand(RegBase([$1A], y), x,y); 
end;


// ===========================================================================
// DIV - Unsigned division

// -- DIV %reg --
function TSlackASM._div(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$F6, $F0+x.gpReg], x); 
end;

// -- DIV mem --
function TSlackASM._div(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin 
  Result := _mem_operand_no_reg(RegBaseBySize([$F6, $35], ptrSize), x);
end;


// ===========================================================================
// IDIV - Signed division

// -- IDIV %reg --
function TSlackASM._idiv(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$F6, $F8+x.gpReg], x);
end;

// -- IDIV mem --
function TSlackASM._idiv(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$F6, $3D], ptrSize), x);
end;



// ===========================================================================
// MUL - Unsigned multiplication

// -- MUL %reg --
function TSlackASM._mul(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$F6, $E0+x.gpReg], x); 
end;

// -- MUL mem --
function TSlackASM._mul(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin 
  Result := _mem_operand_no_reg(RegBaseBySize([$F6, $25], ptrSize), x);
end;



// ===========================================================================
// IMUL - Signed Multiplication

// -- IMUL %reg --
function TSlackASM._imul(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$F6, $E8+x.gpReg], x);
end;

// -- IMUL mem --
function TSlackASM._imul(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin 
  Result := _mem_operand_no_reg(RegBaseBySize([$F6, $2D], ptrSize), x);
end;

// -- IMUL %reg, %reg --
function TSlackASM._imul(x, y: TGPRegister): TBytes; overload;
begin
  if y.size = 1 then
  begin
    WriteLn('IMUL: Invalid size: 1');
    Exit;
  end;
  Result := _reg_operand(RegBase([$0F, $AF], y, -1), x,y); 
end;

// -- IMUL mem,  %reg --
function TSlackASM._imul(x: Pointer; y: TGPRegister): TBytes; overload;
begin 
  if y.size = 1 then
  begin
    WriteLn('IMUL: Invalid size: 1');
    Exit;
  end;
  Result := _mem_operand(RegBase([$0F, $AF], y, -1), x,y); 
end;



// ===========================================================================
// SAL - Shift Arithmetic Left

//SAL %cl, %reg
function TSlackASM._sal(x: TGPRegister): TBytes; overload;
begin
  Result := RegBase([$D2, $E0+x.gpReg], x);
end;

//SAL %cl, %mem
function TSlackASM._sal(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $25], ptrSize), x);
end;


// ===========================================================================
// SAR - Shift Arithmetic Right

// -- SAR %reg --
function TSlackASM._sar(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$D2, $F8+x.gpReg], x);
end;

// -- SAR mem --
function TSlackASM._sar(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $3D], ptrSize), x);
end;


// ===========================================================================
// SHL - Shift Left

// -- SHL %reg --
function TSlackASM._shl(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$D2, $E0+x.gpReg], x);
end;

// -- SHL mem --
function TSlackASM._shl(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $25], ptrSize), x);
end;



// ===========================================================================
// SHR - Shift Right

// -- SHL %reg --
function TSlackASM._shr(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$D2, $E8+x.gpReg], x);
end;

// -- SHL mem --
function TSlackASM._shr(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $2D], ptrSize), x);
end;



// ===========================================================================
// ROL - Rotate Left

// -- ROL %reg --
function TSlackASM._rol(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$D2, $C0+x.gpReg], x);
end;

// -- ROL mem --
function TSlackASM._rol(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $05], ptrSize), x);
end;


// ===========================================================================
// ROR - Rotate Right

// -- ROR %reg --
function TSlackASM._ror(x: TGPRegister): TBytes; overload;
begin 
  Result := RegBase([$D2, $C8+x.gpReg], x);
end;

// -- ROR mem --
function TSlackASM._ror(x: Pointer; ptrSize: Byte=LONG_SIZE): TBytes; overload;
begin
  Result := _mem_operand_no_reg(RegBaseBySize([$D2, $0D], ptrSize), x);
end;






