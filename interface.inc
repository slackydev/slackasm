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
    {$IFDEF FPC}
      function Convert(Size: Byte): TGPRegister;
      function Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
      function EncodeMem(opcode:array of Byte; other: TMemVar; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
    {$ENDIF}
  end;
  
  TMemType = (mtRegMem=$00, mtPointer=$05, mtStack=$80); {mtStack=$40 for byte offset, but we don't support that}
  TMemVar = record
    MemType: TMemType;
    Reg:  TGPRegister;
    Size: Byte;
    Data: TBytes;
    
    {$IFDEF FPC}
      function Encode(opcode:array of Byte; other: TGPRegister; Offset:Int16=-1; OffsetIdx:Int8=0): TBytes;
    {$ENDIF}
  end;
  
  // immediate value
  TImmediate = record
    Value:TBytes;
    Size: Byte;
    
    {$IFDEF FPC}
      function Slice(n: Int8=-1): TBytes;
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
  _AL: TGPRegister = [gpAL, 0, 1];
  _CL: TGPRegister = [gpCL, 0, 1];
  _DL: TGPRegister = [gpDL, 0, 1];
  _BL: TGPRegister = [gpBL, 0, 1];
  _AH: TGPRegister = [gpAH, 0, 1];
  _CH: TGPRegister = [gpCH, 0, 1];
  _DH: TGPRegister = [gpDH, 0, 1];
  _BH: TGPRegister = [gpBH, 0, 1];

  _AX: TGPRegister = [gpAX, 1, 2];
  _CX: TGPRegister = [gpCX, 1, 2];
  _DX: TGPRegister = [gpDX, 1, 2];
  _BX: TGPRegister = [gpBX, 1, 2];
  _SP: TGPRegister = [gpSP, 1, 2];
  _BP: TGPRegister = [gpBP, 1, 2];
  _SI: TGPRegister = [gpSI, 1, 2];
  _DI: TGPRegister = [gpDI, 1, 2];

  EAX: TGPRegister = [gpEAX, 1, 4];
  ECX: TGPRegister = [gpECX, 1, 4];
  EDX: TGPRegister = [gpEDX, 1, 4];
  EBX: TGPRegister = [gpEBX, 1, 4];
  ESP: TGPRegister = [gpESP, 1, 4];
  EBP: TGPRegister = [gpEBP, 1, 4];
  ESI: TGPRegister = [gpESI, 1, 4];
  EDI: TGPRegister = [gpEDI, 1, 4];
  
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

