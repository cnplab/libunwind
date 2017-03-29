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

int
_UXEN_get_proc_name (unw_addr_space_t as, unw_word_t addr, char *bufp, size_t buflen,
                     unw_word_t * offp, void *arg)
{
  struct UXEN_info *ui = arg;

  Debug (16, "as=%p, addr=0x%lx, bufp=%p, buflen=%lu, offp=%p, arg=%p\n",
         as, addr, bufp, buflen, offp, arg);

#if ELF_CLASS == ELFCLASS64
  return _Uelf64_get_proc_name_in_image (as, &ui->edi.ei, ui->baseaddr, ui->offset,
                                         addr, bufp, buflen, offp);
#elif ELF_CLASS == ELFCLASS32
  return _Uelf32_get_proc_name_in_image (as, &ui->edi.ei, ui->baseaddr, ui->offset,
                                         addr, bufp, buflen, offp);
#else
  return -UNW_ENOINFO;
#endif
}
