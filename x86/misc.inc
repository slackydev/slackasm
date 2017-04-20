{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Misc instructions...
[-----------------------------------------------------------------------------}

// ----------------------------------------------------------------------------
// return
procedure TSlackASM._ret();
begin
  WriteBytes([$C3]);
end;

// basic prologue
procedure TSlackASM._prologue();   
begin
  Self.WriteBytes([$55]);          //  push rbp
  Self.WriteBytes([$48,$89,$e5]);  //  mov  rbp, rsp
end;

// basic epilogue
procedure TSlackASM._epilogue();   
begin
  Self.WriteBytes([$5d]);          // pop rbp  | restore old pointer  
end;


// ----------------------------------------------------------------------------
// Arithmetics intrinsics - higher level helpers
procedure TSlackASM._addl(x,y: PInt32; store: TGPRegister32=EAX); overload;
begin
  Self._movl(y, store);
  Self._addl(x, store);
end;

procedure TSlackASM._subl(x,y: PInt32; store: TGPRegister32=EAX); overload;
begin
  Self._movl(y, store);
  Self._subl(x, store);
end;

procedure TSlackASM._imull(x,y: PInt32; store: TGPRegister32=EAX); overload;
begin
  Self._movl(y, store);
  Self._imull(x, store);
end; 
