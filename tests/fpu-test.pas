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


function LapeFMulFunc(sz1,sz2: Byte): Pointer;
const
  NUM_ARGS = 2;
var
  assembler: TSlackASM;
begin
  with assembler := TSlackASM.Create() do
  try
    LapeFuncPrologue(assembler, NUM_ARGS);
    code += _mov(ebp-4, ebx) + _fld(ref(ebx).AsType(sz1));
    code += _mov(ebp-8, ebx) + _fld(ref(ebx).AsType(sz2));
    code += _fmulp;
    code += _mov(ebp+12, ebx);
    code += _fstp(ref(ebx).AsType(f64));
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
