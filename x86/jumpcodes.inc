{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Jump instructions
[-----------------------------------------------------------------------------}
{$RANGECHECKS OFF}

// ----------------------------------------------------------------------------
// JMP - Unconditional jump
function TSlackASM._jmp(dst: TGPRegister): TBytes; overload;
begin
  Result := ToBytes([$FF, dst.gpReg]);
end;

function TSlackASM._jmp(dst: TMemVar): TBytes; overload;
begin
  Assert(dst.Size = 4, 'Illegal operand size');
  Result := dst.Encode([$FF], EAX, $20+Ord(dst.MemType));
end;

// JMP - Relative Position
function TSlackASM._jmp(rel8: Int8): TBytes; overload;
begin
  if rel8 < 0 then rel8 -= 2; //remove self
  Result := ToBytes([$EB, rel8]);
end;

function TSlackASM._jmp(rel32: Int32): TBytes; overload;
begin
  if rel32 < 0 then rel32 -= 5; //remove self
  Result := ToBytes([$E9]) + long_to_bytes(rel32);
end;


// ----------------------------------------------------------------------------
// Jump if Condition Is Met
function TSlackASM._encodeJcc(jcc:Byte; rel: Int32): TBytes;
begin
  if rel < 0 then rel -= 2; //remove self
  if GetIntSize(rel) = 1 then
    Result := ToBytes([jcc, rel])
  else
    Result := ToBytes([$0F, jcc+$10]) + long_to_bytes(rel);
end;

function TSlackASM._jo(rel: Int32): TBytes;  begin Result := Self._encodeJcc($70, rel); end;
function TSlackASM._jno(rel: Int32): TBytes; begin Result := Self._encodeJcc($71, rel); end;
function TSlackASM._jb(rel: Int32): TBytes;  begin Result := Self._encodeJcc($72, rel); end;
function TSlackASM._jc(rel: Int32): TBytes;  begin Result := Self._jb(rel);             end;
function TSlackASM._jnae(rel: Int32): TBytes;begin Result := Self._jb(rel);             end;
function TSlackASM._jae(rel: Int32): TBytes; begin Result := Self._encodeJcc($73, rel); end;
function TSlackASM._jnb(rel: Int32): TBytes; begin Result := Self._jae(rel);            end;
function TSlackASM._jnc(rel: Int32): TBytes; begin Result := Self._jae(rel);            end;
function TSlackASM._je(rel: Int32): TBytes;  begin Result := Self._encodeJcc($74, rel); end;
function TSlackASM._jz(rel: Int32): TBytes;  begin Result := Self._je(rel);             end;
function TSlackASM._jne(rel: Int32): TBytes; begin Result := Self._encodeJcc($75, rel); end;
function TSlackASM._jnz(rel: Int32): TBytes; begin Result := Self._jne(rel);            end;
function TSlackASM._jbe(rel: Int32): TBytes; begin Result := Self._encodeJcc($76, rel); end;
function TSlackASM._jna(rel: Int32): TBytes; begin Result := Self._jbe(rel);            end;
function TSlackASM._ja(rel: Int32): TBytes;  begin Result := Self._encodeJcc($77, rel); end;
function TSlackASM._jnbe(rel: Int32): TBytes;begin Result := Self._ja(rel);             end;
function TSlackASM._js(rel: Int32): TBytes;  begin Result := Self._encodeJcc($78, rel); end;
function TSlackASM._jns(rel: Int32): TBytes; begin Result := Self._encodeJcc($79, rel); end;
function TSlackASM._jp(rel: Int32): TBytes;  begin Result := Self._encodeJcc($7A, rel); end;
function TSlackASM._jpe(rel: Int32): TBytes; begin Result := Self._jp(rel);             end;
function TSlackASM._jpo(rel: Int32): TBytes; begin Result := Self._encodeJcc($7B, rel); end;
function TSlackASM._jnp(rel: Int32): TBytes; begin Result := Self._jpo(rel);            end;
function TSlackASM._jl(rel: Int32): TBytes;  begin Result := Self._encodeJcc($7C, rel); end;
function TSlackASM._jnge(rel: Int32): TBytes;begin Result := Self._jl(rel);             end;
function TSlackASM._jge(rel: Int32): TBytes; begin Result := Self._encodeJcc($7D, rel); end;
function TSlackASM._jnl(rel: Int32): TBytes; begin Result := Self._jge(rel);            end;
function TSlackASM._jle(rel: Int32): TBytes; begin Result := Self._encodeJcc($7E, rel); end;
function TSlackASM._jng(rel: Int32): TBytes; begin Result := Self._jle(rel);            end;
function TSlackASM._jg(rel: Int32): TBytes;  begin Result := Self._encodeJcc($7F, rel); end;
function TSlackASM._jnle(rel: Int32): TBytes;begin Result := Self._jg(rel);             end;

function TSlackASM._jecxz(rel8: Int8): TBytes;begin Result := ToBytes([$E3, rel8]); end;
function TSlackASM._jcxz(rel8: Int8): TBytes; begin Result := Self._jecxz(rel8);    end;




