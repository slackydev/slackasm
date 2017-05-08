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
    code += _mov(mem(x),  ebx);
    code += _setc(SETNE, mem(x) is i8);
    code += _setc(SETNE, mem(x) is i16);
    code += _setc(SETNE, mem(x) is i32);
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
