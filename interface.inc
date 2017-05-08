{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)

  Some core types
[-----------------------------------------------------------------------------}
type
  TByteSet = set of Byte;

  TBytes = array of Byte;
  TNativeCode = array of TBytes;
  
  TExternalMethod = {$IFNDEF CodeInSight}{$IFDEF LAPE}external{$ENDIF}{$ENDIF} procedure();
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
    {$IFDEF FPC}
      function Convert(NewSize: Byte): TGPRegister;
      function Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
    {$ENDIF}
  end;
  
  TMemType = (mtRegMem=$00, mtPointer=$05, mtStack=$80); {mtStack=$40 for byte offset, but we don't support that}
  TMemVar = record
    MemType: TMemType;
    Reg:  TGPRegister;
    Size: Byte;
    Data: TBytes;
    
    {$IFNDEF LAPE}
      function Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
      function EncodeR(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
      function FPUEncode(opcode:array of Byte; Offset:Int16=0; Data16:Boolean=False): TBytes;
      function Offset(n:Int32): TMemVar;
      function AsType(n:Byte): TMemVar;
    {$ENDIF}
  end;
  
  // immediate value
  TImmediate = record
    Value:TBytes;
    Size: Byte;
    
    {$IFNDEF LAPE}
      function Slice(n: Int8=-1): TBytes;
      function Encode(opcode:array of Byte; Other: TGPRegister; Offset:Int16=-1; OffsetIdx:Byte=0): TBytes;
      function EncodeEx(opcode:array of Byte; Other1,Other2: TGPRegister; Offset:Int16=0; OffsetIdx:Byte=0): TBytes;
      function EncodeHC(r8,r16,r32:array of Byte; Other: TGPRegister): TBytes;
    {$ENDIF}
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
  _AL: TGPRegister = {$IFDEF LAPE}[gpAL, 0, 1];{$ELSE}(gpReg:gpAL; BaseOffset:0; Size:1);{$ENDIF}
  _CL: TGPRegister = {$IFDEF LAPE}[gpCL, 0, 1];{$ELSE}(gpReg:gpCL; BaseOffset:0; Size:1);{$ENDIF}
  _DL: TGPRegister = {$IFDEF LAPE}[gpDL, 0, 1];{$ELSE}(gpReg:gpDL; BaseOffset:0; Size:1);{$ENDIF}
  _BL: TGPRegister = {$IFDEF LAPE}[gpBL, 0, 1];{$ELSE}(gpReg:gpBL; BaseOffset:0; Size:1);{$ENDIF}
  _AH: TGPRegister = {$IFDEF LAPE}[gpAH, 0, 1];{$ELSE}(gpReg:gpAH; BaseOffset:0; Size:1);{$ENDIF}
  _CH: TGPRegister = {$IFDEF LAPE}[gpCH, 0, 1];{$ELSE}(gpReg:gpCH; BaseOffset:0; Size:1);{$ENDIF}
  _DH: TGPRegister = {$IFDEF LAPE}[gpDH, 0, 1];{$ELSE}(gpReg:gpDH; BaseOffset:0; Size:1);{$ENDIF}
  _BH: TGPRegister = {$IFDEF LAPE}[gpBH, 0, 1];{$ELSE}(gpReg:gpBH; BaseOffset:0; Size:1);{$ENDIF}

  _AX: TGPRegister = {$IFDEF LAPE}[gpAX, 1, 2];{$ELSE}(gpReg:gpAX; BaseOffset:1; Size:2);{$ENDIF}
  _CX: TGPRegister = {$IFDEF LAPE}[gpCX, 1, 2];{$ELSE}(gpReg:gpCX; BaseOffset:1; Size:2);{$ENDIF}
  _DX: TGPRegister = {$IFDEF LAPE}[gpDX, 1, 2];{$ELSE}(gpReg:gpDX; BaseOffset:1; Size:2);{$ENDIF}
  _BX: TGPRegister = {$IFDEF LAPE}[gpBX, 1, 2];{$ELSE}(gpReg:gpBX; BaseOffset:1; Size:2);{$ENDIF}
  _SP: TGPRegister = {$IFDEF LAPE}[gpSP, 1, 2];{$ELSE}(gpReg:gpSP; BaseOffset:1; Size:2);{$ENDIF}
  _BP: TGPRegister = {$IFDEF LAPE}[gpBP, 1, 2];{$ELSE}(gpReg:gpBP; BaseOffset:1; Size:2);{$ENDIF}
  _SI: TGPRegister = {$IFDEF LAPE}[gpSI, 1, 2];{$ELSE}(gpReg:gpSI; BaseOffset:1; Size:2);{$ENDIF}
  _DI: TGPRegister = {$IFDEF LAPE}[gpDI, 1, 2];{$ELSE}(gpReg:gpDI; BaseOffset:1; Size:2);{$ENDIF}

  EAX: TGPRegister = {$IFDEF LAPE}[gpEAX, 1, 4];{$ELSE}(gpReg:gpEAX; BaseOffset:1; Size:4);{$ENDIF}
  ECX: TGPRegister = {$IFDEF LAPE}[gpECX, 1, 4];{$ELSE}(gpReg:gpECX; BaseOffset:1; Size:4);{$ENDIF}
  EDX: TGPRegister = {$IFDEF LAPE}[gpEDX, 1, 4];{$ELSE}(gpReg:gpEDX; BaseOffset:1; Size:4);{$ENDIF}
  EBX: TGPRegister = {$IFDEF LAPE}[gpEBX, 1, 4];{$ELSE}(gpReg:gpEBX; BaseOffset:1; Size:4);{$ENDIF}
  ESP: TGPRegister = {$IFDEF LAPE}[gpESP, 1, 4];{$ELSE}(gpReg:gpESP; BaseOffset:1; Size:4);{$ENDIF}
  EBP: TGPRegister = {$IFDEF LAPE}[gpEBP, 1, 4];{$ELSE}(gpReg:gpEBP; BaseOffset:1; Size:4);{$ENDIF}
  ESI: TGPRegister = {$IFDEF LAPE}[gpESI, 1, 4];{$ELSE}(gpReg:gpESI; BaseOffset:1; Size:4);{$ENDIF}
  EDI: TGPRegister = {$IFDEF LAPE}[gpEDI, 1, 4];{$ELSE}(gpReg:gpEDI; BaseOffset:1; Size:4);{$ENDIF}
  
const
  szNATIVE = SizeOf(NativeInt); //xxx (x86 only tho)

  szQUAD = 8;
  szLONG = 4;
  szWORD = 2;
  szBYTE = 1;
  
  szSINGLE  = 4;
  szDOUBLE  = 8;
  szLDOUBLE = 10;

  i8  = szBYTE;
  i16 = szWORD;
  i32 = szLONG;
  i64 = szQUAD;

  f32 = szSINGLE;
  f64 = szDOUBLE;
  f80 = szLDOUBLE;
  

//use with _setc
type
  E_SETxx = (
    SETO  = $90, SETNO = $91, SETB  = $92, SETAE = $93, SETE  = $94, SETNE = $95, 
    SETBE = $96, SETA  = $97, SETS  = $98, SETNS = $99, SETPE = $9A, SETPO = $9B, 
    SETL  = $9C, SETGE = $9D, SETLE = $9E, SETG  = $9F
  );

const
  SETC   = SETB;
  SETNAE = SETB;
  SETNB  = SETAE;
  SETNC  = SETAE;
  SETNZ  = SETNE;
  SETNA  = SETBE;
  SETNBE = SETA;
  SETP   = SETPE;
  SETNP  = SETPO;
  SETNGE = SETL;
  SETNL  = SETGE;
  SETNG  = SETLE;
  SETNLE = SETG;

