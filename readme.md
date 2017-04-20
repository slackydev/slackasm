A small assembler in Lape, requires Simba 1.2+ or just the lape interpreter itself.


Example:
```pascal
{$I slackasm/asm.pas}

var
  assembler: TSlackASM;
  callable: external procedure();
  x: Int32 = 2;
  y: Int32 = 100;
begin
  // build the code
  with assembler := TSlackASM.Create() do
  try
    _movl (@x,   EAX);        // EAX := x
    _addl (@y,   EAX);        // EAX := y + EAX
    _imull(@x,   EAX);        // EAX := x * EAX;
    _movl (EAX,  @y);         // y   := EAX;
    _ret;
    callable := Finalize();   // "Build" the method
  finally
    Free(False);
  end;
  
  //execute the code:
  callable();
  WriteLn([x, y]);

  //free the code
  FreeMethod(@callable); 
end.
```