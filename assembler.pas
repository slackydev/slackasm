{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  An x86 assembler
  Note that FPC compatiblity is incomplete.
[-----------------------------------------------------------------------------}
{$IFDEF FPC}
unit assembler;
{$modeswitch advancedrecords}

interface

{$DEFINE MEMMOVE := MOVE}
{$ENDIF}

{$IFDEF FPC}
uses  SysUtils, Windows;
{$ENDIF}

{$IFDEF LAPE}
  function VirtualAlloc(lpAddress:Pointer; dwSize:PtrUInt; flAllocationType:UInt32;flProtect:UInt32): Pointer; external 'VirtualAlloc@Kernel32.dll stdcall';
  function VirtualFree(lpAddress:Pointer; dwSize:PtrUInt; dwFreeType:UInt32): Boolean; external 'VirtualFree@Kernel32.dll stdcall';
  {$IFNDECL LINEENDING}
  const LineEnding = #13#10;
  {$ENDIF}
{$ENDIF}

{$I interface.inc}

type
  TSlackASM = {$IFDEF FPC}class{$ENDIF}{$IFDEF LAPE}record{$ENDIF}
    ExecSize: SizeInt;
    Exec: Pointer;
    Code: TNativeCode;
    
    {$IFDEF FPC}
      constructor Create(PAGE_SIZE: Int32=4096);
      procedure Free(FreeExec: Boolean=True);

      function Len: SizeInt;
      function Location: SizeInt;
      function LocationOf(groupId:Int32): SizeInt;
      function RelLoc(ALabel: Int32): Int32;
      procedure Emit(bytes: TBytes); overload;
      procedure Emit(bytes: TNativeCode); overload;
      procedure WriteDebug();
      function Finalize(Reset: Boolean=False): TExternalMethod;
      
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
      function _jmp(dst: Pointer): TBytes; overload;
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
      function _mov(x,y: TGPRegister): TBytes; overload; 
      function _mov(x: TGPRegister; y: Pointer): TBytes; overload; 
      function _mov(x: Pointer; y: TGPRegister): TBytes; overload; 
      function _mov(x: TPtrAtGPRegister; y: TGPRegister): TBytes; overload; 
      function _mov(x: TGPRegister; y: TPtrAtGPRegister): TBytes; overload; 
      function _mov(x: TImmediate; y: TGPRegister): TBytes; overload; 
      function _movzx(x,y: TGPRegister): TBytes; overload; 
      function _movzx(x: Pointer; y: TGPRegister; MemSize:Byte=BYTE_SIZE): TBytes; overload;
      function _movsx(x,y: TGPRegister): TBytes; overload; 
      function _movsx(x: Pointer; y: TGPRegister; MemSize:Byte=BYTE_SIZE): TBytes; overload; 
      function _inc(x: TGPRegister): TBytes; overload;
      function _inc(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _dec(x: TGPRegister): TBytes; overload;
      function _dec(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _cdq(): TBytes;
      function _cltq(): TBytes;
      function _cmp(x, y: TGPRegister): TBytes; overload;
      function _cmp(x: Pointer; y: TGPRegister): TBytes; overload;
      function _test(x, y: TGPRegister): TBytes; overload;
      function _test(x: Pointer; y: TGPRegister): TBytes; overload;
      function _and(x, y: TGPRegister): TBytes; overload;
      function _and(x: Pointer; y: TGPRegister): TBytes; overload;
      function _or(x, y: TGPRegister): TBytes; overload;
      function _or(x: Pointer; y: TGPRegister): TBytes; overload;
      function _xor(x, y: TGPRegister): TBytes; overload;
      function _xor(x: Pointer; y: TGPRegister): TBytes; overload;
      function _add(x, y: TGPRegister): TBytes; overload;
      function _add(x: Pointer; y: TGPRegister): TBytes; overload;
      function _adc(x, y: TGPRegister): TBytes; overload;
      function _adc(x: Pointer; y: TGPRegister): TBytes; overload;
      function _sub(x, y: TGPRegister): TBytes; overload;
      function _sub(x: Pointer; y: TGPRegister): TBytes; overload;
      function _sbb(x, y: TGPRegister): TBytes; overload;
      function _sbb(x: Pointer; y: TGPRegister): TBytes; overload;
      function _div(x: TGPRegister): TBytes; overload;
      function _div(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _idiv(x: TGPRegister): TBytes; overload;
      function _idiv(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _mul(x: TGPRegister): TBytes; overload;
      function _mul(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _imul(x: TGPRegister): TBytes; overload;
      function _imul(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _imul(x, y: TGPRegister): TBytes; overload;
      function _imul(x: Pointer; y: TGPRegister): TBytes; overload;
      function _sal(x: TGPRegister): TBytes; overload;
      function _sal(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _sar(x: TGPRegister): TBytes; overload;
      function _sar(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _shl(x: TGPRegister): TBytes; overload;
      function _shl(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _shr(x: TGPRegister): TBytes; overload;
      function _shr(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _rol(x: TGPRegister): TBytes; overload;
      function _rol(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _ror(x: TGPRegister): TBytes; overload;
      function _ror(x: Pointer; MemSize: Byte=LONG_SIZE): TBytes; overload;
      function _setx(opcode:E_SETxx; dst: TGPRegister): TBytes; overload;
      function _setx(opcode:E_SETxx; dst: Pointer): TBytes; overload;
      
      //FPU.inc
      function _fildw(src: PInt16): TBytes; overload; 
      function _fildl(src: PInt32): TBytes; overload; 
      function _fildq(src: PInt64): TBytes; overload; 
      function _fld(src: TFPURegister): TBytes; overload; 
      function _flds(src: PSingle): TBytes; overload; 
      function _fldl(src: PDouble): TBytes; overload; 
      function _fistpw(dst: PInt16): TBytes; overload; 
      function _fistpl(dst: PInt32): TBytes; overload; 
      function _fistpq(dst: PInt64): TBytes; overload; 
      function _fstp(dst: TFPURegister): TBytes; overload; 
      function _fstps(dst: PSingle): TBytes; overload; 
      function _fstpl(dst: PDouble): TBytes; overload; 
      function _fistw(dst: PInt16): TBytes; overload; 
      function _fistl(dst: PInt32): TBytes; overload; 
      function _fst(dst: TFPURegister): TBytes; overload; 
      function _fsts(dst: PSingle): TBytes; overload; 
      function _fstl(dst: PDouble): TBytes; overload; 
      function _ffree(reg: TFPURegister): TBytes; 
      function _fadd(src, dst: TFPURegister): TBytes; overload; 
      function _fadds(mem: PSingle): TBytes; overload; 
      function _faddl(mem: PDouble): TBytes; overload; 
      function _faddp(): TBytes; overload; 
      function _faddp(src, dst: TFPURegister): TBytes; overload; 
      function _fiaddw(mem: PInt16): TBytes; overload; 
      function _fiaddl(mem: PInt32): TBytes; overload; 
      function _fsub(src, dst: TFPURegister): TBytes; overload; 
      function _fsubs(mem: PSingle): TBytes; overload; 
      function _fsubl(mem: PDouble): TBytes; overload; 
      function _fsubp(): TBytes; overload; 
      function _fsubp(src, dst: TFPURegister): TBytes; overload; 
      function _fisubw(mem: PInt16): TBytes; overload; 
      function _fisubl(mem: PInt32): TBytes; overload; 
      function _fsubr(src, dst: TFPURegister): TBytes; overload; 
      function _fsubrs(mem: PSingle): TBytes; overload; 
      function _fsubrl(mem: PDouble): TBytes; overload; 
      function _fsubrp(): TBytes; overload; 
      function _fsubrp(src, dst: TFPURegister): TBytes; overload; 
      function _fisubrw(mem: PInt16): TBytes; overload; 
      function _fisubrl(mem: PInt32): TBytes; overload; 
      function _fmul(src, dst: TFPURegister): TBytes; overload; 
      function _fmuls(mem: PSingle): TBytes; overload; 
      function _fmull(mem: PDouble): TBytes; overload; 
      function _fmulp(): TBytes; overload; 
      function _fmulp(src, dst: TFPURegister): TBytes; overload; 
      function _fimulw(mem: PInt16): TBytes; overload; 
      function _fimull(mem: PInt32): TBytes; overload; 
      function _fdiv(src, dst: TFPURegister): TBytes; overload; 
      function _fdivs(mem: PSingle): TBytes; overload; 
      function _fdivl(mem: PDouble): TBytes; overload; 
      function _fdivp(): TBytes; overload; 
      function _fdivp(src, dst: TFPURegister): TBytes; overload; 
      function _fidivw(mem: PInt16): TBytes; overload; 
      function _fidivl(mem: PInt32): TBytes; overload; 
      function _fdivr(src, dst: TFPURegister): TBytes; overload; 
      function _fdivrs(mem: PSingle): TBytes; overload; 
      function _fdivrl(mem: PDouble): TBytes; overload; 
      function _fdivrp(): TBytes; overload; 
      function _fdivrp(src, dst: TFPURegister): TBytes; overload; 
      function _fidivrw(mem: PInt16): TBytes; overload; 
      function _fidivrl(mem: PInt32): TBytes; overload; 
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

  
{$IFDEF FPC}
procedure FreeMethod(ptr: Pointer);
function ref(Left: TGPRegister): TPtrAtGPRegister;
function ref: Pointer; overload;
operator is (Left: TGPRegister; Right:Pointer): TPtrAtGPRegister;
function imm(v: Int32): TImmediate;
function imm8(v: Int8): TImmediate;
function imm16(v: Int16): TImmediate;
function imm32(v: Int32): TImmediate;

implementation
{$ENDIF}

procedure FreeMethod(ptr: Pointer);
begin
  VirtualFree(ptr, 0, $8000);
end;

{$i utils.inc}


// ============================================================================
// Executable memory class
{$IFDEF FPC}
constructor TSlackASM.Create(PAGE_SIZE: Int32=4096);
begin
  ExecSize := PAGE_SIZE;
  Exec := VirtualAlloc(nil, PAGE_SIZE, $00002000 or $00001000, $40);
end;
{$ENDIF}
{$IFDEF LAPE}
function TSlackASM.Create(PAGE_SIZE: Int32=4096): TSlackASM; static;
begin
  Result.ExecSize := PAGE_SIZE;
  Result.Exec := VirtualAlloc(nil, PAGE_SIZE, $00002000 or $00001000, $40);
end;
{$ENDIF}


procedure TSlackASM.Free(FreeExec: Boolean=True);
begin
  if FreeExec then VirtualFree(Self.Exec, 0, $8000);
  SetLength(Self.Code, 0);
  {$IFDEF FPC}
  Self.Destroy;
  {$ENDIF}
end;

function TSlackASM.Size: SizeInt;
begin
  Result := Length(Self.Code);
end;

// mainly used to create labels
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
function TSlackASM.Finalize(Reset: Boolean=False): TExternalMethod;
var i,j,c:Int32;
begin
  c := 0;
  for i:=0 to High(code) do
    for j:=0 to High(code[i]) do
    begin
      (PInt8(exec)+c)^ := code[i][j];
      Inc(c);
    end;
  Result := TExternalMethod(Self.Exec);
  
  if Reset then
  begin
    SetLength(Self.code, 0);
    Self.Exec := VirtualAlloc(nil, Self.ExecSize, $00002000 or $00001000, $40);
  end;
end;

{$IFDEF FPC}
function ToBytes(x: array of Byte): TBytes; 
begin
  SetLength(Result, Length(x));
  MemMove(x[0], Result[0], Length(Result));
end;
{$ENDIF}{$IFDEF LAPE}
type ToBytes = TBytes;
{$ENDIF}



// ---------------------------------------------------------------------------
// TGPRegister
function TGPRegister.Convert(Size: Byte): TGPRegister; {$IFDEF LAPE}constref;{$ENDIF}
begin
  Result := EAX;
  case Size of
    BYTE_SIZE: Result := _AL;
    WORD_SIZE: Result := _AX;
    LONG_SIZE: Result := EAX;      
  end;
  Result.gpReg := Self.gpReg;
end;

function TGPRegister.Encode(opcode:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  opcode[0] += other.BaseOffset;
  case other.Size of
    BYTE_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    WORD_SIZE: Result := [$66] + opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    LONG_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    else       Result := [$90]; //NOP
  end;
end;

function TGPRegister.Encode0F(opcode:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  opcode[1] += other.BaseOffset;
  case self.Size of
    BYTE_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    WORD_SIZE: Result := [$66] + opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    LONG_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    else       Result := [$90]; //NOP
  end;
end;

function TGPRegister.EncodeHC(r8,r16,r32:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  case other.Size of
    BYTE_SIZE: Result :=         r8  + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    WORD_SIZE: Result := [$66] + r16 + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    LONG_SIZE: Result :=         r32 + TBytes([self.gpReg*8 + other.gpReg + Offset]);
    else       Result := [$90]; //NOP
  end;
end;

function TGPRegister.EncodeMem(opcode:array of Byte; other: Pointer; Offset:Byte=$05): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  opcode[0] += self.BaseOffset;
  case self.Size of
    BYTE_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + Offset]);
    WORD_SIZE: Result := [$66] + opcode + TBytes([self.gpReg*8 + Offset]);
    LONG_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + Offset]);
    else       Result := [$90]; //NOP
  end;
  Result += addr_to_bytes(other);
end;

function TGPRegister.EncodeMem0F(opcode:array of Byte; other: Pointer; otherSize:Int32; Offset:Byte=$05): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  opcode[1] += EAX.Convert(otherSize).BaseOffset;
  case self.Size of
    BYTE_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + Offset]);
    WORD_SIZE: Result := [$66] + opcode + TBytes([self.gpReg*8 + Offset]);
    LONG_SIZE: Result :=         opcode + TBytes([self.gpReg*8 + Offset]);
    else       Result := [$90]; //NOP
  end;
  Result += addr_to_bytes(other);
end;


// ---------------------------------------------------------------------------
// TImmediate
function TImmediate.Slice(n: Int8=-1): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if (n = -1) then
  begin
    SetLength(Result, Self.size);
    MemMove(self.value[0], Result[0], self.size);
  end else
  begin
    SetLength(Result, n);
    MemMove(self.value[0], Result[0], n);
  end;
end;

function TImmediate.EncodeHC(r8,r16,r32:array of Byte; Other: TGPRegister): TBytes; {$IFDEF LAPE}constref;{$ENDIF}
begin
  if Length(r8)  > 0 then r8[0]  += other.gpReg;
  if Length(r16) > 0 then r16[0] += other.gpReg;
  if Length(r32) > 0 then r32[0] += other.gpReg;
  case Other.size of
    BYTE_SIZE: Result :=         r8  + self.Slice(Other.size);
    WORD_SIZE: Result := [$66] + r16 + self.Slice(Other.size);
    LONG_SIZE: Result :=         r32 + self.Slice(Other.size);
    else       Result := [$90]; //NOP
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


{$IFDEF FPC}
end.
{$ENDIF}
