// ----------------------------------------------------------------------------
// return
procedure TSlackASM._ret();
begin
  WriteBytes([$C3]);
end;

// ----------------------------------------------------------------------------
// helper for common encoding
function TSlackASM._reg_reg(reg1, reg2: TGPRegister32): Byte; overload;
begin
  Result := $C0 + ord(reg1)*8 + ord(reg2);
end;

function TSlackASM._reg_reg(reg1, reg2: TGPRegister16): Byte; overload;
begin
  Result := $C0 + ord(reg1)*8 + ord(reg2);
end;

function TSlackASM._reg_reg(reg1, reg2: TGPRegister8): Byte; overload;
begin
  Result := $C0 + ord(reg1)*8 + ord(reg2);
end;

function TSlackASM._mem_reg(reg: TGPRegister32): Byte; overload;
begin
  Result := Ord(reg)*8 + $05;
end;

function TSlackASM._mem_reg(reg: TGPRegister16): Byte; overload;
begin
  Result := Ord(reg)*8 + $05;
end;

function TSlackASM._mem_reg(reg: TGPRegister8): Byte; overload;
begin
  Result := Ord(reg)*8 + $05;
end;


// ----------------------------------------------------------------------------
// MOV reg,reg intrinsic
procedure TSlackASM._mov(src, dst: TGPRegister32); overload;
begin
  WriteBytes([$89, _reg_reg(dst, src)]);
end;


// ----------------------------------------------------------------------------
// MOV m32/r32 (move from memory to reg)
procedure TSlackASM._movl(src: PInt32; dst: TGPRegister32); overload;
begin
  Self.WriteBytes([$8B, _mem_reg(dst)]);
  Self.WriteAddr(src);
end;

procedure TSlackASM._movl(src: TGPRegister32; dst: PInt32); overload;
begin
  Self.WriteBytes([$89, _mem_reg(src)]);
  Self.WriteAddr(dst);
end;

// ----------------------------------------------------------------------------
// various
procedure TSlackASM._inc(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$40, Ord(reg)]);
end;

procedure TSlackASM._incl(mem: PInt32); overload;
begin
  Self.WriteBytes([$FF,$05]);
  Self.WriteAddr(mem);
end;

procedure TSlackASM._dec(reg: TGPRegister32); overload;
begin
  Self.WriteBytes([$48, Ord(reg)]);
end;

procedure TSlackASM._decl(mem: PInt32); overload;
begin
  Self.WriteBytes([$FF,$0D]);
  Self.WriteAddr(mem);
end;


procedure TSlackASM._clqd();
begin
  Self.WriteBytes([$99]);
end;


// ----------------------------------------------------------------------------
// Arithmetics intrinsics - memory values
procedure TSlackASM._addl(x,y: PInt32; dest: TGPRegister32=EAX); overload;
begin
  Self._movl(y, dest);
  Self._addl(x, dest);
end;

procedure TSlackASM._subl(x,y: PInt32; dest: TGPRegister32=EAX); overload;
begin
  Self._movl(y, dest);
  Self._subl(x, dest);
end;

procedure TSlackASM._imull(x,y: PInt32; dest: TGPRegister32=EAX); overload;
begin
  Self._movl(y, dest);
  Self._imull(x, dest);
end; 
