program int64;
// Nothing of the complex sort here (no idiv, bitshit etc)
{$I slackasm/assembler.pas}
{$X+}

procedure mov64(var a:TSlackASM; dst: TMemVar);
begin
  a.code += a._mov(edx, dst);
  a.code += a._mov(eax, dst.Offset(4));
end;

procedure bitwiseAnd64(var a:TSlackASM; x,y: TMemVar);
begin
  with a do
  begin
    code += _mov(x, edx);
    code += _mov(x.Offset(4), eax);
    code += _and(y, edx);
    code += _and(y.Offset(4), eax);
  end;
end;

procedure add64(var a:TSlackASM; x,y: TMemVar);
begin
  with a do
  begin
    code += _mov(x, edx);
    code += _mov(x.Offset(4), eax);
    code += _add(y, edx);
    code += _adc(y.Offset(4), eax);
  end;
end;

procedure sub64(var a:TSlackASM; x,y: TMemVar);
begin
  with a do
  begin
    code += _mov(x, edx);
    code += _mov(x.Offset(4), eax);
    code += _sub(y, edx);
    code += _sbb(y.Offset(4), eax);
  end;
end;

procedure imul64(var a:TSlackASM; x,y: TMemVar);
begin
  with a do
  begin
    code += _mov (x, ecx);
    code += _mov (x.Offset(4), eax);
    code += _imul(y, eax);
    code += _imul(y.Offset(4), ecx);
    code += _add (eax, ecx);
    code += _mov (y, eax);
    code += _mul (x);
    code += _add (ecx, edx);
  end;
end;



var
  Test: external procedure();
  z,x,y: Int64 = 10;
begin
  var assembler := TSlackASM.Create();
  add64(assembler, mem(x), mem(y));
  mov64(assembler, mem(z));
  assembler.Code += assembler._ret;

  Test := assembler.Finalize();
  WriteLn assembler.Code;
  assembler.Free();

  Test();
  WriteLn([x, y, z]);

  FreeMethod(@Test);
end.
