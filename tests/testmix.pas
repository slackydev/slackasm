program test1;
{$I slackasm/assembler.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  x,y: Int32;
begin
  x := 100;

  with assembler := TSlackASM.Create(2 shl 11) do
  try
    code += _mov(@x, EAX);
    code += _add(imm(10), _AX);
    code += _mov(_AX, @y);
    code += _ret;
    Method := Finalize();
  finally
    WriteLn(Code);
    Free(False);
  end;

  Method();
  WriteLn([x,y]);
  FreeMethod(@method);
end.
