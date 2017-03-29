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
