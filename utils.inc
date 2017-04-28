{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Utilities
[-----------------------------------------------------------------------------}
function GetIntSize(x: Int64; validSizes: TByteSet=[1,4]): Int32;
begin
  if (Low(Int8)  <= x) and (High(Int8)  >= x) and (1 in validSizes) then Exit(1);
  if (Low(Int16) <= x) and (High(Int16) >= x) and (2 in validSizes) then Exit(2);
  if (Low(Int32) <= x) and (High(Int32) >= x) and (4 in validSizes) then Exit(4);
  if (Low(Int64) <= x) and (High(Int64) >= x) and (8 in validSizes) then Exit(8);
end;

function GetUIntSize(x: UInt64; validSizes: TByteSet=[1,4]): Int32;
begin
  if (High(UInt8)  >= x) and (1 in validSizes) then Exit(1);
  if (High(UInt16) >= x) and (2 in validSizes) then Exit(2);
  if (High(UInt32) >= x) and (4 in validSizes) then Exit(4);
  if (High(UInt64) >= x) and (8 in validSizes) then Exit(4);
end;


function addr_to_bytes(x: Pointer): TBytes;
begin
  SetLength(Result, PTR_SIZE);
  MemMove(x, Result[0], PTR_SIZE);
end;

function long_to_bytes(x: Int32): TBytes;
begin
  SetLength(Result, LONG_SIZE);
  MemMove(x, Result[0], LONG_SIZE);
end;

function word_to_bytes(x: Int16): TBytes;
begin
  SetLength(Result, WORD_SIZE);
  MemMove(x, Result[0], WORD_SIZE);
end;


//Used to do this: mov (%eax), %ecx
//  A) _mov(ref(eax), eax);
function ref(Left: TGPRegister): TPtrAtGPRegister;
begin
  Result.Reg := Left;
end;


// immediate constructions
function imm(v: Int32): TImmediate;
begin
  Result.Size := LONG_SIZE;
  SetLength(Result.Value, Result.Size);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm8(v: Int8): TImmediate;
begin
  Result.Size := BYTE_SIZE;
  SetLength(Result.Value, LONG_SIZE);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm16(v: Int16): TImmediate;
begin
  Result.Size := WORD_SIZE;
  SetLength(Result.Value, LONG_SIZE);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm32(v: Int32): TImmediate;
begin
  Result.Size := LONG_SIZE;
  SetLength(Result.Value, LONG_SIZE);
  MemMove(v, Result.Value[0], Result.Size);
end;


// stack variable construction
operator + (reg:TGPRegister; offset:Int32): TStackVar;
begin
  Result.reg    := reg;
  Result.offset := imm(offset);
end;

operator - (reg:TGPRegister; offset:Int32): TStackVar;
begin
  Result.reg    := reg;
  Result.offset := imm(-offset);
end;




// ---------------------------------------------------------------------------
// lape helpers bellow

{$IFDEF LAPE}
  function ToString(x: TBytes): string; override;
  var i:Int32;
  begin
    Result := ''
    for i:=0 to High(x) do
      Result += IntToHex(x[i],2) + ' ';
  end;

  function ToString(x: TNativeCode): string; override;
  var i:Int32;
  begin
    Result := ''
    for i:=0 to High(x) do
    begin
      Result += ToString(x[i]);
      if i < High(x) then Result += LineEnding;
    end;
  end;
{$ENDIF}