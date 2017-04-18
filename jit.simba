const
  LineEnding = #13#10;

function VirtualAlloc(lpAddress:Pointer; dwSize:PTRUINT; flAllocationType:DWORD;flProtect:DWORD):Pointer; external 'VirtualAlloc@Kernel32.dll stdcall';
function VirtualFree(lpAddress:Pointer; dwSize:PTRUINT; dwFreeType:DWORD):Boolean; external 'VirtualFree@Kernel32.dll stdcall';

type
  TExternalMethod = external procedure();
  PInt32 = ^Int32;
  PInt16 = ^Int16;
  PInt8  = ^Int8;

  TGPRegister32 = (
    EAX = 0,
    ECX = 1,
    EDX = 2,
    EBX = 3,
    ESP = 4,
    EBP = 5,
    ESI = 6,
    EDI = 7
  );

  TGPRegister16 = (
    _AX = 0,
    _CX = 1,
    _DX = 2,
    _BX = 3
  );

  TGPRegister8 = (
    _AL = 0,
    _CL = 1,
    _DL = 2,
    _BL = 3,
    _AH = 4,
    _CH = 5,
    _DH = 6,
    _BH = 7
  );


  //---------------------------------------
  TSlackASM = record
    Exec: Pointer;
    Code: TByteArray;
    Debug: string;
  end;


procedure FreeMethod(ptr: Pointer);
begin
  VirtualFree(ptr, 0, $8000);
end;


// ----------------------------------------------------------------------------
// Executable memory

function TSlackASM.Create(PAGE_SIZE: Int32=4096): TSlackASM; static;
begin
  Result.Exec := VirtualAlloc(nil, PAGE_SIZE, $00002000 or $00001000, $40);
end;

procedure TSlackASM.Free(freeExec: Boolean=True);
begin
  if freeExec then
    VirtualFree(Self.Exec, 0, $8000);
  SetLength(Code, 0);
  SetLength(Debug, 0);
end;

function TSlackASM.Location: SizeInt;
begin
  Result := Length(Self.Code);
end;

function TSlackASM.Rel(ALabel: Int32): Int32;
begin
  Result := ALabel - Self.Location;
end;


// ----------------------------------------------------------------------------
// writers
procedure TSlackASM.WriteBytes(bytes: array of Byte);
var i,top:Int32;
begin
  top := Length(Self.Code);
  SetLength(Self.Code, top + Length(bytes));
  for i:=0 to High(bytes) do Self.Code[top+i] := bytes[i];

  {$IFDEF DEBUG}
  Self.Debug += '>>> ';
  for i:=0 to High(bytes) do
    Self.Debug += IntToHex(bytes[i], 2) + ' ';
  Self.Debug += LineEnding;
  {$ENDIF}
end;


procedure TSlackASM.WriteAddr(p: Pointer);
var i,top,ip:PtrUInt;
begin
  ip := PtrUInt(p);
  top := Length(Self.Code);
  SetLength(Self.Code, SizeOf(Pointer)+top);
  for i:=0 to SizeOf(Pointer)-1 do
    Self.Code[top+i] := ip shr (i*8);

  {$IFDEF DEBUG}
  SetLength(Self.Debug, Length(Self.Debug)-Length(LineEnding));
  Self.Debug += '$'+IntToHex(ip,1) +' '+ LineEnding;
  {$ENDIF}
end;


procedure TSlackASM.WriteInt(n: Int64; bytes: Byte);
var top,i: PtrUInt;
begin
  top := Length(Self.Code);
  SetLength(Self.Code, bytes+top);
  for i:=0 to bytes-1 do
    Self.Code[top+i] := n shr (i*8);

  {$IFDEF DEBUG}
  SetLength(Self.Debug, Length(Self.Debug)-Length(LineEnding));
  Self.Debug += IntToStr(n) +' '+ LineEnding;
  {$ENDIF}
end;


function TSlackASM.Finalize(): TExternalMethod;
begin
  Self.WriteBytes([$C3]);
  MemMove(code[0], exec^, Length(code));
  Result := TExternalMethod(Self.Exec);
end;


procedure TSlackASM.Prologue();
begin
  Self.WriteBytes([$55]);         //  push rbp
  Self.WriteBytes([$48,$89,$e5]); //  mov  rbp, rsp
end;


procedure TSlackASM.Cleanup();
begin
  Self.WriteBytes([$5d]);         // pop rbp  | restore old pointer
end;


// ----------------------------------------------------------------------------
// include misc and core
{$I x86/misc.inc}

// ----------------------------------------------------------------------------
// include jump codes
{$I x86/jumpcodes.inc}

// ----------------------------------------------------------------------------
// raw arithmetic intrinsics
{$I x86/arithmetics.inc}
