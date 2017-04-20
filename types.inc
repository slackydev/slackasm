{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Some core types
[-----------------------------------------------------------------------------}
type
  TExternalMethod = {$IFNDEF CodeInSight}{$IFNDEF FPC}external{$ENDIF}{$ENDIF} procedure();
  PInt64 = ^Int64;
  PInt32 = ^Int32;
  PInt16 = ^Int16;
  PInt8  = ^Int8;
  
  PExtended = ^Extended;
  PSingle   = ^Single;
  PDouble   = ^Double;
  
  
  // general purpose registers
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

  TStackVar = record
    Reg: TGPRegister32;
    Offset: Int32;
  end;
