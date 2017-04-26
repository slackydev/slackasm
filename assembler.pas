{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  An x86 assembler
[-----------------------------------------------------------------------------}
{$IFDEF FPC}
unit assembler;

interface

{$DEFINE MEMMOVE := MOVE}
{$ENDIF}

{$IFDEF FPC}
uses 
  SysUtils, Windows;
{$ELSE}
function VirtualAlloc(lpAddress:Pointer; dwSize:PTRUINT; flAllocationType:DWORD;flProtect:DWORD):Pointer; external 'VirtualAlloc@Kernel32.dll stdcall';
function VirtualFree(lpAddress:Pointer; dwSize:PTRUINT; dwFreeType:DWORD):Boolean; external 'VirtualFree@Kernel32.dll stdcall';
const LineEnding = #13#10;
{$ENDIF}

{$I types.inc}

type
  TSlackASM = {$IFDEF FPC}class{$ELSE}record{$ENDIF}
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
      function Emit(bytes: array of Byte): Int32;
      procedure WriteDebug();
      function Finalize(Reset: Boolean=False): TExternalMethod;

      //misc.inc
      procedure _ret();
      procedure _prologue();   
      procedure _epilogue();   
      procedure _addl(x,y: PInt32; store: TGPRegister32=EAX); overload;
      procedure _subl(x,y: PInt32; store: TGPRegister32=EAX); overload;
      procedure _imull(x,y: PInt32; store: TGPRegister32=EAX); overload;
      
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
      procedure _jmp(dst: TGPRegister32); overload;
      procedure _jmp(dst: Pointer); overload;
      procedure _jmp(rel8: Int8); overload;
      procedure _jmp(rel32: Int32); overload;
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
      procedure _mov(src, dst: TGPRegister32); overload; 
      procedure _movl(src: TGPRegister32; dst: PInt32); overload;
      procedure _movl(reg: TGPRegister32; stv: TStackVar); overload;
      procedure _movl(src: PInt32; dst: TGPRegister32); overload;  
      procedure _movl(stv: TStackVar; reg: TGPRegister32); overload;
      
      procedure _inc(reg: TGPRegister32); overload; 
      procedure _incl(mem: PInt32); overload; 
      procedure _dec(reg: TGPRegister32); overload; 
      procedure _decl(mem: PInt32); overload; 
      
      procedure _cdq(); overload;
      procedure _cltq(); overload;
      procedure _cmpb(src, dst: TGPRegister8); overload;  
      procedure _cmpw(src, dst: TGPRegister16); overload; 
      procedure _cmpl(src, dst: TGPRegister32); overload; 
      procedure _cmpb(mem: PInt32; reg: TGPRegister8); overload; 
      procedure _cmpw(mem: PInt16; reg: TGPRegister16); overload;
      procedure _cmpl(mem: PInt32; reg: TGPRegister32); overload;
      procedure _cmpb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _cmpw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _cmpl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _andb(src, dst: TGPRegister8); overload;  
      procedure _andw(src, dst: TGPRegister16); overload; 
      procedure _andl(src, dst: TGPRegister32); overload; 
      procedure _andb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _andw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _andl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _andb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _andw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _andl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _orb(src, dst: TGPRegister8); overload;  
      procedure _orw(src, dst: TGPRegister16); overload; 
      procedure _orl(src, dst: TGPRegister32); overload;
      procedure _orb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _orw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _orl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _orb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _orw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _orl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _xorb(src, dst: TGPRegister8); overload;  
      procedure _xorw(src, dst: TGPRegister16); overload; 
      procedure _xorl(src, dst: TGPRegister32); overload; 
      procedure _xorb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _xorw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _xorl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _xorb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _xorw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _xorl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _addb(src, dst: TGPRegister8); overload;  
      procedure _addw(src, dst: TGPRegister16); overload; 
      procedure _addl(src, dst: TGPRegister32); overload; 
      procedure _addb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _addw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _addl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _addb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _addw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _addl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _adcb(src, dst: TGPRegister8); overload;  
      procedure _adcw(src, dst: TGPRegister16); overload; 
      procedure _adcl(src, dst: TGPRegister32); overload; 
      procedure _adcb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _adcw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _adcl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _adcb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _adcw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _adcl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _subb(src, dst: TGPRegister8); overload;  
      procedure _subw(src, dst: TGPRegister16); overload; 
      procedure _subl(src, dst: TGPRegister32); overload; 
      procedure _subb(mem: PInt32; reg: TGPRegister8); overload;  
      procedure _subw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _subl(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _subb(stk: TStackVar; reg: TGPRegister8); overload;  
      procedure _subw(stk: TStackVar; reg: TGPRegister16); overload; 
      procedure _subl(stk: TStackVar; reg: TGPRegister32); overload; 
      procedure _divb(reg: TGPRegister8); overload; 
      procedure _divw(reg: TGPRegister16); overload; 
      procedure _divl(reg: TGPRegister32); overload; 
      procedure _divb(mem: PInt8); overload; 
      procedure _divw(mem: PInt16); overload; 
      procedure _divl(mem: PInt32); overload; 
      procedure _idivb(reg: TGPRegister8); overload; 
      procedure _idivw(reg: TGPRegister16); overload; 
      procedure _idivl(reg: TGPRegister32); overload; 
      procedure _idivb(mem: PInt8); overload; 
      procedure _idivw(mem: PInt16); overload;
      procedure _idivl(mem: PInt32); overload; 
      procedure _mulb(reg: TGPRegister8); overload; 
      procedure _mulw(reg: TGPRegister16); overload; 
      procedure _mull(reg: TGPRegister32); overload; 
      procedure _mulb(mem: PInt8); overload; 
      procedure _mulw(mem: PInt16); overload; 
      procedure _mull(mem: PInt32); overload; 
      procedure _imulb(reg: TGPRegister8); overload; 
      procedure _imulw(reg: TGPRegister16); overload; 
      procedure _imull(reg: TGPRegister32); overload; 
      procedure _imulb(mem: PInt8); overload; 
      procedure _imulw(mem: PInt16); overload; 
      procedure _imull(mem: PInt32); overload; 
      procedure _imulw(src, dst: TGPRegister16); overload; 
      procedure _imull(src, dst: TGPRegister32); overload; 
      procedure _imulw(mem: PInt16; reg: TGPRegister16); overload; 
      procedure _imull(mem: PInt32; reg: TGPRegister32); overload; 
      procedure _salb(reg: TGPRegister8); overload; 
      procedure _salw(reg: TGPRegister16); overload; 
      procedure _sall(reg: TGPRegister32); overload; 
      procedure _salb(mem: PInt8); overload; 
      procedure _salw(mem: PInt16); overload; 
      procedure _sall(mem: PInt32); overload; 
      procedure _sarb(reg: TGPRegister8); overload; 
      procedure _sarw(reg: TGPRegister16); overload; 
      procedure _sarl(reg: TGPRegister32); overload; 
      procedure _sarb(mem: PInt8); overload; 
      procedure _sarw(mem: PInt16); overload; 
      procedure _sarl(mem: PInt32); overload; 
      procedure _shlb(reg: TGPRegister8);  overload;
      procedure _shlw(reg: TGPRegister16);  overload;
      procedure _shll(reg: TGPRegister32); overload; 
      procedure _shlb(mem: PInt8); overload; 
      procedure _shlw(mem: PInt16);  overload;
      procedure _shll(mem: PInt32);  overload;
      procedure _shrb(reg: TGPRegister8); overload; 
      procedure _shrw(reg: TGPRegister16); overload; 
      procedure _shrl(reg: TGPRegister32); overload; 
      procedure _shrb(mem: PInt8);  overload;
      procedure _shrw(mem: PInt16);  overload;
      procedure _shrl(mem: PInt32); overload; 
      procedure _rolb(reg: TGPRegister8); overload; 
      procedure _rolw(reg: TGPRegister16); overload; 
      procedure _roll(reg: TGPRegister32); overload; 
      procedure _rolb(mem: PInt8); overload; 
      procedure _rolw(mem: PInt16); overload; 
      procedure _roll(mem: PInt32); overload; 
      procedure _rorb(reg: TGPRegister8); overload; 
      procedure _rorw(reg: TGPRegister16); overload; 
      procedure _rorl(reg: TGPRegister32); overload; 
      procedure _rorb(mem: PInt8); overload; 
      procedure _rorw(mem: PInt16); overload; 
      procedure _rorl(mem: PInt32); overload; 
      
      //FPU.inc
      procedure _fildw(src: PInt32); overload; 
      procedure _fildl(src: PInt32); overload; 
      procedure _fildq(src: PInt64); overload; 
      procedure _fld(reg: TFPURegister); overload; 
      procedure _flds(src: PSingle); overload; 
      procedure _fldl(src: PDouble); overload; 
      procedure _fistpw(dst: PInt16); overload; 
      procedure _fistpl(dst: PInt32); overload; 
      procedure _fistpq(dst: PInt64); overload; 
      procedure _fstp(reg: TFPURegister); overload; 
      procedure _fstps(dst: PSingle); overload; 
      procedure _fstpl(dst: PDouble); overload; 
      procedure _fistw(dst: PInt16); overload; 
      procedure _fistl(dst: PInt32); overload; 
      procedure _fst(reg: TFPURegister); overload; 
      procedure _fsts(dst: PSingle); overload; 
      procedure _fstl(dst: PDouble); overload; 

      procedure _fabs(); overload; 
      procedure _faddp(); overload; 
      procedure _fadds(mem: PSingle); overload; 
      procedure _faddl(mem: PDouble); overload; 
      procedure _fadds(stk: TStackVar); overload; 
      procedure _faddl(stk: TStackVar); overload; 
    {$ENDIF}
  end;

  
{$IFDEF FPC}
procedure FreeMethod(ptr: Pointer);

implementation
{$ENDIF}

procedure FreeMethod(ptr: Pointer);
begin
  VirtualFree(ptr, 0, $8000);
end;

{$i utils.inc}


// ----------------------------------------------------------------------------
// helper for common encoding
{$I x86/_encode.inc}


// ============================================================================
// Executable memory class

{$IFDEF FPC}
constructor TSlackASM.Create(PAGE_SIZE: Int32=4096);
begin
  ExecSize := PAGE_SIZE;
  Exec := VirtualAlloc(nil, ExecSize, $00002000 or $00001000, $40);
end;
{$ELSE}
function TSlackASM.Create(PAGE_SIZE: Int32=4096): TSlackASM; static;
begin
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
