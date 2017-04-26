{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)

  Some core types
[-----------------------------------------------------------------------------}
type
  TByteSet = set of Byte;

  TBytes = array of Byte;
  TNativeCode = array of TBytes;

  TExternalMethod = {$IFNDEF CodeInSight}{$IFNDEF FPC}external{$ENDIF}{$ENDIF} procedure();
  PInt64 = ^Int64;
  PInt32 = ^Int32;
  PInt16 = ^Int16;
  PInt8  = ^Int8;

  PExtended = ^Extended;
  PSingle   = ^Single;
  PDouble   = ^Double;


type
  TGPRegister = record
    gpReg, BaseOffset, Size: Byte;
  end;

  // FPU registers
  TFPURegister = (
    st0 = 0,
    st1 = 1,
    st2 = 2,
    st3 = 3,
    st4 = 4,
    st5 = 5,
    st6 = 6,
    st7 = 7
  );


const
  // general purpose registers
  gpEAX = 0;
  gpECX = 1;
  gpEDX = 2;
  gpEBX = 3;
  gpESP = 4;
  gpEBP = 5;
  gpESI = 6;
  gpEDI = 7;

  gpAX = 0;
  gpCX = 1;
  gpDX = 2;
  gpBX = 3;
  gpSP = 4;
  gpBP = 5;
  gpSI = 6;
  gpDI = 7;

  gpAL = 0;
  gpCL = 1;
  gpDL = 2;
  gpBL = 3;
  gpAH = 4;
  gpCH = 5;
  gpDH = 6;
  gpBH = 7;

  
const
  _AL: TGPRegister = [gpAL, 0, 1];
  _CL: TGPRegister = [gpCL, 0, 1];
  _DL: TGPRegister = [gpDL, 0, 1];
  _BL: TGPRegister = [gpBL, 0, 1];
  _AH: TGPRegister = [gpAH, 0, 1];
  _CH: TGPRegister = [gpCH, 0, 1];
  _DH: TGPRegister = [gpDH, 0, 1];
  _BH: TGPRegister = [gpBH, 0, 1];

  _AX: TGPRegister = [gpEAX, 1, 2];
  _CX: TGPRegister = [gpECX, 1, 2];
  _DX: TGPRegister = [gpEDX, 1, 2];
  _BX: TGPRegister = [gpEBX, 1, 2];
  _SP: TGPRegister = [gpESP, 1, 2];
  _BP: TGPRegister = [gpEBP, 1, 2];
  _SI: TGPRegister = [gpESI, 1, 2];
  _DI: TGPRegister = [gpEDI, 1, 2];

  EAX: TGPRegister = [gpEAX, 1, 4];
  ECX: TGPRegister = [gpECX, 1, 4];
  EDX: TGPRegister = [gpEDX, 1, 4];
  EBX: TGPRegister = [gpEBX, 1, 4];
  ESP: TGPRegister = [gpESP, 1, 4];
  EBP: TGPRegister = [gpEBP, 1, 4];
  ESI: TGPRegister = [gpESI, 1, 4];
  EDI: TGPRegister = [gpEDI, 1, 4];
  
const
  PTR_SIZE  = 4; //xxx
  LONG_SIZE = 4;
  WORD_SIZE = 2;
  BYTE_SIZE = 1;


  
{$IFDEF FPC}
  function ToBytes(x: array of Byte): TBytes;
  begin
    SetLength(Result, Length(x));
    MemMove(x[0], Result[0], Length(Result));
  end;
{$ENDIF}
{$IFDEF LAPE}
  type ToBytes = TBytes;
{$ENDIF}


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


function SizeToAReg(Size: Byte): TGPRegister;
begin
  if      Size = BYTE_SIZE then Result := _AL
  else if Size = WORD_SIZE then Result := _AX
  else if Size = LONG_SIZE then Result := EAX;
end;

function RegBase(opBase:TBytes; reg: TGPRegister; Offset: Int16=0): TBytes;
var i,j: Byte;
begin
  j := 0;
  if reg.Size = 2 then
  begin
    SetLength(Result, Length(opBase)+1);
    Result[0] := $66;
    j := 1;
  end else
    SetLength(Result, Length(opBase));

  Result[j] := opBase[0] + Offset + reg.BaseOffset;
  for i:=1 to High(opBase) do
    Result[i+j] := opBase[i];
end;

function RegBaseBySize(opBase:TBytes; Size: Byte; Offset: Int16=0): TBytes;
begin
  Result := RegBase(opBase, SizeToAReg(Size), Offset);
end;
