program mul_func;
{------------------------------------------------------------------------------]
 Shows what instructions we currently lack, as well as how to implement a native
 cdecl function in lape itself, using lapes "special" argument passing with
 this assembler.
[------------------------------------------------------------------------------}
{$I slackasm/assembler.pas}
{$X+}

{------------------------------------------------------------------------------]
 Implementing the following native method:
  ,----------------------------------------------------------.
  | procedure Mul(const P: PParamArray; const Res: Pointer); |
  | begin                                                    |
  |   PInt32(Res)^ := PInt32(P^[0])^ * PInt32(P^[1])^;       |
  | end;                                                     |
  `----------------------------------------------------------´
 Lape implements argument passing as just a pointer to it's stack at the current
 offset -argcount or something, that's the whole PParamArray deal.
 While the result is just a direct pointer to the output variable.
[------------------------------------------------------------------------------}
function MulFunc(): Pointer;
var
  assembler: TSlackASM;

  //we have no methods for working with the stack, and shit that relates to it
  procedure push_ebp();           begin assembler.code += ToBytes([$55]); end;
  procedure pop_ebp();            begin assembler.code += ToBytes([$5D]); end;
  procedure mov_ebpx_eax(x: Byte) begin assembler.code += ToBytes([$8B,$45,x]); end;
  procedure mov_ebpx_ecx(x: Byte) begin assembler.code += ToBytes([$8B,$4D,x]); end;
  procedure mov_ebpx_edx(x: Byte) begin assembler.code += ToBytes([$8B,$55,x]); end;
  procedure mov_ebpx_ebx(x: Byte) begin assembler.code += ToBytes([$8B,$5D,x]); end;

begin
  with assembler := TSlackASM.Create(2 shl 11) do
  try
    // prologue
    push_ebp;
    code += _mov(esp,ebp);

    // load real args
    mov_ebpx_edx(12);                    //resvar (Pointer)
    mov_ebpx_ebx(08);                    //argz (ptr to array [WORD] of Pointer)

    // load lape args
    code += _mov(ref(ebx), eax);         // deref
    code += _mov(ref(eax), eax);         //mov contents of first arg to eax

    code += _add(imm(4),   ebx);         //offset by 1
    code += _mov(ref(ebx), ecx);         // deref
    code += _mov(ref(ecx), ecx);         //mov contents of second arg to ecx

    // implementation
    code += _imul(ECX, EAX);

    //result
    code += _mov(EAX, ref(EDX));

    // epilogue
    pop_ebp;
    code += _ret;

    Result := Finalize();
  finally
    WriteLn(Code);
    Free(False);
  end;
end;

var
  f: external function(x,y:Int32): Int32;
begin
  f := MulFunc();

  WriteLn f(20,5);

  FreeMethod(@f);
end.
