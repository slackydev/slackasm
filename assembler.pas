{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  An x86 assembler
  Note that FPC compatiblity is incomplete.
[-----------------------------------------------------------------------------}
{$ASSERTIONS ON}
{$IFDEF FPC}
unit assembler;
{$modeswitch advancedrecords}
{$MACRO ON}

interface

{$DEFINE MEMMOVE := MOVE}
{$ENDIF}

{$IFDEF FPC}
uses  SysUtils, Windows, Math;
{$ENDIF}

{$IFDEF LAPE}
  function VirtualAlloc(lpAddress:Pointer; dwSize:PtrUInt; flAllocationType:UInt32;flProtect:UInt32): Pointer; external 'VirtualAlloc@Kernel32.dll stdcall';
  function VirtualFree(lpAddress:Pointer; dwSize:PtrUInt; dwFreeType:UInt32): Boolean; external 'VirtualFree@Kernel32.dll stdcall';
  const LineEnding = #13#10;
{$ENDIF}

{$I interface.inc}

type
  TSlackASM = {$IFNDEF LAPE}class{$ELSE}record{$ENDIF}
    Code: TNativeCode;
    Labels: array of record name: string; id:Int32; end;

    {$IFNDEF LAPE}
      constructor Create();
      procedure Free();

      function Size: SizeInt;
      function Location: SizeInt;
      function LocationOf(groupId:Int32): SizeInt;
      function RelLoc(ALabel: Int32): Int32;
      procedure Emit(bytes: TBytes); overload;
      procedure Emit(bytes: TNativeCode); overload;
      procedure WriteDebug();
      function Finalize(): TExternalMethod;
    {$ENDIF}
  end;

const DT16: array[0..0] of Byte = {$IFDEF LAPE}[$66]{$ELSE}($66){$ENDIF};
const NOP:  array[0..0] of Byte = {$IFDEF LAPE}[$90]{$ELSE}($90){$ENDIF};

const FIRST_IDX   =  0;
const SECOND_IDX  =  1;

{$IFNDEF LAPE}

{$I if_headers.inc}

procedure FreeMethod(ptr: Pointer);
function GetIntSize(x: Int64; validSizes: TByteSet=[1,4]): Int32;
function GetUIntSize(x: UInt64; validSizes: TByteSet=[1,4]): Int32;

function addr_to_bytes(x: Pointer): TBytes;
function long_to_bytes(x: Int32): TBytes;
function word_to_bytes(x: Int16): TBytes;

function imm(v: Int32; Size:Byte=szLONG): TImmediate;
function imm8(v: Int32): TImmediate;
function imm16(v: Int32): TImmediate;
function imm32(v: Int32): TImmediate;

function ref(Reg: TGPRegister): TMemVar;
operator + (reg:TGPRegister; offset:Int32): TMemVar;
operator - (reg:TGPRegister; offset:Int32): TMemVar;
function mem(var x; Size:Byte=szLONG): TMemVar;
function ptr(x: Pointer; Size:Byte=szLONG): TMemVar;

operator + (Left:TBytes; Right:TBytes): TBytes;
operator + (Left:array of Byte; Right:TBytes): TBytes;
operator + (Left:TBytes; Right:array of Byte): TBytes;
operator + (Left: TNativeCode; Right: TBytes): TNativeCode;

function ToBytes(x: array of Byte): TBytes;

implementation
{$ENDIF}

{$i utils.inc}


// ============================================================================
// Executable memory class
{$IFNDEF LAPE}
constructor TSlackASM.Create();
begin
  //nothing atm
end;
{$ELSE}
function TSlackASM.Create(): TSlackASM; static;
begin
  Result := [];
end;
{$ENDIF}


procedure TSlackASM.Free();
begin
  SetLength(Self.Code, 0);
  {$IFNDEF LAPE}
  Self.Destroy;
  {$ENDIF}
end;


function TSlackASM.Size: SizeInt;
begin
  Result := Length(Self.Code);
end;

function TSlackASM.Location: SizeInt;
var i:Int32;
begin
  Result := 0;
  for i:=0 to High(Self.Code) do
    Result += Length(Self.Code[i]);
end;

function TSlackASM.LocationOf(GroupId: Int32): SizeInt;
var i:Int32;
begin
  Result := 0;
  if GroupId > High(Self.Code) then
    Exit(Self.Location);
  for i:=0 to GroupId do
    Result += Length(Self.Code[i]);
end;

// mainly used to jump to a label relative to "here"
function TSlackASM.RelLoc(ALabel: Int32): Int32;
begin
  Result := ALabel - Self.Location;
end;


// ----------------------------------------------------------------------------
// emit code
procedure TSlackASM.Emit(bytes: TBytes); overload;
var tmp: Int32;
begin
  tmp := Length(Self.Code);
  SetLength(Self.Code, tmp+1);
  SetLength(Self.Code[tmp], Length(bytes));
  MemMove(bytes[0], Self.Code[tmp][0], Length(bytes));
end;

procedure TSlackASM.Emit(bytes: TNativeCode); overload;
var i: Int32;
begin
  for i:=0 to High(bytes) do
    Emit(bytes[i]);
end;


// ----------------------------------------------------------------------------
// debugging
procedure TSlackASM.WriteDebug();
var i,j:Int32;
begin
  for i:=0 to High(Self.Code) do
  begin
    for j:=0 to High(self.Code[i]) do
      Write(IntToHex(Self.Code[i][j], 2), ' ');
    WriteLn();
  end;
end;


// ----------------------------------------------------------------------------
// moves the code to executable mem, returns it
function TSlackASM.Finalize(): TExternalMethod;
var
  i,j,c, nBytes:Int32;
  exec: PInt8;
begin
  nBytes := Trunc(2 ** (Floor(Logn(2, Self.Location+1)) + 1));       // should round to nearest page size..
  exec   := VirtualAlloc(nil, nBytes, $00002000 or $00001000, $40);  // but I don't think it's important for now
                                                                     // * GetSystemInfo
  c := 0;
  for i:=0 to High(code) do
    for j:=0 to High(code[i]) do
    begin
      (exec+c)^ := code[i][j];
      Inc(c);
    end;
  Result := TExternalMethod(exec);
end;



// ---------------------------------------------------------------------------
// TGPRegister
function TGPRegister.Convert(NewSize: Byte): TGPRegister; {$IFDEF LAPE}constref;{$ENDIF}
begin
  Result := EAX;
  case NewSize of
    szBYTE: Result := _AL;
    szWORD: Result := _AX;
    szLONG: Result := EAX;
  end;
  Result.gpReg := Self.gpReg;
end;

function TGPRegister.Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Offset = -1 then 
    Offset := $C0;
  
  opcode[OffsetIdx] += other.BaseOffset;
  case self.Size of
    szBYTE: Result :=        opcode + ToBytes([self.gpReg*8 + other.gpReg + Offset]);
    szWORD: Result := DT16 + opcode + ToBytes([self.gpReg*8 + other.gpReg + Offset]);
    szLONG: Result :=        opcode + ToBytes([self.gpReg*8 + other.gpReg + Offset]);
    else       Result := NOP;
  end;
end;


// ---------------------------------------------------------------------------
// TMemVar
function TMemVar.Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Offset = -1 then
    Offset := Ord(self.MemType);
  
  opcode[OffsetIdx] += self.Reg.BaseOffset;
  case other.Size of
    szBYTE: Result :=        opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    szWORD: Result := DT16 + opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    szLONG: Result :=        opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    else    Result := NOP;
  end;
  if (self.MemType = mtStack) and (Ord(self.reg.gpReg) = Ord(ESP.gpReg)) then
    Result += ToBytes([$24]);
  
  if Length(self.Data) > 0 then
    Result += self.Data;
end;

function TMemVar.EncodeR(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Offset = -1 then
    Offset := Ord(self.MemType);
  
  opcode[OffsetIdx] += other.BaseOffset;
  case other.Size of
    szBYTE: Result :=        opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    szWORD: Result := DT16 + opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    szLONG: Result :=        opcode + ToBytes([other.gpReg*8 + self.Reg.gpReg + Offset]);
    else    Result := NOP;
  end;
  if (self.MemType = mtStack) and (Ord(self.reg.gpReg) = Ord(ESP.gpReg)) then
    Result += ToBytes([$24]);
  
  if Length(self.Data) > 0 then
    Result += self.Data;
end;

function TMemVar.FPUEncode(opcode:array of Byte; Offset:Int16=0; Data16:Boolean=False): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
var BaseOffset: Byte;
begin
  BaseOffset := Ord(self.MemType);
  if Data16 then Result := DT16;
  
  Result += opcode + ToBytes([self.Reg.gpReg + Offset + BaseOffset]);
  
  if (self.MemType = mtStack) and (Ord(self.reg.gpReg) = Ord(ESP.gpReg)) then
    Result += ToBytes([$24]);
  
  if Length(self.Data) > 0 then
    Result += self.Data;
end;

function TMemVar.Offset(n:Int32): TMemVar; {$IFDEF LAPE}constref;{$ENDIF}
var tmp:Int32;
begin
  Assert(self.MemType <> mtRegMem, 'Illegal operand');
  Result := Self;
  Result.Data := Copy(Self.Data);
  MemMove(Result.Data[0], tmp, i32);
  tmp += n;
  MemMove(tmp, Result.Data[0], i32);
end;

function TMemVar.AsType(n:Byte): TMemVar; {$IFDEF LAPE}constref;{$ENDIF}
begin
  Result := Self;
  Result.Reg  := Result.Reg.Convert(n);
  Result.Size := n;
end;



// ---------------------------------------------------------------------------
// TImmediate
function TImmediate.Slice(n: Int8=-1): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if (n = -1) then
  begin
    SetLength(Result, Self.Size);
    MemMove(self.value[0], Result[0], self.Size);
  end else
  begin
    SetLength(Result, n);
    MemMove(self.value[0], Result[0], n);
  end;
end;

function TImmediate.Encode(opcode:array of Byte; Other: TGPRegister; Offset:Int16=-1; OffsetIdx:Byte=0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Offset=-1 then Offset := $C0;
  opcode[OffsetIdx] += other.BaseOffset;
  case Other.Size of
    szBYTE: Result :=        opcode + ToBytes([other.gpReg + Offset]) + self.Slice(Other.Size);
    szWORD: Result := DT16 + opcode + ToBytes([other.gpReg + Offset]) + self.Slice(Other.Size);
    szLONG: Result :=        opcode + ToBytes([other.gpReg + Offset]) + self.Slice(Other.Size);
    else    Result := NOP;
  end;
end;

function TImmediate.EncodeEx(opcode:array of Byte; Other1,Other2: TGPRegister; Offset:Int16=0; OffsetIdx:Byte=0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  opcode[OffsetIdx] += other1.BaseOffset;
  case Other1.Size of
    szBYTE: Result :=        opcode + ToBytes([other1.gpReg*8 + other2.gpReg + Offset]) + self.Slice(Other1.Size);
    szWORD: Result := DT16 + opcode + ToBytes([other1.gpReg*8 + other2.gpReg + Offset]) + self.Slice(Other1.Size);
    szLONG: Result :=        opcode + ToBytes([other1.gpReg*8 + other2.gpReg + Offset]) + self.Slice(Other1.Size);
    else    Result := NOP;
  end;
end;


function TImmediate.EncodeHC(r8,r16,r32:array of Byte; Other: TGPRegister): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Length(r8)  > 0 then r8 [High(r8) ] += other.gpReg;
  if Length(r16) > 0 then r16[High(r16)] += other.gpReg;
  if Length(r32) > 0 then r32[High(r32)] += other.gpReg;
  case Other.Size of
    szBYTE: Result :=        r8  + self.Slice(Other.Size);
    szWORD: Result := DT16 + r16 + self.Slice(Other.Size);
    szLONG: Result :=        r32 + self.Slice(Other.Size);
    else    Result := NOP;
  end;
end;



// ----------------------------------------------------------------------------
// include misc and core
{$I x86/misc.inc}

// ----------------------------------------------------------------------------
// handling the various flags that exists
{$I x86/flags.inc}

// ----------------------------------------------------------------------------
// include jump codes
{$I x86/jumpcodes.inc}

// ----------------------------------------------------------------------------
// working with general purpose registers
{$I x86/general_purpose.inc}

// ----------------------------------------------------------------------------
// working with the FPU (not SIMD instructions)
{$I x86/FPU.inc}


{$IFNDEF LAPE}
end.
{$ENDIF}
