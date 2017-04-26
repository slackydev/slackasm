{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Various functions that makes producing the machinecode a little simpler
[-----------------------------------------------------------------------------}

function long_to_bytes(myLong: Int32): TBytes;
begin
  SetLength(Result, LONG_SIZE);
  MemMove(myLong, Result[0], LONG_SIZE);
end;

function addr_to_bytes(addr: Pointer): TBytes;
begin
  SetLength(Result, PTR_SIZE);
  MemMove(addr, Result[0], PTR_SIZE);
end;

function __reg_operand2(reg1, reg2: Byte): Byte; overload; 
begin 
  Result := $C0 + reg2*8 + reg1; 
end;

function __mem_operand2(reg: Byte): Byte; overload;
begin 
  Result := reg*8 + $05; 
end;


// ----------------------------------------------------------------------------
// OP register, register
function _reg_operand(opcode:array of Byte; reg1, reg2: TGPRegister): TBytes; 
begin
  Result := opcode + ToBytes([__reg_operand2(reg1.gpReg, reg2.gpReg)]);
end;


// ----------------------------------------------------------------------------
// OP memory, register | OP register, memory
function _mem_operand(opcode:array of Byte; mem: Pointer; reg: TGPRegister): TBytes;
begin
  Result := opcode + ToBytes([__mem_operand2(reg.gpReg)]) + addr_to_bytes(mem);
end;


// ----------------------------------------------------------------------------
// OP memory
function _mem_operand_no_reg(opcode:array of Byte; mem: Pointer): TBytes;
begin
  Result := opcode + addr_to_bytes(mem);
end;
