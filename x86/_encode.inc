{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Various functions that makes producing the machinecode a little simpler
[-----------------------------------------------------------------------------}

function _reg_reg(reg1, reg2: TGPRegister32): Byte; overload; begin Result := $C0 + ord(reg1)*8 + ord(reg2); end;
function _reg_reg(reg1, reg2: TGPRegister16): Byte; overload; begin Result := $C0 + ord(reg1)*8 + ord(reg2); end;
function _reg_reg(reg1, reg2: TGPRegister8): Byte;  overload; begin Result := $C0 + ord(reg1)*8 + ord(reg2); end;

function _mem_reg(reg: TGPRegister32): Byte; overload; begin Result := Ord(reg)*8 + $05; end;
function _mem_reg(reg: TGPRegister16): Byte; overload; begin Result := Ord(reg)*8 + $05; end;
function _mem_reg(reg: TGPRegister8): Byte;  overload; begin Result := Ord(reg)*8 + $05; end;

function _stk_reg(stk:TStackVar; reg: TGPRegister32): Byte; overload;
begin
  if (Low(Int8) <= stk.Offset) and (High(Int8) <= stk.Offset) then
    Result := Ord(reg)*8 + $44 + (Ord(stk.reg)-4)
  else
    Result := Ord(reg)*8 + $84 + (Ord(stk.reg)-4);
end;

function _stk_reg(stk:TStackVar; reg: TGPRegister16): Byte; overload;
begin
  if (Low(Int8) <= stk.Offset) and (High(Int8) <= stk.Offset) then
    Result := Ord(reg)*8 + $44 + (Ord(stk.reg)-4)
  else
    Result := Ord(reg)*8 + $84 + (Ord(stk.reg)-4);
end;

function _stk_reg(stk:TStackVar; reg: TGPRegister8): Byte; overload;
begin
  if (Low(Int8) <= stk.Offset) and (High(Int8) <= stk.Offset) then
    Result := Ord(reg)*8 + $44 + (Ord(stk.reg)-4)
  else
    Result := Ord(reg)*8 + $84 + (Ord(stk.reg)-4);
end;