program test1;
{$I slackasm/assembler.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  x: Int32 = 2;
  y: Int32 = 100;
  z,tmp,i: Int32;
  lim: Int32 = 25000000 - 1;
  t: Int64;
begin
  //---- Pure lape version which we'll write
  Write('Pure Lape: ');
  t := GetTickCount();
    repeat
      tmp := x+y;
      z := (tmp*tmp) div y;
      Inc(i);
    until lim < i;
  WriteLn(Format('Used %4d ms', [GetTickCount()-t]),': ',  [x, y, z, i]);


  //---- Now let's do that with assembly (using a do..while loop)
  with assembler := TSlackASM.Create(2 shl 11) do
  try
    code += _mov(imm(0),EBX);        // move 0 to %ebx (it's kept in %ebx, as the loop counter)
    var lbl1 := Location;            // make a label so we can jump here
    //loop body -->
    code += _mov (@x,   EAX);        // move `x` to %eax
    code += _add (@y,   EAX);        // add `y` to %eax
    code += _imul(EAX      );        // imul %eax            [EAX *= EAX]
    code += _cltq;                   // convert long to quad [div uses both EAX and EDX]
    code += _idiv(@y       );        // %eax div `y`         [EAX has result, EDX has remainder]
    code += _mov (EAX,  @z );        // move %eax to `z`
    code += _inc (EBX      );        // inc %ebx             [increase our counter]
    code += _cmp (@lim, EBX);        // compare `lim` to %ebx
    code += _jle (RelLoc(lbl1));     // if %ebx <= lim then goto lbl1
    //<-- loop end
    code += _mov (EBX,  @i );        // move %ebx to `i`
    code += _ret;                    // return
    Method := Finalize();            // Build a function from the assembler
  finally
  //WriteLn(assembler.Code);         // (print the machinecode we produced)
    Free(False);                     // free it, but not the method we made
  end;

  Write('Lape ASM : ');
  t := GetTickCount();
  try
    z := 0;
    i := 0;
    Method();
  finally
    FreeMethod(@method);
  end;
  WriteLn(Format('Used %4d ms', [GetTickCount()-t]),': ',  [x, y, z, i]);
end.
