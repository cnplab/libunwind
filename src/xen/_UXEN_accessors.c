#include "_UXEN_internal.h"

void
_UXEN_put_unwind_info (unw_addr_space_t as, unw_proc_info_t * pip, void *arg)
{
  printf ("_UXEN_put_unwind_info()\n");
  printf ("  as=%p, pip=%p, arg=%p\n", as, pip, arg);
  printf ("  not implemented.\n");
}

int
_UXEN_get_dyn_info_list_addr (unw_addr_space_t as, unw_word_t * dilap, void *arg)
{
  Debug (16, "as=%p, dilap=%p, void=%p\n", as, dilap, arg);
  /* dynamic unwinding lists are some crazy shit. We don't need it, so tell libunwind. */
  Debug (1, "not implemented.\n");
  return -UNW_ENOINFO;
}

int
_UXEN_access_fpreg (unw_addr_space_t as, unw_regnum_t regnum, unw_fpreg_t * fpvalp, int write,
                    void *arg)
{
  Debug (16, "as=%p, regnum=%u, fpvalp=%p, write=%u, arg=%p\n", as, regnum, fpvalp, write, arg);
  Debug (1, "not implemented!\n");
  return -UNW_EINVAL;
}

int
_UXEN_resume (unw_addr_space_t as, unw_cursor_t * cp, void *arg)
{
  Debug (16, "as=%p, cp=%p, arg=%p\n", as, cp, arg);
  Debug (1, "not implemented!\n");
  return -UNW_EINVAL;
}

PROTECTED unw_accessors_t _UXEN_accessors = {
  .find_proc_info = _UXEN_find_proc_info,
  .put_unwind_info = _UXEN_put_unwind_info,
  .get_dyn_info_list_addr = _UXEN_get_dyn_info_list_addr,
  .access_mem = _UXEN_access_mem,
  .access_reg = _UXEN_access_reg,
  .access_fpreg = _UXEN_access_fpreg,
  .resume = _UXEN_resume,
  .get_proc_name = _UXEN_get_proc_name
};
