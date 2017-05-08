{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Jump instructions
[-----------------------------------------------------------------------------}
{$RANGECHECKS OFF}

// ----------------------------------------------------------------------------
// JMP - Unconditional jump
function _jmp(dst: TGPRegister): TBytes; overload;
begin
  Result := ToBytes([$FF, dst.gpReg]);
end;

function _jmp(dst: TMemVar): TBytes; overload;
begin
  Assert(dst.Size = 4, 'Illegal operand size');
  Result := dst.Encode([$FF], EAX, $20+Ord(dst.MemType));
end;

// JMP - Relative Position
function _jmp(rel8: Int8): TBytes; overload;
begin
  if rel8 < 0 then rel8 -= 2; //remove self
  Result := ToBytes([$EB, rel8]);
end;

function _jmp(rel32: Int32): TBytes; overload;
begin
  if rel32 < 0 then rel32 -= 5; //remove self
  Result := ToBytes([$E9]) + long_to_bytes(rel32);
end;


// ----------------------------------------------------------------------------
// Jump if Condition Is Met
function _encodeJcc(jcc:Byte; rel: Int32): TBytes;
begin
  if GetIntSize(rel) = 1 then
  begin
    if rel < 0 then rel -= 1+1; //remove self
    Result := ToBytes([jcc, rel]);
  end else
  begin
    if rel < 0 then rel -= 4+2; //remove self
    Result := ToBytes([$0F, jcc+$10]) + long_to_bytes(rel);
  end;
end;

function _jo(rel: Int32): TBytes;  begin Result := _encodeJcc($70, rel); end;
function _jno(rel: Int32): TBytes; begin Result := _encodeJcc($71, rel); end;
function _jb(rel: Int32): TBytes;  begin Result := _encodeJcc($72, rel); end;
function _jc(rel: Int32): TBytes;  begin Result := _jb(rel);             end;
function _jnae(rel: Int32): TBytes;begin Result := _jb(rel);             end;
function _jae(rel: Int32): TBytes; begin Result := _encodeJcc($73, rel); end;
function _jnb(rel: Int32): TBytes; begin Result := _jae(rel);            end;
function _jnc(rel: Int32): TBytes; begin Result := _jae(rel);            end;
function _je(rel: Int32): TBytes;  begin Result := _encodeJcc($74, rel); end;
function _jz(rel: Int32): TBytes;  begin Result := _je(rel);             end;
function _jne(rel: Int32): TBytes; begin Result := _encodeJcc($75, rel); end;
function _jnz(rel: Int32): TBytes; begin Result := _jne(rel);            end;
function _jbe(rel: Int32): TBytes; begin Result := _encodeJcc($76, rel); end;
function _jna(rel: Int32): TBytes; begin Result := _jbe(rel);            end;
function _ja(rel: Int32): TBytes;  begin Result := _encodeJcc($77, rel); end;
function _jnbe(rel: Int32): TBytes;begin Result := _ja(rel);             end;
function _js(rel: Int32): TBytes;  begin Result := _encodeJcc($78, rel); end;
function _jns(rel: Int32): TBytes; begin Result := _encodeJcc($79, rel); end;
function _jp(rel: Int32): TBytes;  begin Result := _encodeJcc($7A, rel); end;
function _jpe(rel: Int32): TBytes; begin Result := _jp(rel);             end;
function _jpo(rel: Int32): TBytes; begin Result := _encodeJcc($7B, rel); end;
function _jnp(rel: Int32): TBytes; begin Result := _jpo(rel);            end;
function _jl(rel: Int32): TBytes;  begin Result := _encodeJcc($7C, rel); end;
function _jnge(rel: Int32): TBytes;begin Result := _jl(rel);             end;
function _jge(rel: Int32): TBytes; begin Result := _encodeJcc($7D, rel); end;
function _jnl(rel: Int32): TBytes; begin Result := _jge(rel);            end;
function _jle(rel: Int32): TBytes; begin Result := _encodeJcc($7E, rel); end;
function _jng(rel: Int32): TBytes; begin Result := _jle(rel);            end;
function _jg(rel: Int32): TBytes;  begin Result := _encodeJcc($7F, rel); end;
function _jnle(rel: Int32): TBytes;begin Result := _jg(rel);             end;

function _jecxz(rel8: Int8): TBytes;begin Result := ToBytes([$E3, rel8]); end;
function _jcxz(rel8: Int8): TBytes; begin Result := _jecxz(rel8);    end;




