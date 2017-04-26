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


  //---- Now let's do that with assembly!
  with assembler := TSlackASM.Create(2 shl 11) do
  try
    emit( _mov (@i,   EBX) );        // mov `i` to %ebx (it's kept in %ebx, throughout the loop)
    var lbl1 := Location;            // make a label so we can jump here
    //loop body -->
    emit( _mov (@x,   EAX) );        // mov `x` to %eax
    emit( _add (@y,   EAX) );        // add `y` to %eax
    emit( _imul(EAX      ) );        // imul %EAX            [think Sqr(EAX)]
    emit( _cltq );                   // convert long to quad [div uses both EAX and EDX]
    emit( _idiv(@y       ) );        // %eax div `y`         [EAX has result, EDX has remainder]
    emit( _mov (EAX,  @z ) );        // mov %eax to `z`
    emit( _inc (EBX      ) );        // inc %ebx             [our counter]
    emit( _cmp (@lim, EBX) );        // compare `lim` to %ecx
    emit( _jle (RelLoc(lbl1)));      // if lim <= ecx then goto lbl1
    //<-- loop end
    emit( _mov (EBX,  @i ) );        // mov EBX to `i`
    emit( _ret );                    // return
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
