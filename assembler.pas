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
    //ExecSize: SizeInt;
    //Exec: Pointer;
    Code: TNativeCode;
    
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
      
      //misc.inc
      function _ret: TBytes;
      function _nop: TBytes;
      
      //flags.inc
      function _cmc: TBytes;
      function _clc: TBytes;
      function _stc: TBytes;
      function _cld: TBytes;
      function _std: TBytes;
      function _cli: TBytes;
      function _sti: TBytes;
      function _clts: TBytes;
      function _lahf: TBytes;
      function _sahf: TBytes;
      
      //jumps.inc
      function _jmp(dst: TGPRegister): TBytes; overload;
      function _jmp(dst: TMemVar): TBytes; overload;
      function _jmp(rel8: Int8): TBytes; overload;
      function _jmp(rel32: Int32): TBytes; overload;
      function _encodeJcc(jcc:Byte; rel: Int32): TBytes;
      function _jo(rel: Int32): TBytes; 
      function _jno(rel: Int32): TBytes;
      function _jb(rel: Int32): TBytes; 
      function _jc(rel: Int32): TBytes; 
      function _jnae(rel: Int32): TBytes;
      function _jae(rel: Int32): TBytes;
      function _jnb(rel: Int32): TBytes;
      function _jnc(rel: Int32): TBytes;
      function _je(rel: Int32): TBytes; 
      function _jz(rel: Int32): TBytes; 
      function _jne(rel: Int32): TBytes;
      function _jnz(rel: Int32): TBytes;
      function _jbe(rel: Int32): TBytes;
      function _jna(rel: Int32): TBytes;
      function _ja(rel: Int32): TBytes; 
      function _jnbe(rel: Int32): TBytes;
      function _js(rel: Int32): TBytes; 
      function _jns(rel: Int32): TBytes;
      function _jp(rel: Int32): TBytes; 
      function _jpe(rel: Int32): TBytes;
      function _jpo(rel: Int32): TBytes;
      function _jnp(rel: Int32): TBytes;
      function _jl(rel: Int32): TBytes; 
      function _jnge(rel: Int32): TBytes;
      function _jge(rel: Int32): TBytes;
      function _jnl(rel: Int32): TBytes;
      function _jle(rel: Int32): TBytes;
      function _jng(rel: Int32): TBytes;
      function _jg(rel: Int32): TBytes; 
      function _jnle(rel: Int32): TBytes;
      function _jecxz(rel8: Int8): TBytes;
      function _jcxz(rel8: Int8): TBytes;
      
      //general_purpose.inc
      function _push(x: TGPRegister): TBytes; overload; 
      function _push(x: TMemVar): TBytes; overload; 
      function _push(x: TImmediate): TBytes; overload;
      function _pop(x: TGPRegister): TBytes; overload;
      function _pop(x: TMemVar): TBytes; overload;
      function _mov(x,y: TGPRegister): TBytes; overload; 
      function _mov(x: TMemVar; y: TGPRegister): TBytes; overload; 
      function _mov(x: TGPRegister; y: TMemVar): TBytes; overload; 
      function _mov(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _movzx(x,y: TGPRegister): TBytes; overload; 
      function _movzx(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _movsx(x,y: TGPRegister): TBytes; overload; 
      function _movsx(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _lea(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _inc(x: TGPRegister): TBytes; overload;
      function _inc(x: TMemVar): TBytes; overload;
      function _dec(x: TGPRegister): TBytes; overload;
      function _dec(x: TMemVar): TBytes; overload;
      function _cdq(): TBytes;
      function _cltq(): TBytes;
      function _test(x, y: TGPRegister): TBytes; overload;
      function _test(x: TGPRegister; y: TMemVar): TBytes; overload; 
      function _test(x: TMemVar; y: TGPRegister): TBytes; overload; 
      function _test(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _cmp(x,y: TGPRegister): TBytes; overload;
      function _cmp(x: TGPRegister; y: TMemVar): TBytes; overload; 
      function _cmp(x: TMemVar; y: TGPRegister): TBytes; overload; 
      function _cmp(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _and(x, y: TGPRegister): TBytes; overload;
      function _and(x: TGPRegister; y: TMemVar): TBytes; overload; 
      function _and(x: TMemVar; y: TGPRegister): TBytes; overload; 
      function _and(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _or(x, y: TGPRegister): TBytes; overload;
      function _or(x: TGPRegister; y: TMemVar): TBytes; overload;
      function _or(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _or(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _xor(x, y: TGPRegister): TBytes; overload;
      function _xor(x: TGPRegister; y: TMemVar): TBytes; overload;
      function _xor(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _xor(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _add(x, y: TGPRegister): TBytes; overload;
      function _add(x: TGPRegister; y: TMemVar): TBytes; overload; 
      function _add(x: TMemVar; y: TGPRegister): TBytes; overload; 
      function _add(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _adc(x, y: TGPRegister): TBytes; overload;
      function _adc(x: TGPRegister; y: TMemVar): TBytes; overload;
      function _adc(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _adc(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _sub(x, y: TGPRegister): TBytes; overload;
      function _sub(x: TGPRegister; y: TMemVar): TBytes; overload;
      function _sub(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _sub(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _sbb(x, y: TGPRegister): TBytes; overload;
      function _sbb(x: TGPRegister; y: TMemVar): TBytes; overload;
      function _sbb(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _sbb(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _div(x: TGPRegister): TBytes; overload;
      function _div(x: TMemVar): TBytes; overload;
      function _idiv(x: TGPRegister): TBytes; overload;
      function _idiv(x: TMemVar): TBytes; overload;
      function _mul(x: TGPRegister): TBytes; overload;
      function _mul(x: TMemVar): TBytes; overload;
      function _imul(x: TGPRegister): TBytes; overload;
      function _imul(x: TMemVar): TBytes; overload;
      function _imul(x,y: TGPRegister): TBytes; overload;
      function _imul(x: TMemVar; y: TGPRegister): TBytes; overload;
      function _imul(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _sal(x: TGPRegister): TBytes; overload;
      function _sal(x: TMemVar): TBytes; overload;
      function _sar(x: TGPRegister): TBytes; overload;
      function _sar(x: TMemVar): TBytes; overload;
      function _shl(x: TGPRegister): TBytes; overload;
      function _shl(x: TMemVar): TBytes; overload;
      function _shr(x: TGPRegister): TBytes; overload;
      function _shr(x: TMemVar): TBytes; overload;
      function _rol(x: TGPRegister): TBytes; overload;
      function _rol(x: TMemVar): TBytes; overload;
      function _ror(x: TGPRegister): TBytes; overload;
      function _ror(x: TMemVar): TBytes; overload;
      function _setc(opcode:E_SETxx; dst: TGPRegister): TBytes; overload;
      function _setc(opcode:E_SETxx; dst: TMemVar): TBytes; overload;
      
      //FPU.inc
      function _fld(src: TFPURegister): TBytes; overload; 
      function _fld(src: TMemVar): TBytes; overload;
      function _fild(src: TMemVar): TBytes; overload;
      function _fst(dst: TFPURegister): TBytes; overload; 
      function _fst(dst: TMemVar): TBytes; overload;
      function _fist(dst: TMemVar): TBytes;
      function _fstp(dst: TFPURegister): TBytes; overload; 
      function _fstp(dst: TMemVar): TBytes; overload;
      function _fistp(dst: TMemVar): TBytes;
      function _fisttp(dst: TMemVar): TBytes;
      function _ffree(reg: TFPURegister): TBytes; 
      function _faddp(dst: TFPURegister=st1): TBytes; overload; 
      function _fadd(src, dst: TFPURegister): TBytes; overload; 
      function _fadd(mem: TMemVar): TBytes; overload;
      function _fiadd(mem: TMemVar): TBytes; 
      function _fsubp(dst: TFPURegister=st1): TBytes; overload; 
      function _fsub(src, dst: TFPURegister): TBytes; overload; 
      function _fsub(mem: TMemVar): TBytes; overload;
      function _fisub(mem: TMemVar): TBytes; 
      function _fsubrp(dst: TFPURegister=st1): TBytes; overload; 
      function _fsubr(src, dst: TFPURegister): TBytes; overload; 
      function _fsubr(mem: TMemVar): TBytes; overload;
      function _fisubr(mem: TMemVar): TBytes; 
      function _fmulp(dst: TFPURegister=st1): TBytes; overload; 
      function _fmul(src, dst: TFPURegister): TBytes; overload; 
      function _fmul(mem: TMemVar): TBytes; overload;
      function _fimul(mem: TMemVar): TBytes; 
      function _fdivp(dst: TFPURegister=st1): TBytes; overload; 
      function _fdiv(src, dst: TFPURegister): TBytes; overload; 
      function _fdiv(mem: TMemVar): TBytes; overload;
      function _fidiv(mem: TMemVar): TBytes; 
      function _fdivrp(dst: TFPURegister=st1): TBytes; overload; 
      function _fdivr(src, dst: TFPURegister): TBytes; overload; 
      function _fdivr(mem: TMemVar): TBytes; overload;
      function _fidivr(mem: TMemVar): TBytes; 
      function _ftst(): TBytes; 
      function _fxam(): TBytes; 
      function _fabs(): TBytes;
      function _frndint(): TBytes;
      function _fsqrt(): TBytes;
      function _fcos(): TBytes;
      function _fsin(): TBytes;
      function _fsincos(): TBytes;
      function _fptan(): TBytes;
      function _fpatan(): TBytes;
      function _fprem(): TBytes;
      function _fprem1(): TBytes;
      function _fscale(): TBytes;
      function _fxtract(): TBytes;
      function _fyl2x(): TBytes;
      function _fyl2xp1(): TBytes;
    {$ENDIF}
  end;

const DT16: array[0..0] of Byte = {$IFDEF LAPE}[$66]{$ELSE}($66){$ENDIF};
const NOP:  array[0..0] of Byte = {$IFDEF LAPE}[$90]{$ELSE}($90){$ENDIF};

const FIRST_IDX   =  0;
const SECOND_IDX  =  1;

{$IFNDEF LAPE}
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
//operator is (memvar:TMemVar; Size:Byte): TMemVar;  {FPC can't -.-}

operator + (Left:TBytes; Right:TBytes): TBytes;
operator + (Left:array of Byte; Right:TBytes): TBytes;
operator + (Left:TBytes; Right:array of Byte): TBytes;
operator + (Left: TNativeCode; Right: TBytes): TNativeCode;

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


{$IFNDEF LAPE}
function ToBytes(x: array of Byte): TBytes; 
begin
  SetLength(Result, Length(x));
  MemMove(x[0], Result[0], Length(Result));
end;
{$ELSE}
type ToBytes = TBytes;
{$ENDIF}



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
