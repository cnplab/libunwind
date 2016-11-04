#include "_UXEN_internal.h"

/* We only carry this information in ui because we need access
 * to it from generic libunwind functions. There is nothing magic
 * about it that would prevent us from changing it at will. So
 * this helper function allows us to change it without exposing
 * UXEN_info to every application that uses libunwind-xen.
 */
void
_UXEN_change_vcpu (struct UXEN_info *ui, int vcpu)
{
  ui->vcpu = vcpu;
}
