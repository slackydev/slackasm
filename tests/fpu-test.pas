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


function LapeFMulFunc(sz1,sz2: Byte): Pointer;
const
  NUM_ARGS = 2;
var
  assembler: TSlackASM;
begin
  with assembler := TSlackASM.Create() do
  try
    LapeFuncPrologue(assembler, NUM_ARGS);
    code += _mov(ebp-4, ebx) + _fld(ref(ebx) is sz1);
    code += _mov(ebp-8, ebx) + _fld(ref(ebx) is sz2);
    code += _fmulp;
    code += _mov(ebp+12, ebx);
    code += _fstp(ref(ebx) is f64);
    LapeFuncEpilogue(assembler, NUM_ARGS);
    Result := Finalize();
  finally
    Free();
  end;
end;


var
  fmul: external function(x:Single; y:Double): Double;
begin
  fmul := LapeFMulFunc(f32,f64);
  WriteLn fmul(62000, 2);
  FreeMethod(@fmul);
end.
