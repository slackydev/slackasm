{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Instructions that act on the FPU
[-----------------------------------------------------------------------------}

// ===========================================================================
// FILD - Load Integer To FPU Stack (And Convert To 80bit Float)

// FILDW m16
procedure TSlackASM._fildw(src: PInt32); overload; 
begin
  Self.EmitTo(src, SizeOf(Pointer), Self.Emit([$DF, $05]));
end;

// FILDL m32
procedure TSlackASM._fildl(src: PInt32); overload; 
begin
  Self.EmitTo(src, SizeOf(Pointer), Self.Emit([$DB, $05]));
end;

// FILDLL/FILDQ m64
procedure TSlackASM._fildq(src: PInt64); overload; 
begin
  Self.EmitTo(src, SizeOf(Pointer), Self.Emit([$DF, $2D]));
end;


// ===========================================================================
// FLD - Load Float To FPU Stack (And Convert To 80bit Float)

// FLD freg
procedure TSlackASM._fld(reg: TFPURegister); overload; 
begin
  Self.Emit([$D9, $C0+Ord(reg)]);
end;

// FLDS m64fp
procedure TSlackASM._flds(src: PSingle); overload; 
begin
  Self.EmitTo(src, SizeOf(Pointer), Self.Emit([$D9, $05]));
end;

// FLDL m64fp
procedure TSlackASM._fldl(src: PDouble); overload; 
begin
  Self.EmitTo(src, SizeOf(Pointer), Self.Emit([$DD, $05]));
end;


// ===========================================================================
// FISTP - Store Integer To Memory And Pop

// FISTPW m16
procedure TSlackASM._fistpw(dst: PInt16); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$66, $DF, $1D]));
end;

// FISTPL m32
procedure TSlackASM._fistpl(dst: PInt32); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$DB, $1D]));
end;

// FISTPQ m64
procedure TSlackASM._fistpq(dst: PInt64); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$DF, $3D]));
end;


// ===========================================================================
// FSTP - Store Float To Memory And Pop

// FSTP %freg
procedure TSlackASM._fstp(reg: TFPURegister); overload; 
begin
  Self.Emit([$DD, $D8+Ord(reg)]);
end;

// FSTPS m32fp
procedure TSlackASM._fstps(dst: PSingle); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$D9, $1D]));
end;

// FSTPL m64fp
procedure TSlackASM._fstpl(dst: PDouble); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$DD, $1D]));
end;



// ===========================================================================
// FIST - Store Integer To Memory And Pop

// FISTW m16fp
procedure TSlackASM._fistw(dst: PInt16); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$66, $DF, $15]));
end;

// FISTL m32fp
procedure TSlackASM._fistl(dst: PInt32); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$DB, $15]));
end;


// ===========================================================================
// FST - Store Float To Memory

// FST %freg
procedure TSlackASM._fst(reg: TFPURegister); overload; 
begin
  Self.Emit([$DD, $D0+Ord(reg)]);
end;

// FSTS m32fp
procedure TSlackASM._fsts(dst: PSingle); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$D9, $15]));
end;

// FSTL m64fp
procedure TSlackASM._fstl(dst: PDouble); overload; 
begin
  Self.EmitTo(dst, SizeOf(Pointer), Self.Emit([$DD, $15]));
end;


// ===========================================================================
// FABS - Absolute Value - Clear the sign of st0

procedure TSlackASM._fabs(); overload; 
begin
  Self.Emit([$D9, $E1]);
end;



// ===========================================================================
// FADD - Floating Point Addition

procedure TSlackASM._faddp(); overload; 
begin
  Self.Emit([$DE, $C1]);
end;

// FADD m32fp
procedure TSlackASM._fadds(mem: PSingle); overload; 
begin
  Self.EmitTo(mem, SizeOf(Pointer), Self.Emit([$D8, $05]));
end;

// FADD m64fp
procedure TSlackASM._faddl(mem: PDouble); overload; 
begin
  Self.EmitTo(mem, SizeOf(Pointer), Self.Emit([$DC, $05]));
end;


// FADD stk32fp
procedure TSlackASM._fadds(stk: TStackVar); overload; 
begin
  if GetIntSize(stk.Offset, [1,4]) = 1 then
    Self.EmitTo(stk.Offset, 1, Self.Emit([$D8, $45]))
  if GetIntSize(stk.Offset, [1,4]) = 4 then
    Self.EmitTo(stk.Offset, 4, Self.Emit([$D8, $85]))
end;

// FADD stk64fp
procedure TSlackASM._faddl(stk: TStackVar); overload; 
begin
  if GetIntSize(stk.Offset, [1,4]) = 1 then
    Self.EmitTo(stk.Offset, 1, Self.Emit([$DC, $45]))
  if GetIntSize(stk.Offset, [1,4]) = 4 then
    Self.EmitTo(stk.Offset, 4, Self.Emit([$DC, $85]))
end;





