#ifndef _UXEN_internal
#define _UXEN_internal

#include "libunwind_i.h"
#include "libunwind-xen.h"
#include "xenctrl.h"
#include "xencall.h"
#include "xenforeignmemory.h"

struct UXEN_info
  {
    int domid;
    int vcpu;
    char *fname;
    struct elf_dyn_info edi;
    xencall_handle *callh;
    xenforeignmemory_handle *fmemh;
  };

int get_vcpu_context(struct UXEN_info *ui, vcpu_guest_context_t *vc);
int get_word_size(struct UXEN_info *ui);

#endif /* _UXEN_internal */
