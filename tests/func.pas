program mul_func;
{------------------------------------------------------------------------------]
 Shows how to implement a native cdecl function within lape itself,
 using lapes "special" argument passing


 The goal of this example - Implement the following native method:
 -----------------------------------------------------------------
  | procedure Mul(const P: PParamArray; const Res: Pointer); cdecl;
  | begin
  |   PInt32(Res)^ := PInt32(P^[0])^ * PInt32(P^[1])^;
  | end;

 Lape implements argument passing as just a pointer to it's stack at the current
 offset -argcount or something, that's the whole PParamArray deal.
 While the result is just a direct pointer to the output variable.
[------------------------------------------------------------------------------}
{$I slackasm/assembler.pas}
{$X+}


function MulFunc(): Pointer;
var
  assembler: TSlackASM;
begin
  with assembler := TSlackASM.Create() do
  try
    // prologue
    code += _push(ebp);
    code += _mov(esp, ebp);

    // load real args
    code += _mov(ebp+12, edx);           // Result pointer
    code += _mov(ebp+08, ebx);           // argz (ptr to array [WORD] of Pointer)

    // load lape args
    code += _mov(ref(ebx), eax);         // deref ebx into eax
    code += _mov(ref(eax), eax);         // mov contents of first arg to eax

    code += _add(imm(4),   ebx);         // offset by size of pointer
    code += _mov(ref(ebx), ecx);         // deref ebx into eax
    code += _mov(ref(ecx), ecx);         // mov contents of second arg to ecx

    // implementation
    code += _imul(ecx, eax);

    // result
    code += _mov(eax, ref(edx));

    // epilogue
    code += _pop(ebp);
    code += _ret;

    Result := Finalize();
  finally
    WriteLn(Code);
    Free();
  end;
end;

var
  mul: external function(x,y:Int32): Int32;
begin
  mul := MulFunc();

  WriteLn mul(1000, 5);

  FreeMethod(@mul);
end.
