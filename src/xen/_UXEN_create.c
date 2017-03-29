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

#include <string.h>
#include "_UXEN_internal.h"

void *
_UXEN_create (int domid, int vcpu, char *fname)
{
  struct UXEN_info *ui = malloc (sizeof (struct UXEN_info));

  Debug (16, "domid=%d, vcpu=%d, fname=%s\n", domid, vcpu, fname);
  if (!ui)
    return NULL;

  memset (ui, 0, sizeof (*ui));
  ui->domid = domid;
  ui->vcpu = vcpu;
  ui->fname = strdup (fname);
  ui->edi.di_cache.format = -1;
  ui->edi.di_debug.format = -1;
  ui->callh = xencall_open (NULL, XENCALL_OPENFLAG_NON_REENTRANT);
  if (ui->callh == NULL)
    {
      Debug (0, "libunwind:_UXEN_create: xencall handle is NULL!\n");
      return NULL;
    }
  ui->fmemh = xenforeignmemory_open (NULL, 0);
  if (ui->fmemh == NULL)
    {
      Debug (0, "libunwind:_UXEN_create: xenforeignmemory handle is NULL!\n");
      goto close_callh;
    }
  ui->wordsize = UWXEN_get_word_size (ui);
  Debug (15, "domid %d has word size %u\n", ui->domid, ui->wordsize);
  if (elf_map_image (&ui->edi.ei, ui->fname))
    {
      Debug (0, "libunwind:_UXEN_create: could not read ELF file %s!\n", ui->fname);
      goto close_fmemh;
    }
  Debug (15, "mapped ELF %s (%zu bytes)\n", ui->fname, ui->edi.ei.size);
  UWXEN_get_ptload_offset (ui);
  Debug (15, "PT_LOAD segment: physical offset 0x%lx, base addr 0x%lx\n", ui->offset, ui->baseaddr);

  return ui;

close_fmemh:
  xenforeignmemory_close (ui->fmemh);
close_callh:
  xencall_close (ui->callh);
  return NULL;
}
