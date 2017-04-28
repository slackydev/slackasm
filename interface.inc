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
      function Encode(opcode:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes;
      function Encode0F(opcode:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes;
      function EncodeHC(r8,r16,r32:array of Byte; other: TGPRegister; Offset:Byte=$C0): TBytes;
      function EncodeMem(opcode:array of Byte; other: Pointer; Offset:Byte=$05): TBytes;
      function EncodeMem0F(opcode:array of Byte; other: Pointer; otherSize:Int32; Offset:Byte=$05): TBytes;
    {$ENDIF}
  end;
  
  TPtrAtGPRegister = record
    Reg: TGPRegister;
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
  
  // immediate value
  TImmediate = record
    Value:TBytes;
    Size: Byte;
    
    {$IFDEF FPC}
      function Slice(n: Int8=-1): TBytes;
      function EncodeHC(r8,r16,r32:array of Byte; Other: TGPRegister): TBytes;
    {$ENDIF}
  end;


  //used for stack offsets
  TStackVar = record
    Reg: TGPRegister;
    Offset: TImmediate;

    {$IFDEF FPC}
      function Slice(n: Int8=-1): TBytes;
    {$ENDIF}
  end;



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
  QUAD_SIZE = 8;
  LONG_SIZE = 4;
  WORD_SIZE = 2;
  BYTE_SIZE = 1;
  PTR_SIZE  = SizeOf(NativeInt); //xxx - 32bit


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

