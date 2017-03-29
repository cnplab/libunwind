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

#include "_UXEN_internal.h"

void
_UXEN_put_unwind_info (unw_addr_space_t as, unw_proc_info_t * pip, void *arg)
{
  Debug (0, "_UXEN_put_unwind_info()\n");
  Debug (16, "  as=%p, pip=%p, arg=%p\n", as, pip, arg);
  Debug (0, "  not implemented.\n");
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
