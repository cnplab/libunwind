#include "_UXEN_internal.h"

int
_UXEN_get_proc_name (unw_addr_space_t as, unw_word_t addr, char *bufp, size_t buflen,
                     unw_word_t * offp, void *arg)
{
  struct UXEN_info *ui = arg;

  Debug (16, "as=%p, addr=0x%lx, bufp=%p, buflen=%lu, offp=%p, arg=%p\n",
         as, addr, bufp, buflen, offp, arg);

  if (ui->edi.ei.image == NULL)
    if (elf_map_image (&ui->edi.ei, ui->fname))
      {
        printf ("could not read ELF file %s!\n", ui->fname);
        return -UNW_ENOINFO;
      }

  //TODO get segbase and offset from the ELF file
  return _Uelf64_get_proc_name_in_image (as, &ui->edi.ei, 0x1000, 0, addr, bufp, buflen, offp);
}
