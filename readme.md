A simple assembler in Lape, requires Simba 1.2+ or just the latest lape interpreter (might need tweaks).
Operations follow the same order as found in AT&T syntax, opcode names are mainly similar to Intel's.


Basic example:
```pascal
{$I slackasm/assembler.pas}

var
  assembler: TSlackASM;
  callable: external procedure();
  x: Int32 = 2;
  y: Int32 = 100;
  z: Int32;
begin
  // compute: (x + y) * x
  with assembler := TSlackASM.Create() do
  try
    code += _mov (mem(x), eax);        // eax := x
    code += _add (mem(y), eax);        // eax := eax += y
    code += _imul(mem(x), eax);        // eax := eax *= x;
    code += _mov (eax, mem(z));        // z   := eax;
    code += _ret;
    callable := Finalize();   // "Build" the method
  finally
    Free(False);
  end;

  // execute the code:
  callable();
  WriteLn([x, y, z]); //prints 2, 100, 204

  // free the code
  FreeMethod(@callable);
end.
```


For more indepth examples, check out the Examples folder.