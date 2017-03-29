/* libunwind - a platform-independent unwind library
   Copyright (c) 2017 NEC Europe Ltd., NEC Corporation
        Contributed by Florian Schmidt <florian.schmidt@neclab.eu>

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include <stdio.h>
#include "_UXEN_internal.h"

#define RETURN_REGISTER(regname)                                        \
do {                                                                    \
  Debug(6, "returning %s=0x%lx\n", #regname, ctx->user_regs.regname);   \
  *valp = ctx->user_regs.regname;                                       \
  return 0;                                                             \
} while(0)

#if defined(__i386__)
int
access_reg_x86 (vcpu_guest_context_t * ctx, unw_regnum_t regnum, unw_word_t * valp)
{
  switch (regnum)
    {
    case UNW_X86_EAX:
      RETURN_REGISTER (eax);
    case UNW_X86_EDX:
      RETURN_REGISTER (edx);
    case UNW_X86_ECX:
      RETURN_REGISTER (ecx);
    case UNW_X86_EBX:
      RETURN_REGISTER (ebx);
    case UNW_X86_ESI:
      RETURN_REGISTER (esi);
    case UNW_X86_EDI:
      RETURN_REGISTER (edi);
    case UNW_X86_EBP:
      RETURN_REGISTER (ebp);
    case UNW_X86_ESP:
      RETURN_REGISTER (esp);
    case UNW_X86_EIP:
      RETURN_REGISTER (eip);
    case UNW_X86_EFLAGS:
      RETURN_REGISTER (eflags);
    case UNW_X86_GS:
      RETURN_REGISTER (gs);
    case UNW_X86_FS:
      RETURN_REGISTER (fs);
    case UNW_X86_ES:
      RETURN_REGISTER (es);
    case UNW_X86_DS:
      RETURN_REGISTER (ds);
    case UNW_X86_SS:
      RETURN_REGISTER (ss);
    case UNW_X86_CS:
      RETURN_REGISTER (cs);
    default:
      return -UNW_EBADREG;
    }
}
#endif /* i386 */

#if defined(__x86_64__)
unw_word_t
access_reg_x86_64 (vcpu_guest_context_t * ctx, unw_regnum_t regnum, unw_word_t * valp)
{
  switch (regnum)
    {
    case UNW_X86_64_RAX:
      RETURN_REGISTER (rax);
    case UNW_X86_64_RDX:
      RETURN_REGISTER (rdx);
    case UNW_X86_64_RCX:
      RETURN_REGISTER (rcx);
    case UNW_X86_64_RBX:
      RETURN_REGISTER (rbx);
    case UNW_X86_64_RSI:
      RETURN_REGISTER (rsi);
    case UNW_X86_64_RDI:
      RETURN_REGISTER (rdi);
    case UNW_X86_64_RBP:
      RETURN_REGISTER (rbp);
    case UNW_X86_64_RSP:
      RETURN_REGISTER (rsp);
    case UNW_X86_64_R8:
      RETURN_REGISTER (r8);
    case UNW_X86_64_R9:
      RETURN_REGISTER (r9);
    case UNW_X86_64_R10:
      RETURN_REGISTER (r10);
    case UNW_X86_64_R11:
      RETURN_REGISTER (r11);
    case UNW_X86_64_R12:
      RETURN_REGISTER (r12);
    case UNW_X86_64_R13:
      RETURN_REGISTER (r13);
    case UNW_X86_64_R14:
      RETURN_REGISTER (r14);
    case UNW_X86_64_R15:
      RETURN_REGISTER (r15);
    case UNW_X86_64_RIP:
      RETURN_REGISTER (rip);
    default:
      return -UNW_EBADREG;
    }
}
#endif /* x86_64 */

int
_UXEN_access_reg (unw_addr_space_t as, unw_regnum_t regnum, unw_word_t * valp, int write, void *arg)
{
  struct UXEN_info *ui = arg;
  vcpu_guest_context_t ctx;

  Debug (16, "as=%p, regnum=%u, valp=%p, write=%u, arg=%p\n", as, regnum, valp, write, arg);
  if (write)
    return -UNW_EINVAL;

  if (UWXEN_get_vcpu_context (ui, &ctx))
    return -UNW_EINVAL;
#if defined(__x86_64__)
  return access_reg_x86_64 (&ctx, regnum, valp);
#elif defined(__i386__)
  return access_reg_x86 (&ctx, regnum, valp);
#else
#warning "_UXEN_access_reg not implemented for this architecture!"
#endif
}
