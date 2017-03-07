#include "_UXEN_internal.h"

int
_UXEN_get_proc_name (unw_addr_space_t as, unw_word_t addr, char *bufp, size_t buflen,
                     unw_word_t * offp, void *arg)
{
  struct UXEN_info *ui = arg;

  Debug (16, "as=%p, addr=0x%lx, bufp=%p, buflen=%lu, offp=%p, arg=%p\n",
         as, addr, bufp, buflen, offp, arg);

#if ELF_CLASS == ELFCLASS64
  return _Uelf64_get_proc_name_in_image (as, &ui->edi.ei, ui->baseaddr, ui->offset,
                                         addr, bufp, buflen, offp);
#elif ELF_CLASS == ELFCLASS32
  return _Uelf32_get_proc_name_in_image (as, &ui->edi.ei, ui->baseaddr, ui->offset,
                                         addr, bufp, buflen, offp);
#else
  return -UNW_ENOINFO;
#endif
}
