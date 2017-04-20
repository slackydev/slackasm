{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Jump instructions
[-----------------------------------------------------------------------------}

// ----------------------------------------------------------------------------
// JMP - Unconditional jump
procedure TSlackASM._jmp(dst: TGPRegister32); overload;
begin
  Self.WriteBytes([$FF, Ord(dst)]);
end;

procedure TSlackASM._jmp(dst: Pointer); overload;
begin
  Self.WriteBytes([$FF, $25]);
  Self.WriteAddr(dst);
end;

// JMP - Relative Position
procedure TSlackASM._jmp(rel8: Int8); overload;
begin
  if rel8 < 0 then rel8 -= 1; //remove self
  Self.WriteBytes([$EB, rel8]);
end;

procedure TSlackASM._jmp(rel32: Int32); overload;
begin
  Self.WriteBytes([$E9]);
  if rel32 < 0 then rel32 -= 1; //remove self
  WriteInt(rel32, SizeOf(Int32));
end;


// ----------------------------------------------------------------------------
// Jump if Condition Is Met
procedure TSlackASM._encodeJcc(jcc:Byte; rel: Int32);
begin
  if rel < 0 then rel -= 2; //remove self
  if (rel >= Low(Int8)) and (rel <= High(Int8)) then
    Self.WriteBytes([jcc, rel])
  else
  begin
    Self.WriteBytes([$0F, jcc+$10]);
    WriteInt(rel, 4);
  end;
end;

procedure TSlackASM._jo(rel: Int32);  overload; begin Self._encodeJcc($70, rel); end;
procedure TSlackASM._jno(rel: Int32); overload; begin Self._encodeJcc($71, rel); end;
procedure TSlackASM._jb(rel: Int32);  overload; begin Self._encodeJcc($72, rel); end;
procedure TSlackASM._jc(rel: Int32);  overload; begin Self._jb(rel);             end;
procedure TSlackASM._jnae(rel: Int32);overload; begin Self._jb(rel);             end;
procedure TSlackASM._jae(rel: Int32); overload; begin Self._encodeJcc($73, rel); end;
procedure TSlackASM._jnb(rel: Int32); overload; begin Self._jae(rel);            end;
procedure TSlackASM._jnc(rel: Int32); overload; begin Self._jae(rel);            end;
procedure TSlackASM._je(rel: Int32);  overload; begin Self._encodeJcc($74, rel); end;
procedure TSlackASM._jz(rel: Int32);  overload; begin Self._je(rel);             end;
procedure TSlackASM._jne(rel: Int32); overload; begin Self._encodeJcc($75, rel); end;
procedure TSlackASM._jnz(rel: Int32); overload; begin Self._jne(rel);            end;
procedure TSlackASM._jbe(rel: Int32); overload; begin Self._encodeJcc($76, rel); end;
procedure TSlackASM._jna(rel: Int32); overload; begin Self._jbe(rel);            end;
procedure TSlackASM._ja(rel: Int32);  overload; begin Self._encodeJcc($77, rel); end;
procedure TSlackASM._jnbe(rel: Int32);overload; begin Self._ja(rel);             end;
procedure TSlackASM._js(rel: Int32);  overload; begin Self._encodeJcc($78, rel); end;
procedure TSlackASM._jns(rel: Int32); overload; begin Self._encodeJcc($79, rel); end;
procedure TSlackASM._jp(rel: Int32);  overload; begin Self._encodeJcc($7A, rel); end;
procedure TSlackASM._jpe(rel: Int32); overload; begin Self._jp(rel);             end;
procedure TSlackASM._jpo(rel: Int32); overload; begin Self._encodeJcc($7B, rel); end;
procedure TSlackASM._jnp(rel: Int32); overload; begin Self._jpo(rel);            end;
procedure TSlackASM._jl(rel: Int32);  overload; begin Self._encodeJcc($7C, rel); end;
procedure TSlackASM._jnge(rel: Int32);overload; begin Self._jl(rel);             end;
procedure TSlackASM._jge(rel: Int32); overload; begin Self._encodeJcc($7D, rel); end;
procedure TSlackASM._jnl(rel: Int32); overload; begin Self._jge(rel);            end;
procedure TSlackASM._jle(rel: Int32); overload; begin Self._encodeJcc($7E, rel); end;
procedure TSlackASM._jng(rel: Int32); overload; begin Self._jle(rel);            end;
procedure TSlackASM._jg(rel: Int32);  overload; begin Self._encodeJcc($7F, rel); end;
procedure TSlackASM._jnle(rel: Int32);overload; begin Self._jg(rel);             end;

procedure TSlackASM._jecxz(rel: Int8);overload; begin Self.WriteBytes([$E3, rel]) end;
procedure TSlackASM._jcxz(rel: Int8); overload; begin Self._jecxz(rel) end;




