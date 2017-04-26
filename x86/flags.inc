{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Working with the various flags
[-----------------------------------------------------------------------------}

// CMC - Complement Carry Flag
function TSlackASM._cmc: TBytes;  begin Result := ToBytes([$F5]); end;

// CLC - Clear Carry Flag
function TSlackASM._clc: TBytes;  begin Result := ToBytes([$F8]); end;

// STC - Set Carry Flag
function TSlackASM._stc: TBytes;  begin Result := ToBytes([$F9]); end;

// CLD - Clear Direction Flag
function TSlackASM._cld: TBytes;  begin Result := ToBytes([$FC]); end;

// STD - Set Direction Flag
function TSlackASM._std: TBytes;  begin Result := ToBytes([$FD]); end;

// CLI - Clear Interrupt Flag
function TSlackASM._cli: TBytes;  begin Result := ToBytes([$FA]); end;

// STI - Set Interrupt Flag
function TSlackASM._sti: TBytes;  begin Result := ToBytes([$FB]); end;

// CLTS - Clear Task-Switched Flag in CR0
function TSlackASM._clts: TBytes; begin Result := ToBytes([$0F, $06]); end;

// LAHF - Load Status Flags into AH Register
function TSlackASM._lahf: TBytes; begin Result := ToBytes([$9F]); end;

// SAHF - Store AH into Flags
function TSlackASM._sahf: TBytes; begin Result := ToBytes([$9E]); end;




