{-----------------------------------------------------------------------------]
  Author: Jarl K. Holta
  License: GNU Lesser GPL (http://www.gnu.org/licenses/lgpl.html)
  
  Misc instructions...
[-----------------------------------------------------------------------------}

// ----------------------------------------------------------------------------
// return
function TSlackASM._ret(): TBytes;
begin
  Result := ToBytes([$C3]);
end;


// ----------------------------------------------------------------------------
// Arithmetics intrinsics - higher level helpers
(*
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
*)
