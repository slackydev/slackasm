{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Utilities
[-----------------------------------------------------------------------------}
{$MACRO ON}
{$IFNDEF LAPE}{$DEFINE MemMove := Move}{$ENDIF}

procedure FreeMethod(ptr: Pointer);
begin
  VirtualFree(ptr, 0, $8000);
end;


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
  SetLength(Result, szNATIVE);
  MemMove(x, Result[0], szNATIVE);
end;

function long_to_bytes(x: Int32): TBytes;
begin
  SetLength(Result, szLONG);
  MemMove(x, Result[0], szLONG);
end;

function word_to_bytes(x: Int16): TBytes;
begin
  SetLength(Result, szWORD);
  MemMove(x, Result[0], szWORD);
end;


// immediate constructions
function imm(v: Int32; Size:Byte=szLONG): TImmediate;
begin
  Result.Size := Size;
  SetLength(Result.Value, Result.Size);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm8(v: Int32): TImmediate;
begin
  Result.Size := szBYTE;
  SetLength(Result.Value, szLONG);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm16(v: Int32): TImmediate;
begin
  Result.Size := szWORD;
  SetLength(Result.Value, szLONG);
  MemMove(v, Result.Value[0], Result.Size);
end;

function imm32(v: Int32): TImmediate;
begin
  Result.Size := szLONG;
  SetLength(Result.Value, szLONG);
  MemMove(v, Result.Value[0], Result.Size);
end;


//Used to do this: mov (%eax), %ecx
//  A) _mov(ref(eax), eax);
function ref(Reg: TGPRegister): TMemVar;
begin
  Result.MemType := mtRegMem;
  Result.Reg  := Reg;
  Result.Size := Reg.Size;
  Result.Data := {$IFDEF LAPE}[]{$ELSE}nil{$ENDIF};
end;


// stack variable construction
operator + (reg:TGPRegister; offset:Int32): TMemVar;
begin
  Result.MemType := mtStack;
  Result.Reg  := Reg;
  Result.Size := szLONG;
  Result.Data := long_to_bytes(offset);
end;

operator - (reg:TGPRegister; offset:Int32): TMemVar;
begin
  Result.MemType := mtStack;
  Result.Reg  := Reg;
  Result.Size := szLONG;
  Result.Data := long_to_bytes(-offset);
end;

function mem(var x; Size:Byte=szLONG): TMemVar;
begin
  Result.MemType := mtPointer;
  Result.Reg  := EAX.Convert(Size);
  Result.Size := Size;
  Result.Data := addr_to_bytes(@x);
end;

function ptr(x: Pointer; Size:Byte=szLONG): TMemVar;
begin
  Result.MemType := mtPointer;
  Result.Reg  := EAX.Convert(Size);
  Result.Size := Size;
  Result.Data := addr_to_bytes(x);
end;

{$IFDEF LAPE}
operator is (memvar:TMemVar; Size:Byte): TMemVar;
begin
  Result := memvar;
  Result.Reg := Result.Reg.Convert(Size);
  Result.Size := Size;
end;
{$ENDIF}


// ---------------------------------------------------------------------------
// FPC helpers
{$IFDEF FPC}
operator + (Left:TBytes; Right:TBytes): TBytes;
begin
  SetLength(Result, Length(Left) + Length(Right));
  MemMove(Left[0],  Result[0],            Length(Left));
  MemMove(Right[0], Result[Length(Left)], Length(Right));
end;

operator + (Left:array of Byte; Right:TBytes): TBytes;
begin
  SetLength(Result, Length(Left) + Length(Right));
  MemMove(Left[0],  Result[0],            Length(Left));
  MemMove(Right[0], Result[Length(Left)], Length(Right));
end;

operator + (Left:TBytes; Right:array of Byte): TBytes;
begin
  SetLength(Result, Length(Left) + Length(Right));
  MemMove(Left[0],  Result[0],            Length(Left));
  MemMove(Right[0], Result[Length(Left)], Length(Right));
end;

//XXX
operator + (Left: TNativeCode; Right: TBytes): TNativeCode;
var tmp: Int32;
begin
  tmp := Length(Left);
  SetLength(Left, tmp+1);
  SetLength(Left[tmp], Length(Right));
  MemMove(Right[0], Left[tmp][0], Length(Right));
  Result := Left;
end;
{$ENDIF}


// ---------------------------------------------------------------------------
// lape helpers

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
