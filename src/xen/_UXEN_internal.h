#ifndef _UXEN_internal
#define _UXEN_internal

#include "libunwind_i.h"
#include "libunwind-xen.h"
#include "xenctrl.h"
#include "xencall.h"
#include "xenforeignmemory.h"

#undef PAGE_SHIFT
#define PAGE_SHIFT XC_PAGE_SHIFT
#undef PAGE_SIZE
#define PAGE_SIZE  (1<<PAGE_SHIFT)
#undef PAGE_MASK
#define PAGE_MASK  (~(PAGE_SIZE-1))

struct UXEN_info
  {
    int domid;
    int vcpu;
    unsigned int wordsize;
    char *fname;
    xencall_handle *callh;
    xenforeignmemory_handle *fmemh;
    struct elf_dyn_info edi;
    // The offset of the loadable segment in the ELF file...
    uint64_t offset;
    // ... and the virtual address of that offset.
    uint64_t baseaddr;
  };

static inline int
UWXEN_get_vcpu_context(struct UXEN_info *ui, vcpu_guest_context_t *vc)
{
  struct xen_domctl domctl;
  domctl.domain = (domid_t)ui->domid;
  domctl.interface_version = XEN_DOMCTL_INTERFACE_VERSION;
  domctl.cmd = XEN_DOMCTL_getvcpucontext;
  domctl.u.vcpucontext.vcpu = (uint16_t)ui->vcpu;
  domctl.u.vcpucontext.ctxt.p = vc;
  Debug (16, "doing get_vcpu_context hypercall\n");
  return xencall1(ui->callh, __HYPERVISOR_domctl, (unsigned long)(&domctl));
}

static inline int
UWXEN_get_word_size(struct UXEN_info *ui)
{
  struct xen_domctl domctl;
  domctl.domain = (domid_t)ui->domid;
  domctl.interface_version = XEN_DOMCTL_INTERFACE_VERSION;
  domctl.cmd = XEN_DOMCTL_get_address_size;
  Debug (16, "doing get_address_size hypercall\n");
  if (xencall1(ui->callh, __HYPERVISOR_domctl, (unsigned long)(&domctl)))
    return -1;
  return (domctl.u.address_size.size / 8);
}

void UWXEN_get_ptload_offset(struct UXEN_info *ui);

#endif /* _UXEN_internal */
