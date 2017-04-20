{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the general purpose registers
[-----------------------------------------------------------------------------}

// ===========================================================================
// MOV - Move (Copy) Left Operand into Right Operand

// -- MOV %reg, %reg --
procedure TSlackASM._mov(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$89, _reg_reg(dst, src)]); end;

// -- MOV %reg, mem --
procedure TSlackASM._movl(src: TGPRegister32; dst: PInt32); overload; begin Self.WriteMemInstr4([$89], dst, src); end;

// -- MOV %reg, stk --
procedure TSlackASM._movl(reg: TGPRegister32; stv: TStackVar); overload; begin Self.WriteStkInstr4([$89], stv, reg); end;

// --  MOV mem, %reg --
procedure TSlackASM._movl(src: PInt32; dst: TGPRegister32);   overload; begin Self.WriteMemInstr4([$8B], src, dst); end;

// --  MOV stk, %reg --
procedure TSlackASM._movl(stv: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$8B], stv, reg); end;

// TODO..: add support for byte and word
//


// ===========================================================================
// INC - Increment Operand
procedure TSlackASM._inc(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$40, Ord(reg)]);
end;

procedure TSlackASM._incl(mem: PInt32); overload;
begin
  Self.WriteBytes([$FF,$05]);
  Self.WriteAddr(mem);
end;


// ===========================================================================
// DEC - Decrement Operand
procedure TSlackASM._dec(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$48, Ord(reg)]);
end;

procedure TSlackASM._decl(mem: PInt32); overload;
begin
  Self.WriteBytes([$FF,$0D]);
  Self.WriteAddr(mem);
end;


// ===========================================================================
// CDQ - Convert Doubleword to Quadword (EDX:EAX = sign-extend of EAX)
procedure TSlackASM._cdq();
begin
  Self.WriteBytes([$99]);
end;

procedure TSlackASM._cltq();
begin
  Self._cdq();
end;


// ===========================================================================
// CMP - Compare 

// -- CMP %reg, %reg --
procedure TSlackASM._cmpb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$38, _reg_reg(src, dst)]); end;
procedure TSlackASM._cmpw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $39, _reg_reg(src, dst)]); end;
procedure TSlackASM._cmpl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$39, _reg_reg(src, dst)]); end;

// -- CMP mem,  %reg --
procedure TSlackASM._cmpb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$3A], mem, reg); end;
procedure TSlackASM._cmpw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $3B], mem, reg); end;
procedure TSlackASM._cmpl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$3B], mem, reg); end;

// -- CMP stk,  %reg --
procedure TSlackASM._cmpb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$3A], stk, reg); end;
procedure TSlackASM._cmpw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $3B], stk, reg); end;
procedure TSlackASM._cmpl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$3B], stk, reg); end;


// ===========================================================================
// AND - Logical And

// -- AND %reg, %reg --
procedure TSlackASM._andb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$20, _reg_reg(src, dst)]); end;
procedure TSlackASM._andw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $21, _reg_reg(src, dst)]); end;
procedure TSlackASM._andl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$21, _reg_reg(src, dst)]); end;

// -- AND mem,  %reg --
procedure TSlackASM._andb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$22], mem, reg); end;
procedure TSlackASM._andw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $23], mem, reg); end;
procedure TSlackASM._andl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$23], mem, reg); end;

// -- AND stk,  %reg --
procedure TSlackASM._andb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$22], stk, reg); end;
procedure TSlackASM._andw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $23], stk, reg); end;
procedure TSlackASM._andl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$23], stk, reg); end;


// ===========================================================================
// OR - Logical Inclusive Or

// -- OR %reg, %reg --
procedure TSlackASM._orb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$08, _reg_reg(src, dst)]); end;
procedure TSlackASM._orw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $09, _reg_reg(src, dst)]); end;
procedure TSlackASM._orl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$09, _reg_reg(src, dst)]); end;

// -- OR mem,  %reg --
procedure TSlackASM._orb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$0A], mem, reg); end;
procedure TSlackASM._orw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $0B], mem, reg); end;
procedure TSlackASM._orl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$0B], mem, reg); end;

// -- OR stk,  %reg --
procedure TSlackASM._orb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$0A], stk, reg); end;
procedure TSlackASM._orw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $0B], stk, reg); end;
procedure TSlackASM._orl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$0B], stk, reg); end;


// ===========================================================================
// XOR - Logical Exclusive OR

// -- XOR %reg, %reg --
procedure TSlackASM._xorb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$30, _reg_reg(src, dst)]); end;
procedure TSlackASM._xorw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $31, _reg_reg(src, dst)]); end;
procedure TSlackASM._xorl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$31, _reg_reg(src, dst)]); end;

// -- XOR mem,  %reg --
procedure TSlackASM._xorb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$32], mem, reg); end;
procedure TSlackASM._xorw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $33], mem, reg); end;
procedure TSlackASM._xorl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$33], mem, reg); end;

// -- XOR stk,  %reg --
procedure TSlackASM._xorb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$32], stk, reg); end;
procedure TSlackASM._xorw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $33], stk, reg); end;
procedure TSlackASM._xorl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$33], stk, reg); end;



// ===========================================================================
// ADD - Addition

// -- ADD %reg, %reg --
procedure TSlackASM._addb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$00, _reg_reg(src, dst)]); end;
procedure TSlackASM._addw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $01, _reg_reg(src, dst)]); end;
procedure TSlackASM._addl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$01, _reg_reg(src, dst)]); end;

// -- ADD mem,  %reg --
procedure TSlackASM._addb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$02], mem, reg); end;
procedure TSlackASM._addw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $03], mem, reg); end;
procedure TSlackASM._addl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$03], mem, reg); end;

// -- ADD stk,  %reg --
procedure TSlackASM._addb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$02], stk, reg); end;
procedure TSlackASM._addw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $03], stk, reg); end;
procedure TSlackASM._addl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$03], stk, reg); end;


// ===========================================================================
// ADC - Arithmetic Addition with Carry

// -- ADC %reg, %reg -----------------------------
procedure TSlackASM._adcb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$10, _reg_reg(src, dst)]); end;
procedure TSlackASM._adcw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $11, _reg_reg(src, dst)]); end;
procedure TSlackASM._adcl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$11, _reg_reg(src, dst)]); end;

// -- ADC mem, %reg -------------------------------------
procedure TSlackASM._adcb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$12], mem, reg); end;
procedure TSlackASM._adcw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $13], mem, reg); end;
procedure TSlackASM._adcl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$13], mem, reg); end;

// -- ADC stk, %reg -------------------------------------
procedure TSlackASM._adcb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$12], stk, reg); end;
procedure TSlackASM._adcw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $13], stk, reg); end;
procedure TSlackASM._adcl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$13], stk, reg); end;


// ===========================================================================
// SUB - Subtract

// -- SUB %reg, %reg --
procedure TSlackASM._subb(src, dst: TGPRegister8);  overload; begin Self.WriteBytes([$28, _reg_reg(src, dst)]); end;
procedure TSlackASM._subw(src, dst: TGPRegister16); overload; begin Self.WriteBytes([$66, $29, _reg_reg(src, dst)]); end;
procedure TSlackASM._subl(src, dst: TGPRegister32); overload; begin Self.WriteBytes([$29, _reg_reg(src, dst)]); end;

// -- SUB mem,  %reg --
procedure TSlackASM._subb(mem: PInt32; reg: TGPRegister8);  overload; begin Self.WriteMemInstr1([$2A], mem, reg); end;
procedure TSlackASM._subw(mem: PInt16; reg: TGPRegister16); overload; begin Self.WriteMemInstr2([$66, $2B], mem, reg); end;
procedure TSlackASM._subl(mem: PInt32; reg: TGPRegister32); overload; begin Self.WriteMemInstr4([$2B], mem, reg); end;

// -- SUB stk,  %reg --
procedure TSlackASM._subb(stk: TStackVar; reg: TGPRegister8);  overload; begin Self.WriteStkInstr1([$2A], stk, reg); end;
procedure TSlackASM._subw(stk: TStackVar; reg: TGPRegister16); overload; begin Self.WriteStkInstr2([$66, $2B], stk, reg); end;
procedure TSlackASM._subl(stk: TStackVar; reg: TGPRegister32); overload; begin Self.WriteStkInstr4([$2B], stk, reg); end;



// ===========================================================================
// DIV - Unsigned division

// -- DIV %reg ------------------------------
procedure TSlackASM._divb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$F6, $F0+ord(reg)]);
end;

procedure TSlackASM._divw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $F7, $F0+ord(reg)]);
end; 

procedure TSlackASM._divl(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$F7, $F0+ord(reg)]);
end;

// -- DIV mem ------------------------
procedure TSlackASM._divb(mem: PInt8); overload;
begin
  Self.WriteBytes([$F6, $35]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._divw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $F7, $35]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._divl(mem: PInt32); overload;
begin
  Self.WriteBytes([$F7, $35]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// IDIV - Signed division

// -- IDIV %reg ------------------------------
procedure TSlackASM._idivb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$F6, $F8+ord(reg)]);
end;

procedure TSlackASM._idivw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $F7, $F8+ord(reg)]);
end; 

procedure TSlackASM._idivl(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$F7, $F8+ord(reg)]);
end;

// -- IDIV mem ------------------------
procedure TSlackASM._idivb(mem: PInt8); overload;
begin
  Self.WriteBytes([$F6, $3D]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._idivw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $F7, $3D]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._idivl(mem: PInt32); overload;
begin
  Self.WriteBytes([$F7, $3D]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// MUL - Unsigned multiplication

// -- MUL %reg -----------------------------
procedure TSlackASM._mulb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$F6, $E0+ord(reg)]);
end; 

procedure TSlackASM._mulw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $F7, $E0+ord(reg)]);
end; 

procedure TSlackASM._mull(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$F7, $E0+ord(reg)]);
end;

// -- MUL mem ------------------------
procedure TSlackASM._mulb(mem: PInt8); overload;
begin
  Self.WriteBytes([$F6, $25]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._mulw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $F7, $25]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._mull(mem: PInt32); overload;
begin
  Self.WriteBytes([$F7, $25]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// IMUL - Signed Multiplication

// -- IMUL %reg ------------------------------
procedure TSlackASM._imulb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$F6, $E8+ord(reg)]);
end; 

procedure TSlackASM._imulw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $F7, $E8+ord(reg)]);
end; 

procedure TSlackASM._imull(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$F7, $E8+ord(reg)]);
end;

// -- IMUL mem ------------------------
procedure TSlackASM._imulb(mem: PInt8); overload;
begin
  Self.WriteBytes([$F6, $2D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._imulw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $F7, $2D]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._imull(mem: PInt32); overload;
begin
  Self.WriteBytes([$F7, $2D]);
  Self.WriteAddr(mem);
end;

// -- IMUL %reg, %reg -----------------------------
procedure TSlackASM._imulw(src, dst: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $0F, $AF, _reg_reg(src, dst)]);
end; 

procedure TSlackASM._imull(src, dst: TGPRegister32); overload;
begin
  Self.WriteBytes([$0F, $AF, _reg_reg(src, dst)]);
end;

// -- IMUL mem, %reg --------------------------------------
procedure TSlackASM._imulw(mem: PInt16; reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $0F, $AF, _mem_reg(reg)]);
  Self.WriteAddr(mem);
end; 

procedure TSlackASM._imull(mem: PInt32; reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$0F, $AF, _mem_reg(reg)]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// SAL - Shift Arithmetic Left

//SAL %cl, %reg
procedure TSlackASM._salb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $E0+ord(reg)]);
end;

procedure TSlackASM._salw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $E0+ord(reg)]);
end;

procedure TSlackASM._sall(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $E0+ord(reg)]);
end;

//SAL %cl, %mem
procedure TSlackASM._salb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $25]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._salw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $25]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._sall(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $25]);
  Self.WriteAddr(mem);
end;


// ===========================================================================
// SAR - Shift Arithmetic Right

//SAR %cl, %reg
procedure TSlackASM._sarb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $F8+ord(reg)]);
end;

procedure TSlackASM._sarw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $F8+ord(reg)]);
end;

procedure TSlackASM._sarl(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $F8+ord(reg)]);
end;

//SAR %cl, mem
procedure TSlackASM._sarb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $3D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._sarw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $3D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._sarl(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $3D]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// SHL - Shift Left

//SHL %cl, %reg
procedure TSlackASM._shlb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $E0+ord(reg)]);
end;

procedure TSlackASM._shlw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $E0+ord(reg)]);
end;

procedure TSlackASM._shll(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $E0+ord(reg)]);
end;

//SHL %cl, mem
procedure TSlackASM._shlb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $25]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._shlw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $25]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._shll(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $25]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// SHR - Shift Right

//SHR %cl, %reg
procedure TSlackASM._shrb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $E8+ord(reg)]);
end;

procedure TSlackASM._shrw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $E8+ord(reg)]);
end;

procedure TSlackASM._shrl(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $E8+ord(reg)]);
end;

//SHR %cl, mem
procedure TSlackASM._shrb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $2D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._shrw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $2D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._shrl(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $2D]);
  Self.WriteAddr(mem);
end;



// ===========================================================================
// ROL - Rotate Left

//ROL %cl, %reg
procedure TSlackASM._rolb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $C0+ord(reg)]);
end;

procedure TSlackASM._rolw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $C0+ord(reg)]);
end;

procedure TSlackASM._roll(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $C0+ord(reg)]);
end;

//ROL %cl, mem
procedure TSlackASM._rolb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $05]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._rolw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $05]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._roll(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $05]);
  Self.WriteAddr(mem);
end;


// ===========================================================================
// ROR - Rotate Right

//ROR %cl, %reg
procedure TSlackASM._rorb(reg: TGPRegister8); overload;
begin
  Self.WriteBytes([$D2, $C8+ord(reg)]);
end;

procedure TSlackASM._rorw(reg: TGPRegister16); overload;
begin
  Self.WriteBytes([$66, $D3, $C8+ord(reg)]);
end;

procedure TSlackASM._rorl(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$D3, $C8+ord(reg)]);
end;

//ROR %cl, %mem
procedure TSlackASM._rorb(mem: PInt8); overload;
begin
  Self.WriteBytes([$D2, $0D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._rorw(mem: PInt16); overload;
begin
  Self.WriteBytes([$66, $D3, $0D]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._rorl(mem: PInt32); overload;
begin
  Self.WriteBytes([$D3, $0D]);
  Self.WriteAddr(mem);
end;





