{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Working with the various flags
[-----------------------------------------------------------------------------}

// CMC - Complement Carry Flag
procedure TSlackASM._cmc;  begin Self.WriteBytes([$F5]); end;
// CLC - Clear Carry Flag
procedure TSlackASM._clc;  begin Self.WriteBytes([$F8]); end;
// STC - Set Carry Flag
procedure TSlackASM._stc;  begin Self.WriteBytes([$F9]); end;
// CLD - Clear Direction Flag
procedure TSlackASM._cld;  begin Self.WriteBytes([$FC]); end;
// STD - Set Direction Flag
procedure TSlackASM._std;  begin Self.WriteBytes([$FD]); end;
// CLI - Clear Interrupt Flag
procedure TSlackASM._cli;  begin Self.WriteBytes([$FA]); end;
// STI - Set Interrupt Flag
procedure TSlackASM._sti;  begin Self.WriteBytes([$FB]); end;
// CLTS - Clear Task-Switched Flag in CR0
procedure TSlackASM._clts; begin Self.WriteBytes([$0F, $06]); end;
// LAHF - Load Status Flags into AH Register
procedure TSlackASM._lahf; begin Self.WriteBytes([$9F]); end;
// SAHF - Store AH into Flags
procedure TSlackASM._sahf; begin Self.WriteBytes([$9E]); end;




