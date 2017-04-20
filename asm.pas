{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  An x86 assembler
[-----------------------------------------------------------------------------}
{$IFDEF FPC}
unit assembler;
{$ENDIF}

{$IFDEF FPC}
interface;
{$ENDIF}

const
  LineEnding = #13#10;

{$IFDEF FPC}
uses 
  SysUtils, Windows;
{$ELSE}
function VirtualAlloc(lpAddress:Pointer; dwSize:PTRUINT; flAllocationType:DWORD;flProtect:DWORD):Pointer; external 'VirtualAlloc@Kernel32.dll stdcall';
function VirtualFree(lpAddress:Pointer; dwSize:PTRUINT; dwFreeType:DWORD):Boolean; external 'VirtualFree@Kernel32.dll stdcall';
{$ENDIF}


type
  TSlackASM = {$IFDEF FPC}class{$ELSE}record{$ENDIF}
    Exec: Pointer;
    Code: TByteArray;
    WriteDebug: Boolean;
    Debug: string;
    
    {$IFDEF FPC}
      constructor Create(PAGE_SIZE: Int32=4096; WriteDebug: Boolean=False);
      destructor Free(FreeExec: Boolean=True);

      function Location: SizeInt;
      function Rel(ALabel: Int32): Int32;

      procedure WriteBytes(bytes: array of Byte; Newline:Boolean = True);
      procedure WriteAddr(p: Pointer);
      procedure WriteInt(n: Int64; bytes: Byte; Raw:Boolean = False);

      function Finalize(): TExternalMethod;
      
      //writebytes.inc
      procedure WriteMemInstr4(opcode:array of Byte; mem: Pointer; reg: TGPRegister32);
      procedure WriteMemInstr2(opcode:array of Byte; mem: Pointer; reg: TGPRegister16);
      procedure WriteMemInstr1(opcode:array of Byte; mem: Pointer; reg: TGPRegister8);
      procedure WriteStkInstr4(opcode:array of Byte; stk: TStackVar; reg: TGPRegister32);
      procedure WriteStkInstr2(opcode:array of Byte; stk: TStackVar; reg: TGPRegister16);
      procedure WriteStkInstr1(opcode:array of Byte; stk: TStackVar; reg: TGPRegister8);
      
      //misc.inc
      procedure _ret();
      procedure _prologue();   
      procedure _epilogue();   
      procedure _addl(x,y: PInt32; store: TGPRegister32=EAX);
      procedure _subl(x,y: PInt32; store: TGPRegister32=EAX); 
      procedure _imull(x,y: PInt32; store: TGPRegister32=EAX); 
      
      //flags.inc
      procedure _cmc;
      procedure _clc;
      procedure _stc;
      procedure _cld;
      procedure _std;
      procedure _cli;
      procedure _sti;
      procedure _clts;
      procedure _lahf;
      procedure _sahf;
      
      //jumps.inc
      procedure _jmp(dst: TGPRegister32); 
      procedure _jmp(dst: Pointer); 
      procedure _jmp(rel8: Int8); 
      procedure _jmp(rel32: Int32); 
      procedure _encodeJcc(jcc:Byte; rel: Int32);
      procedure _jo(rel: Int32);  
      procedure _jno(rel: Int32); 
      procedure _jb(rel: Int32);  
      procedure _jc(rel: Int32);  
      procedure _jnae(rel: Int32);
      procedure _jae(rel: Int32); 
      procedure _jnb(rel: Int32); 
      procedure _jnc(rel: Int32); 
      procedure _je(rel: Int32);  
      procedure _jz(rel: Int32);  
      procedure _jne(rel: Int32); 
      procedure _jnz(rel: Int32); 
      procedure _jbe(rel: Int32); 
      procedure _jna(rel: Int32); 
      procedure _ja(rel: Int32);  
      procedure _jnbe(rel: Int32);
      procedure _js(rel: Int32);  
      procedure _jns(rel: Int32); 
      procedure _jp(rel: Int32);  
      procedure _jpe(rel: Int32); 
      procedure _jpo(rel: Int32); 
      procedure _jnp(rel: Int32); 
      procedure _jl(rel: Int32);  
      procedure _jnge(rel: Int32);
      procedure _jge(rel: Int32); 
      procedure _jnl(rel: Int32); 
      procedure _jle(rel: Int32); 
      procedure _jng(rel: Int32); 
      procedure _jg(rel: Int32);  
      procedure _jnle(rel: Int32);
      procedure _jecxz(rel: Int8);
      procedure _jcxz(rel: Int8);   

      //general purpose.inc
      procedure _mov(src, dst: TGPRegister32); 
      procedure _movl(src: TGPRegister32; dst: PInt32); 
      procedure _movl(reg: TGPRegister32; stv: TStackVar); 
      procedure _movl(src: PInt32; dst: TGPRegister32);   
      procedure _movl(stv: TStackVar; reg: TGPRegister32); 
      
      procedure _inc(reg: TGPRegister32); 
      procedure _incl(mem: PInt32); 
      procedure _dec(reg: TGPRegister32); 
      procedure _decl(mem: PInt32); 
      
      procedure _cdq();
      procedure _cltq();
      procedure _cmpb(src, dst: TGPRegister8);  
      procedure _cmpw(src, dst: TGPRegister16); 
      procedure _cmpl(src, dst: TGPRegister32); 
      procedure _cmpb(mem: PInt32; reg: TGPRegister8);  
      procedure _cmpw(mem: PInt16; reg: TGPRegister16); 
      procedure _cmpl(mem: PInt32; reg: TGPRegister32); 
      procedure _cmpb(stk: TStackVar; reg: TGPRegister8);  
      procedure _cmpw(stk: TStackVar; reg: TGPRegister16); 
      procedure _cmpl(stk: TStackVar; reg: TGPRegister32); 
      procedure _andb(src, dst: TGPRegister8);  
      procedure _andw(src, dst: TGPRegister16); 
      procedure _andl(src, dst: TGPRegister32); 
      procedure _andb(mem: PInt32; reg: TGPRegister8);  
      procedure _andw(mem: PInt16; reg: TGPRegister16); 
      procedure _andl(mem: PInt32; reg: TGPRegister32); 
      procedure _andb(stk: TStackVar; reg: TGPRegister8);  
      procedure _andw(stk: TStackVar; reg: TGPRegister16); 
      procedure _andl(stk: TStackVar; reg: TGPRegister32); 
      procedure _orb(src, dst: TGPRegister8);  
      procedure _orw(src, dst: TGPRegister16); 
      procedure _orl(src, dst: TGPRegister32); 
      procedure _orb(mem: PInt32; reg: TGPRegister8);  
      procedure _orw(mem: PInt16; reg: TGPRegister16); 
      procedure _orl(mem: PInt32; reg: TGPRegister32); 
      procedure _orb(stk: TStackVar; reg: TGPRegister8);  
      procedure _orw(stk: TStackVar; reg: TGPRegister16); 
      procedure _orl(stk: TStackVar; reg: TGPRegister32); 
      procedure _xorb(src, dst: TGPRegister8);  
      procedure _xorw(src, dst: TGPRegister16); 
      procedure _xorl(src, dst: TGPRegister32); 
      procedure _xorb(mem: PInt32; reg: TGPRegister8);  
      procedure _xorw(mem: PInt16; reg: TGPRegister16); 
      procedure _xorl(mem: PInt32; reg: TGPRegister32); 
      procedure _xorb(stk: TStackVar; reg: TGPRegister8);  
      procedure _xorw(stk: TStackVar; reg: TGPRegister16); 
      procedure _xorl(stk: TStackVar; reg: TGPRegister32); 
      procedure _addb(src, dst: TGPRegister8);  
      procedure _addw(src, dst: TGPRegister16); 
      procedure _addl(src, dst: TGPRegister32); 
      procedure _addb(mem: PInt32; reg: TGPRegister8);  
      procedure _addw(mem: PInt16; reg: TGPRegister16); 
      procedure _addl(mem: PInt32; reg: TGPRegister32); 
      procedure _addb(stk: TStackVar; reg: TGPRegister8);  
      procedure _addw(stk: TStackVar; reg: TGPRegister16); 
      procedure _addl(stk: TStackVar; reg: TGPRegister32); 
      procedure _adcb(src, dst: TGPRegister8);  
      procedure _adcw(src, dst: TGPRegister16); 
      procedure _adcl(src, dst: TGPRegister32); 
      procedure _adcb(mem: PInt32; reg: TGPRegister8);  
      procedure _adcw(mem: PInt16; reg: TGPRegister16); 
      procedure _adcl(mem: PInt32; reg: TGPRegister32); 
      procedure _adcb(stk: TStackVar; reg: TGPRegister8);  
      procedure _adcw(stk: TStackVar; reg: TGPRegister16); 
      procedure _adcl(stk: TStackVar; reg: TGPRegister32); 
      procedure _subb(src, dst: TGPRegister8);  
      procedure _subw(src, dst: TGPRegister16); 
      procedure _subl(src, dst: TGPRegister32); 
      procedure _subb(mem: PInt32; reg: TGPRegister8);  
      procedure _subw(mem: PInt16; reg: TGPRegister16); 
      procedure _subl(mem: PInt32; reg: TGPRegister32); 
      procedure _subb(stk: TStackVar; reg: TGPRegister8);  
      procedure _subw(stk: TStackVar; reg: TGPRegister16); 
      procedure _subl(stk: TStackVar; reg: TGPRegister32); 
      procedure _divb(reg: TGPRegister8); 
      procedure _divw(reg: TGPRegister16); 
      procedure _divl(reg: TGPRegister32); 
      procedure _divb(mem: PInt8); 
      procedure _divw(mem: PInt16); 
      procedure _divl(mem: PInt32); 
      procedure _idivb(reg: TGPRegister8); 
      procedure _idivw(reg: TGPRegister16); 
      procedure _idivl(reg: TGPRegister32); 
      procedure _idivb(mem: PInt8); 
      procedure _idivw(mem: PInt16); 
      procedure _idivl(mem: PInt32); 
      procedure _mulb(reg: TGPRegister8); 
      procedure _mulw(reg: TGPRegister16); 
      procedure _mull(reg: TGPRegister32); 
      procedure _mulb(mem: PInt8); 
      procedure _mulw(mem: PInt16); 
      procedure _mull(mem: PInt32); 
      procedure _imulb(reg: TGPRegister8); 
      procedure _imulw(reg: TGPRegister16); 
      procedure _imull(reg: TGPRegister32); 
      procedure _imulb(mem: PInt8); 
      procedure _imulw(mem: PInt16); 
      procedure _imull(mem: PInt32); 
      procedure _imulw(src, dst: TGPRegister16); 
      procedure _imull(src, dst: TGPRegister32); 
      procedure _imulw(mem: PInt16; reg: TGPRegister16); 
      procedure _imull(mem: PInt32; reg: TGPRegister32); 
      procedure _salb(reg: TGPRegister8); 
      procedure _salw(reg: TGPRegister16); 
      procedure _sall(reg: TGPRegister32); 
      procedure _salb(mem: PInt8); 
      procedure _salw(mem: PInt16); 
      procedure _sall(mem: PInt32); 
      procedure _sarb(reg: TGPRegister8); 
      procedure _sarw(reg: TGPRegister16); 
      procedure _sarl(reg: TGPRegister32); 
      procedure _sarb(mem: PInt8); 
      procedure _sarw(mem: PInt16); 
      procedure _sarl(mem: PInt32); 
      procedure _shlb(reg: TGPRegister8); 
      procedure _shlw(reg: TGPRegister16); 
      procedure _shll(reg: TGPRegister32); 
      procedure _shlb(mem: PInt8); 
      procedure _shlw(mem: PInt16); 
      procedure _shll(mem: PInt32); 
      procedure _shrb(reg: TGPRegister8); 
      procedure _shrw(reg: TGPRegister16); 
      procedure _shrl(reg: TGPRegister32); 
      procedure _shrb(mem: PInt8); 
      procedure _shrw(mem: PInt16); 
      procedure _shrl(mem: PInt32); 
      procedure _rolb(reg: TGPRegister8); 
      procedure _rolw(reg: TGPRegister16); 
      procedure _roll(reg: TGPRegister32); 
      procedure _rolb(mem: PInt8); 
      procedure _rolw(mem: PInt16); 
      procedure _roll(mem: PInt32); 
      procedure _rorb(reg: TGPRegister8); 
      procedure _rorw(reg: TGPRegister16); 
      procedure _rorl(reg: TGPRegister32); 
      procedure _rorb(mem: PInt8); 
      procedure _rorw(mem: PInt16); 
      procedure _rorl(mem: PInt32); 
      
      //FPU.inc
      procedure _fildw(src: PInt32);
      procedure _fildl(src: PInt32);
      procedure _fildq(src: PInt64);
      procedure _fld(reg: TFPURegister);
      procedure _flds(src: PSingle);
      procedure _fldl(src: PDouble);
      procedure _fistpw(dst: PInt16);
      procedure _fistpl(dst: PInt32);
      procedure _fistpq(dst: PInt64);
      procedure _fstp(reg: TFPURegister);
      procedure _fstps(dst: PSingle);
      procedure _fstpl(dst: PDouble);
      procedure _fistw(dst: PInt16);
      procedure _fistl(dst: PInt32);
      procedure _fst(reg: TFPURegister);
      procedure _fsts(dst: PSingle);
      procedure _fstl(dst: PDouble);
    {$ENDIF}
  end;

{$I types.inc}

  
{$IFDEF FPC}
procedure FreeMethod(ptr: Pointer);
operator + (offset:Int32; reg:TGPRegister32): TStackOffset;
operator - (offset:Int32; reg:TGPRegister32): TStackOffset;


implementation
{$ENDIF}

procedure FreeMethod(ptr: Pointer);
begin
  VirtualFree(ptr, 0, $8000);
end;

operator + (offset:Int32; reg:TGPRegister32): TStackVar;
begin
  Result.Reg := Reg;
  Result.Offset := Offset;
end;

operator - (offset:Int32; reg:TGPRegister32): TStackVar;
begin
  Result.Reg := Reg;
  Result.Offset := -Offset;
end;

function ToString(x: TStackVar): string; override;
var t:Int32;
begin
  Result := ToStr(x.Offset) + '%('+ToStr(x.Reg)+')';
end;

procedure TryRemoveNewline(var s: string);
begin
  if Copy(s, Length(s)-Length(LineEnding)+1, Length(LineEnding)) = LineEnding then
    SetLength(s, Length(s)-Length(LineEnding));
end;


// ----------------------------------------------------------------------------
// helper for common encoding
{$I x86/_encode.inc}



// ============================================================================
// Executable memory class

{$IFDEF FPC}
constructor TSlackASM.Create(PAGE_SIZE: Int32=4096; WriteDebug: Boolean=False);
{$ELSE}
function TSlackASM.Create(PAGE_SIZE: Int32=4096; WriteDebug: Boolean=False): TSlackASM; static;
{$ENDIF}
begin
  Result.Exec := VirtualAlloc(nil, PAGE_SIZE, $00002000 or $00001000, $40);
  Result.WriteDebug := WriteDebug;
end;

{$IFDEF FPC}
destructor TSlackASM.Free(FreeExec: Boolean=True);
{$ELSE}
procedure TSlackASM.Free(FreeExec: Boolean=True);
{$ENDIF}
begin
  if FreeExec then VirtualFree(Self.Exec, 0, $8000);
  SetLength(Code, 0);
  SetLength(Debug, 0);
  WriteDebug := False;
end;


// mainly used to create labels
function TSlackASM.Location: SizeInt;
begin
  Result := Length(Self.Code);
end;

// mainly used to jump to a label relative to "here"
function TSlackASM.Rel(ALabel: Int32): Int32;
begin
  Result := ALabel - Self.Location;
end;


// ----------------------------------------------------------------------------
// writers
procedure TSlackASM.WriteBytes(bytes: array of Byte; Newline:Boolean = True);
var i,top:Int32;
begin
  top := Length(Self.Code);
  SetLength(Self.Code, top + Length(bytes));
  for i:=0 to High(bytes) do Self.Code[top+i] := bytes[i];

  if Self.WriteDebug then
  begin
    for i:=0 to High(bytes) do
      Self.Debug += IntToHex(bytes[i], 2) + ' ';
    if Newline then
      Self.Debug += LineEnding;
  end;
end;


procedure TSlackASM.WriteAddr(p: Pointer);
var i,top,ip:PtrUInt;
begin
  ip := PtrUInt(p);
  top := Length(Self.Code);
  SetLength(Self.Code, SizeOf(Pointer)+top);
  for i:=0 to SizeOf(Pointer)-1 do
    Self.Code[top+i] := ip shr (i*8);

  if Self.WriteDebug then
  begin
    TryRemoveNewline(Self.Debug);
    Self.Debug += '$'+IntToHex(ip,1) +' '+ LineEnding;
  end;
end;


procedure TSlackASM.WriteInt(n: Int64; bytes: Byte; Raw:Boolean = False);
var top,i: PtrUInt;
begin
  top := Length(Self.Code);
  SetLength(Self.Code, bytes+top);
  for i:=0 to bytes-1 do
    Self.Code[top+i] := n shr (i*8);

  if Self.WriteDebug then
  begin
    TryRemoveNewline(Self.Debug);
    if not Raw then
      Self.Debug += IntToStr(n) +' '
    else
      for i:=0 to bytes-1 do
        Self.Debug += IntToHex(n shr (i*8) and $FF) + ' ';
    Self.Debug += LineEnding;
  end;
end;


// moves the code to executable mem, returns it
function TSlackASM.Finalize(): TExternalMethod;
begin
  MemMove(code[0], exec^, Length(code));
  Result := TExternalMethod(Self.Exec);
end;


// ----------------------------------------------------------------------------
// Some functions used to write certian instructions
{$I x86/writebytes.inc}

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
{$I x86/general purpose.inc}

// ----------------------------------------------------------------------------
// working with the FPU (not simd)
{$I x86/FPU.inc}


{$IFDEF FPC}
end.
{$ENDIF}
