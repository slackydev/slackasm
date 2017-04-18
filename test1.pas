program test1;
{$DEFINE DEBUG}
{$I slackasm/asm.pas}
{$X+}

var
  assembler: TSlackASM;
  Method: external procedure();
var
  x: Int32   = 2;
  y: Int32   = 100;
  z,i: Int32 = 0;
  lim: Int32 = 10000000;
  t: Int64;
begin
  with assembler := TSlackASM.Create(2 shl 11) do
  try
    var myLabel := Location;     // set mylabel here
    _addl (@x,   @y );           // y -> EAX; add x, EAX. {does not write to y}
    _imull(EAX      );           // imul eax, eax
    _clqd;                       // clqd
    _idivl(@y       );           // EAX div y
    _movl (EAX,  @z );           // EAX -> z
    _incl (@i       );           // inc i
    _movl (@i,   ECX);           // i -> ECX
    _cmpl (@lim, ECX);           // cmp lim ecx
    _jle  (Rel(myLabel));        // if lim <= ecx then goto myLabel
    Method := Finalize();        // "Build" the method
  finally
    Free(False);
  end;

  Write('Lape ASM : ');
  t := GetTickCount();
  try
    Method();
  finally
    FreeMethod(@method);
  end;
  WriteLn(Format('Used %4d ms', [GetTickCount()-t]),': ',  [x, y, z, i]);


  //---- Pure lape code that does the same
  Write('Pure Lape: ');
  t := GetTickCount();
  {-->} for i:=0 to lim do z := Sqr(x+y) div y;
  WriteLn(Format('Used %4d ms', [GetTickCount()-t]),': ',  [x, y, z, i]);
end.
