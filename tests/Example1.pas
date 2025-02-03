program Example1;
{$I slackasm/assembler.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  x: Int32 = 2;
  y: Int32 = 100;
  z,tmp,i: Int32;
  lim: Int32 = 50000000 - 1;
  t: Int64;
begin
  //---- Pure lape version which we'll write
  Write('Pure Lape: ');
  t := GetTickCount();
    repeat
      tmp := x+y;
      z := (tmp*tmp) - y;
      Inc(i);
    until lim < i;
  WriteLn(Format('Used %4d ms', [GetTickCount()-t]),': ',  [x, y, z, i]);


  //---- Now let's do that with assembly (using a do..while loop)
  with assembler := TSlackASM.Create() do
  try
    code += _push(eax);                   // for cleanup as we touch these!
    code += _push(ebx);       
  
    code += _mov(imm(0), ebx);            // move 0 to %ebx (it's kept in %ebx, as the loop counter)
    var lbl1 := Location;                 // make a label so we can jump here
    //loop body -->
    code += _mov(mem(x), eax);            // move `x` to %eax
    code += _add(mem(y), eax);            // add `y` to %eax
    code += _imul(eax);                   // imul %eax         [EAX *= EAX]
    code += _sub(mem(y), eax);            // %eax - `y`
    code += _mov(eax, mem(z));            // move %eax to `z`
    code += _inc(ebx);                    // inc %ebx          [increase our counter]
    code += _cmp(mem(lim), ebx);          // compare `lim` to %ebx
    code += _jle(RelLoc(lbl1));           // if %ebx <= lim then goto lbl1
    //<-- loop end
    code += _mov(ebx, mem(i));            // move %ebx to `i`

    code += _pop(ebx);                    // recover ebx, eax
    code += _pop(eax); 
    code += _ret;                         // return

    Method := Finalize();                 // create a function for us to call
  finally
  //WriteLn(Code);                        // (print the machinecode we produced)
    Free();                               // resets it
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
