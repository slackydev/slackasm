program test1;
{$I slackasm/assembler.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  z,x,y: Int32;
begin
  x := 100;
  y := 10;
  with assembler := TSlackASM.Create() do
  try
    code += _mov(mem(x), ebx);
    code += _imul(imm(10), _ax);
    code += _imul(imm(10), _cx);
    code += _imul(imm(10), _dx);
    code += _imul(imm(10), _bx);
    code += _mov(ebx, mem(z));
    code += _ret;
    Method := Finalize();
  finally
    WriteLn(Code);
    Free();
  end;

  Method();
  WriteLn([x, y, z]);
  FreeMethod(@method);
end.
