{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the FPU
[-----------------------------------------------------------------------------}

// ===========================================================================
// FILD - Load Integer To FPU Stack (And Convert To 80bit Float)

// FILDW %m16
procedure TSlackASM._fildw(src: PInt32);
begin
  WriteBytes([$DF, $05]);
  Self.WriteAddr(src);
end;

// FILDL %m32
procedure TSlackASM._fildl(src: PInt32);
begin
  WriteBytes([$DB, $05]);
  Self.WriteAddr(src);
end;

// FILDLL/FILDQ %m64
procedure TSlackASM._fildq(src: PInt64);
begin
  WriteBytes([$DF, $2D]);
  Self.WriteAddr(src);
end;


// ===========================================================================
// FLD - Load Float To FPU Stack (And Convert To 80bit Float)

// FLD %fpureg
procedure TSlackASM._fld(reg: TFPURegister);
begin
  WriteBytes([$D9, $C0+Ord(reg)]);
end;

// FLDS %m64
procedure TSlackASM._flds(src: PSingle);
begin
  WriteBytes([$D9, $05]);
  Self.WriteAddr(src);
end;

// FLDL %m64
procedure TSlackASM._fldl(src: PDouble);
begin
  WriteBytes([$DD, $05]);
  Self.WriteAddr(src);
end;


// ===========================================================================
// FISTP - Store Integer To Memory And Pop

// FISTPW %m16
procedure TSlackASM._fistpw(dst: PInt16);
begin
  WriteBytes([$66, $DF, $1D]);
  Self.WriteAddr(dst);
end;

// FISTPL %m32
procedure TSlackASM._fistpl(dst: PInt32);
begin
  WriteBytes([$DB, $1D]);
  Self.WriteAddr(dst);
end;

// FISTPQ %m64
procedure TSlackASM._fistpq(dst: PInt64);
begin
  WriteBytes([$DF, $3D]);
  Self.WriteAddr(dst);
end;


// ===========================================================================
// FSTP - Store Float To Memory And Pop

// FSTP %fpureg
procedure TSlackASM._fstp(reg: TFPURegister);
begin
  WriteBytes([$DD, $D8+Ord(reg)]);
end;

// FSTPS %m32
procedure TSlackASM._fstps(dst: PSingle);
begin
  WriteBytes([$D9, $1D]);
  Self.WriteAddr(dst);
end;

// FSTPL %m64
procedure TSlackASM._fstpl(dst: PDouble);
begin
  WriteBytes([$DD, $1D]);
  Self.WriteAddr(dst);
end;



// ===========================================================================
// FIST - Store Integer To Memory And Pop

// FISTW %m16
procedure TSlackASM._fistw(dst: PInt16);
begin
  WriteBytes([$66, $DF, $15]);
  Self.WriteAddr(dst);
end;

// FISTL %m32
procedure TSlackASM._fistl(dst: PInt32);
begin
  WriteBytes([$DB, $15]);
  Self.WriteAddr(dst);
end;


// ===========================================================================
// FST - Store Float To Memory

// FST %fpureg
procedure TSlackASM._fst(reg: TFPURegister);
begin
  WriteBytes([$DD, $D0+Ord(reg)]);
end;

// FSTS %m32
procedure TSlackASM._fsts(dst: PSingle);
begin
  WriteBytes([$D9, $15]);
  Self.WriteAddr(dst);
end;

// FSTL %m64
procedure TSlackASM._fstl(dst: PDouble);
begin
  WriteBytes([$DD, $15]);
  Self.WriteAddr(dst);
end;



