/* We need to get a separate copy of the ELF-code into
   libunwind-xen since it cannot (and must not) have any ELF
   dependencies on libunwind.  */
#include "libunwind_i.h"        /* get ELFCLASS defined */
#include "_UXEN_internal.h"
#include "../elfxx.c"

void
get_ptload_offset (struct UXEN_info *ui)
{
  Elf_W (Ehdr) * ehdr;
  Elf_W (Phdr) * phdr;
  int i;
  int seen_loadable = 0;
  void *image = ui->edi.ei.image;

  ehdr = image;
  phdr = (Elf_W (Phdr) *) (image + ehdr->e_phoff);

  for (i = 0; i < ehdr->e_phnum; i++)
    {
      if (phdr[i].p_type == PT_LOAD)
        {
          if (seen_loadable)
            {
              printf ("warning: found more than one lodable segment in ELF file. Ignoring all but the first!\n");
              printf ("This will probably lead to breakage if your code spans more than that first segment.\n");
              continue;
            }
          seen_loadable = 1;
          ui->offset = phdr[i].p_offset;
          // or should this be vaddr?
          ui->baseaddr = phdr[i].p_paddr;
        }
    }
}
