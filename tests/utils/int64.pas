program int64;
// Nothing of the complex sort here (no idiv, bitshit etc)
{$I slackasm/assembler.pas}
{$X+}

procedure mov_edxeax(var a:TSlackASM; dst: TMemVar);
begin
  a.code += _mov(edx, dst);
  a.code += _mov(eax, dst+4);
end;

procedure mov_eaxedx(var a:TSlackASM; dst: TMemVar);
begin
  a.code += _mov(eax, dst);
  a.code += _mov(edx, dst+4);
end;

procedure bitwiseAnd64(var a:TSlackASM; x,y: TMemVar);
begin
  a.code += _mov(x, edx);
  a.code += _mov(x+4, eax);
  a.code += _and(y, edx);
  a.code += _and(y+4, eax);
end;

procedure add64(var a:TSlackASM; x,y: TMemVar);
begin
  a.code += _mov(x, edx);
  a.code += _mov(x+4, eax);
  a.code += _add(y, edx);
  a.code += _adc(y+4, eax);
end;

procedure sub64(var a:TSlackASM; x,y: TMemVar);
begin
  a.code += _mov(x, edx);
  a.code += _mov(x+4, eax);
  a.code += _sub(y, edx);
  a.code += _sbb(y+4, eax);
end;

procedure imul64(var a:TSlackASM; x,y: TMemVar);
begin
  a.code += _mov (x, ecx);
  a.code += _mov (x+4, eax);
  a.code += _imul(y, eax);
  a.code += _imul(y+4, ecx);
  a.code += _add (eax, ecx);
  a.code += _mov (y, eax);
  a.code += _mul (x);
  a.code += _add (ecx, edx);
  a.code += _xchg(eax, edx); //not really needed
end;



var
  Test: external procedure();
  z,x,y: Int64 = 10;
begin
  var assembler := TSlackASM.Create();
  imul64(assembler, mem(x), mem(y));
  mov_edxeax(assembler, mem(z));
  assembler.Code += _ret;

  Test := assembler.Finalize();
  WriteLn assembler.Code;
  assembler.Free();

  Test();
  WriteLn([x, y, z]);

  FreeMethod(@Test);
end.
