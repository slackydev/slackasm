program lapefunc;
{$assertions ON}
{$I slackasm/assembler.pas}
{$X+}

procedure LapeFuncPrologue(var assembler:TSlackASM; argCount:Word);
var
  i:Int32;
begin
  with assembler do
  begin
    code += _push(ebp);
    code += _mov(esp, ebp);
    code += _mov(ebp+08, ebx);
    for i:=0 to argCount-1 do
    begin
      code += _mov(ref(ebx), ecx);
      code += _push(ecx);
      if i <> argCount-1 then
        code += _add(imm(4), ebx);
    end;
  end;
end;

procedure LapeFuncEpilogue(var assembler:TSlackASM; argCount:Int32);
begin
  assembler.code += assembler._add(imm(4*argCount), esp);
  assembler.code += assembler._pop(ebp);
  assembler.code += assembler._ret;
end;


function LapeMulFunc(DTYPE:Byte=szLONG): Pointer;
const NUM_ARGS = 2;
var   assembler: TSlackASM;
begin
  with assembler := TSlackASM.Create() do
  try
    LapeFuncPrologue(assembler, NUM_ARGS);
    code += _mov(ebp-4, edx);
    code += _movzx(ref(edx) is DTYPE, eax);
    code += _mov(ebp-8, edx);
    code += _movzx(ref(edx) is DTYPE, ecx);
    code += _imul(ecx, eax);
    code += _mov(ebp+12, edx);
    code += _mov(eax, ref(edx));
    LapeFuncEpilogue(assembler, NUM_ARGS);
    Result := Finalize();
  finally
    Free()
  end;
end;


var
  mul: external function(x,y:Int32): Int32;
begin
  mul := LapeMulFunc( SizeOf(Int32) );
  WriteLn mul(62000, 2);
  FreeMethod(@mul);
end.
