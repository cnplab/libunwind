#include <stdio.h>
#include "_UXEN_internal.h"

int
_UXEN_access_reg (unw_addr_space_t as, unw_regnum_t regnum, unw_word_t * valp, int write, void *arg)
{
  struct UXEN_info *ui = arg;
  vcpu_guest_context_t ctx;

  Debug (16, "as=%p, regnum=%u, valp=%p, write=%u, arg=%p\n", as, regnum, valp, write, arg);
  if (UWXEN_get_vcpu_context (ui, &ctx))
    return -UNW_EINVAL;
  switch (regnum)
    {
    case 6:
      Debug (6, "returning RBP=0x%lx\n", ctx.user_regs.rbp);
      *valp = ctx.user_regs.rbp;
      return 0;
    case 7:
      Debug (6, "returning RSP=0x%lx\n", ctx.user_regs.rsp);
      *valp = ctx.user_regs.rsp;
      return 0;
    case 16:
      Debug (6, "returning RIP=0x%lx\n", ctx.user_regs.rip);
      *valp = ctx.user_regs.rip;
      return 0;
    default:
      return -UNW_EBADREG;
    }
}
