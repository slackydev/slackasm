program lapefunc;
{$assertions ON}
{$I slackasm/assembler.pas}
{$X+}

procedure LapeFuncPrologue(var a:TSlackASM; argCount:Word);
var i:Int32;
begin
  a.Code += _push(ebp);
  a.Code += _mov(esp, ebp);
  a.Code += _mov(ebp+08, ebx);
  for i:=0 to argCount-1 do
  begin
    a.Code += _mov(ref(ebx), ecx);
    a.Code += _push(ecx);
    if i <> argCount-1 then
      a.Code += _add(imm(4), ebx);
  end;
end;

procedure LapeFuncEpilogue(var a:TSlackASM; argCount:Int32);
begin
  a.Code += _add(imm(4*argCount), esp);
  a.Code += _pop(ebp);
  a.Code += _ret;
end;


function LapeMulFunc(sz1,sz2: Byte): Pointer;
const NUM_ARGS = 2;
var   assembler: TSlackASM;
begin
  with assembler := TSlackASM.Create() do
  try
    LapeFuncPrologue(assembler, NUM_ARGS);
    code += _mov(ebp-4, edx) + _movzx(ref(edx).AsType(sz1), eax);
    code += _mov(ebp-8, edx) + _movzx(ref(edx).AsType(sz2), ecx);
    code += _imul(ecx, eax);
    code += _mov(ebp+12, edx);
    code += _mov(eax, ref(edx));
    LapeFuncEpilogue(assembler, NUM_ARGS);
    Result := Finalize();
  finally
    WriteLn(Code);
    Free()
  end;
end;


var
  mul: external function(x,y:Int32): Int32;
begin
  mul := LapeMulFunc(i32,i32);
  WriteLn mul(62000, 2);
  FreeMethod(@mul);
end.
