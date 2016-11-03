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
    xencall_handle *callh;
    xenforeignmemory_handle *fmemh;
    struct elf_dyn_info edi;
    // The offset of the loadable segment in the ELF file...
    uint64_t offset;
    // ... and the virtual address of that offset.
    uint64_t baseaddr;
  };

int get_vcpu_context(struct UXEN_info *ui, vcpu_guest_context_t *vc);
int get_word_size(struct UXEN_info *ui);
void get_ptload_offset(struct UXEN_info *ui);

#endif /* _UXEN_internal */
