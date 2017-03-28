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
