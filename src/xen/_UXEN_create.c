#include <string.h>
#include "_UXEN_internal.h"

void *
_UXEN_create (int domid, int vcpu, char *fname)
{
  struct UXEN_info *ui = malloc (sizeof (struct UXEN_info));

  if (!ui)
    return NULL;

  memset (ui, 0, sizeof (*ui));
  ui->domid = domid;
  ui->vcpu = vcpu;
  ui->fname = fname;
  ui->edi.di_cache.format = -1;
  ui->edi.di_debug.format = -1;
  ui->callh = xencall_open (NULL, XENCALL_OPENFLAG_NON_REENTRANT);
  if (ui->callh == NULL)
    {
      printf ("xencall handle is NULL!\n");
      return NULL;
    }
  ui->fmemh = xenforeignmemory_open (NULL, 0);
  if (ui->fmemh == NULL)
    {
      printf ("xenforeignmemory handle is NULL!\n");
      return NULL;
    }
  return ui;
}
