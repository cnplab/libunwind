#include "_UXEN_internal.h"

int
get_vcpu_context (struct UXEN_info *ui, vcpu_guest_context_t * vc)
{
  struct xen_domctl domctl;
  domctl.domain = (domid_t) ui->domid;
  domctl.interface_version = XEN_DOMCTL_INTERFACE_VERSION;
  domctl.cmd = XEN_DOMCTL_getvcpucontext;
  domctl.u.vcpucontext.vcpu = (uint16_t) ui->vcpu;
  domctl.u.vcpucontext.ctxt.p = vc;
  return xencall1 (ui->callh, __HYPERVISOR_domctl, (unsigned long) (&domctl));
}

int
get_word_size (struct UXEN_info *ui)
{
  struct xen_domctl domctl;
  domctl.domain = (domid_t) ui->domid;
  domctl.interface_version = XEN_DOMCTL_INTERFACE_VERSION;
  domctl.cmd = XEN_DOMCTL_get_address_size;
  if (xencall1 (ui->callh, __HYPERVISOR_domctl, (unsigned long) (&domctl)))
    return -1;
  return (domctl.u.address_size.size / 8);
}
