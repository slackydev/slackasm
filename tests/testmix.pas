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
    WriteLn(_mov(ebp+12, eax));
    WriteLn(_mov(ebp+12, ecx));

    code += _mov(@x, eax);
    code += _add(imm(10), _ax);
    code += _mov(_ax, @y);
    code += _ret;
    Method := Finalize();
  finally
    WriteLn(Code);
    Free(False);
  end;

  //Method();
  WriteLn([x,y]);
  FreeMethod(@method);
end.
