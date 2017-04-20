{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Helper functions for writing certian instructions
[-----------------------------------------------------------------------------}

// --------------------------------------------------------------------------------------
// OP memory, register
// OP register, memory
procedure TSlackASM.WriteMemInstr4(opcode:array of Byte; mem: Pointer; reg: TGPRegister32);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_mem_reg(reg)], False);
  Self.WriteAddr(mem);
end;

procedure TSlackASM.WriteMemInstr2(opcode:array of Byte; mem: Pointer; reg: TGPRegister16);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_mem_reg(reg)], False);
  Self.WriteAddr(mem);
end;

procedure TSlackASM.WriteMemInstr1(opcode:array of Byte; mem: Pointer; reg: TGPRegister8);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_mem_reg(reg)], False);
  Self.WriteAddr(mem);
end;


// --------------------------------------------------------------------------------------
// OP stack, register
// OP register, stack
procedure TSlackASM.WriteStkInstr4(opcode:array of Byte; stk: TStackVar; reg: TGPRegister32);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_stk_reg(stk,reg)], False);
  if (stk.Reg = ESP) then
    Self.WriteBytes([$24], False);

  if (Low(Int8) <= stk.Offset) and (High(Int8) >= stk.Offset) then
    Self.WriteInt(stk.Offset, 1, True)
  else
    Self.WriteInt(stk.Offset, 4, True);
end;

procedure TSlackASM.WriteStkInstr2(opcode:array of Byte; stk: TStackVar; reg: TGPRegister16);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_stk_reg(stk,reg)], False);
  if (stk.Reg = ESP) then
    Self.WriteBytes([$24], False);

  if (Low(Int8) <= stk.Offset) and (High(Int8) >= stk.Offset) then
    Self.WriteInt(stk.Offset, 1, True)
  else
    Self.WriteInt(stk.Offset, 4, True);
end;

procedure TSlackASM.WriteStkInstr1(opcode:array of Byte; stk: TStackVar; reg: TGPRegister8);
var i:Int32;
begin
  for i:=0 to High(opcode) do
    Self.WriteBytes([opcode[i]], False);
  Self.WriteBytes([_stk_reg(stk,reg)], False);
  if (stk.Reg = ESP) then
    Self.WriteBytes([$24], False);

  if (Low(Int8) <= stk.Offset) and (High(Int8) >= stk.Offset) then
    Self.WriteInt(stk.Offset, 1, True)
  else
    Self.WriteInt(stk.Offset, 4, True);
end;