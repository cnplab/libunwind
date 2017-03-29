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

/* We need to get a separate copy of the ELF-code into
   libunwind-xen since it cannot (and must not) have any ELF
   dependencies on libunwind.  */
#include "libunwind_i.h"        /* get ELFCLASS defined */
#include "_UXEN_internal.h"
#include "../elfxx.c"

void
UWXEN_get_ptload_offset (struct UXEN_info *ui)
{
  Elf_W (Ehdr) * ehdr;
  Elf_W (Phdr) * phdr;
  int i;
  int seen_loadable = 0;
  void *image = ui->edi.ei.image;
  ehdr = image;
  phdr = (Elf_W (Phdr) *) (image + ehdr->e_phoff);

  for (i = 0; i < ehdr->e_phnum; i++)
    {
      if (phdr[i].p_type == PT_LOAD)
        {
          if (seen_loadable)
            {
              Debug (0, "warning: found more than one lodable segment in ELF file. Ignoring all but the first!\n");
              Debug (0, "This will probably lead to breakage if your code spans more than that first segment.\n");
              continue;
            }
          seen_loadable = 1;
          ui->offset = phdr[i].p_offset;
          // or should this be vaddr?
          ui->baseaddr = phdr[i].p_paddr;
        }
    }
}
