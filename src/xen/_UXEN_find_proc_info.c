#include <stdio.h>
#include "_UXEN_internal.h"

/* adapted from src/ptrace/_UPT_find_proc_info.c */
static int
get_unwind_info (struct UXEN_info *ui, unw_addr_space_t as, unw_word_t ip)
{
  struct elf_dyn_info *edi = &ui->edi;

#if UNW_TARGET_IA64 && defined(__linux)
  if (!edi->ktab.start_ip && _Uia64_get_kernel_table (&edi->ktab) < 0)
    return -UNW_ENOINFO;

  if (edi->ktab.format != -1 && ip >= edi->ktab.start_ip && ip < edi->ktab.end_ip)
    return 0;
#endif

  if ((edi->di_cache.format != -1 && ip >= edi->di_cache.start_ip && ip < edi->di_cache.end_ip)
#if UNW_TARGET_ARM
      || (edi->di_debug.format != -1 && ip >= edi->di_arm.start_ip && ip < edi->di_arm.end_ip)
#endif
      || (edi->di_debug.format != -1 && ip >= edi->di_debug.start_ip && ip < edi->di_debug.end_ip))
    return 0;

  if (tdep_find_unwind_table (edi, as, ui->fname, ui->baseaddr, ui->offset, ip) < 0)
    {
      printf ("could not find unwind table!\n");
      return -UNW_ENOINFO;
    }

  /* This can happen in corner cases where dynamically generated
     code falls into the same page that contains the data-segment
     and the page-offset of the code is within the first page of
     the executable.  */
  if (edi->di_cache.format != -1 && (ip < edi->di_cache.start_ip || ip >= edi->di_cache.end_ip))
    edi->di_cache.format = -1;

  if (edi->di_debug.format != -1 && (ip < edi->di_debug.start_ip || ip >= edi->di_debug.end_ip))
    edi->di_debug.format = -1;

  if (edi->di_cache.format == -1
#if UNW_TARGET_ARM
      && edi->di_arm.format == -1
#endif
      && edi->di_debug.format == -1)
    {
      printf ("Ended up with neither di_cache no di_debug!\n");
      return -UNW_ENOINFO;
    }

  return 0;
}

/* END adapted from src/ptrace/_UPT_find_proc_info.c */

int
_UXEN_find_proc_info (unw_addr_space_t as, unw_word_t ip, unw_proc_info_t * pip,
                      int need_unwind_info, void *arg)
{
  struct UXEN_info *ui = arg;
  int ret = -UNW_ENOINFO;

  Debug (16, "as=%p, ip=0x%lx, pip=%p, need_unwind_info=%d, arg=%p\n",
         as, ip, pip, need_unwind_info, arg);
  if (get_unwind_info (ui, as, ip) < 0)
    {
      printf ("could not get unwind info!\n");
      return -UNW_ENOINFO;
    }

  if (ret == -UNW_ENOINFO && ui->edi.di_cache.format != -1)
    ret = tdep_search_unwind_table (as, ip, &ui->edi.di_cache, pip, need_unwind_info, arg);
  Debug (2, "searching di_cache retval %d\n", ret);

  if (ret == -UNW_ENOINFO && ui->edi.di_debug.format != -1)
    ret = tdep_search_unwind_table (as, ip, &ui->edi.di_debug, pip, need_unwind_info, arg);
  Debug (2, "searching di_debug retval %d\n", ret);

  return ret;
}
