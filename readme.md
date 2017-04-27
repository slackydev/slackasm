A small assembler in Lape, requires Simba 1.2+ or just the lape interpreter itself.
Operations follow the same order as found in AT&T syntax, opcode names are a mixture of Intel and AT&T.


Example:
```pascal
{$I slackasm/assembler.pas}

var
  assembler: TSlackASM;
  callable: external procedure();
  x: Int32 = 2;
  y: Int32 = 100;
  z: Int32;
begin
  // result -> (x + y) * x
  with assembler := TSlackASM.Create() do
  try
    code += _mov (@x,   EAX);        // EAX := x
    code += _add (@y,   EAX);        // EAX := EAX += y
    code += _imul(@x,   EAX);        // EAX := EAX *= x;
    code += _mov (EAX,  @z);         // z   := EAX;
    code += _ret;
    callable := Finalize();   // "Build" the method
  finally
    Free(False);
  end;

  //execute the code:
  callable();
  WriteLn([x, y, z]); //prints 2, 100, 204

  //free the code
  FreeMethod(@callable);
end.
```