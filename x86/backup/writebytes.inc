{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Helper functions for writing certian instructions
[-----------------------------------------------------------------------------}
{$DEFINE EmitMemReg :=
  x := Self.Emit(opcode);
  Self.EmitTo([_mem_reg(reg)], x);
  Self.EmitTo(mem, SizeOf(Pointer), x);
}

{$DEFINE EmitStkReg :=
  x := Self.Emit(opcode, False);
  Self.EmitTo([_stk_reg(stk,reg)], x);
  if (stk.Reg = ESP) then Self.EmitTo([$24], x);
  Self.EmitTo(stk.Offset, GetIntSize(x, [1,4]), x);
}

// --------------------------------------------------------------------------------------
// OP memory, register
// OP register, memory
procedure TSlackASM.EmitMemInstr4(opcode:array of Byte; mem: Pointer; reg: TGPRegister32); var x,i:Int32; begin {$IFDEF FPC}EmitMemReg{$ELSE}{$MACRO EmitMemReg}{$ENDIF} end;
procedure TSlackASM.EmitMemInstr2(opcode:array of Byte; mem: Pointer; reg: TGPRegister16); var x,i:Int32; begin {$IFDEF FPC}EmitMemReg{$ELSE}{$MACRO EmitMemReg}{$ENDIF} end;
procedure TSlackASM.EmitMemInstr1(opcode:array of Byte; mem: Pointer; reg: TGPRegister8);  var x,i:Int32; begin {$IFDEF FPC}EmitMemReg{$ELSE}{$MACRO EmitMemReg}{$ENDIF} end;


// --------------------------------------------------------------------------------------
// OP stack, register
// OP register, stack
procedure TSlackASM.EmitStkInstr4(opcode:array of Byte; stk: TStackVar; reg: TGPRegister32); var x,i:Int32; begin{$IFDEF FPC}EmitStkReg{$ELSE}{$MACRO EmitStkReg}{$ENDIF} end;
procedure TSlackASM.EmitStkInstr2(opcode:array of Byte; stk: TStackVar; reg: TGPRegister16); var x,i:Int32; begin{$IFDEF FPC}EmitStkReg{$ELSE}{$MACRO EmitStkReg}{$ENDIF} end;
procedure TSlackASM.EmitStkInstr1(opcode:array of Byte; stk: TStackVar; reg: TGPRegister8);  var x,i:Int32; begin{$IFDEF FPC}EmitStkReg{$ELSE}{$MACRO EmitStkReg}{$ENDIF} end;





