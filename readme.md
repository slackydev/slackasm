A simple assembler in Lape, requires Simba 1.2+ or just the latest lape interpreter (might need tweaks).
Operations follow the same order as found in AT&T syntax, opcode names are mainly similar to Intel's.


Basic example:
```pascal
{$I slackasm/assembler.pas}

var
  Assembler: TSlackASM;
  Callable: external procedure();
  x: Int32 = 2;
  y: Int32 = 100;
  z: Int32;
begin
  // compute: (x + y) * x
  with Assembler := TSlackASM.Create() do
  try
    Code += _mov (mem(x), eax);        // eax := x
    Code += _add (mem(y), eax);        // eax := eax += y
    Code += _imul(mem(x), eax);        // eax := eax *= x;
    Code += _mov (eax, mem(z));        // z   := eax;
    Code += _ret;
    Callable := Finalize();   // "Build" the method
  finally
    Free();
  end;

  // execute the code
  Callable();
  WriteLn([x, y, z]); //prints 2, 100, 204

  // free the code
  FreeMethod(@callable);
end.
```


For more indepth examples, check out the Examples folder.