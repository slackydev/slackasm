program test1;
{$I slackasm/assembler.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  x,y: Int32;
  b: Boolean;
begin
  x := 100;
  y := 10;
  with assembler := TSlackASM.Create(2 shl 11) do
  try
    code += _imul(ecx, eax);
    code += _ret;
    Method := Finalize();
  finally
    WriteLn(Code);
    Free(False);
  end;

  //Method();
  WriteLn([x, y, b]);
  FreeMethod(@method);
end.
