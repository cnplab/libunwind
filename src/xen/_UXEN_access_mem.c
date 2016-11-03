#include <stdio.h>
#include <sys/user.h>
#include "_UXEN_internal.h"

typedef struct mapped_page
  {
    unw_word_t base;            // page number, i.e. addr>>PAGE_SHIFT
    unsigned long mfn;
    void *buf;
    struct mapped_page *next;
  }
mapped_page_t;

static unsigned long
xen_translate_foreign_address (struct UXEN_info *ui, unsigned long long virt)
{
  vcpu_guest_context_t ctx;
  int levels;
  int i;
  uint64_t addr, mask, clamp, offset;
  xen_pfn_t pfn;
  void *map;
  int wordsize;

  get_vcpu_context (ui, &ctx);
  wordsize = get_word_size (ui);

  if (wordsize == 8)
    {
      /* 64-bit has a 4-level page table */
      levels = 4;
      /* clamp values to 48 bit virtual address range */
      clamp = (1ULL << 48) - 1;
      addr = (uint64_t) xen_cr3_to_pfn_x86_64 (ctx.ctrlreg[3]) << PAGE_SHIFT;
      addr &= clamp;
    }
  else
    { /* wordsize == 4, any weird other values throw and error much earlier */
      /* 32-bit has a 3-level page table */
      levels = 3;
      /* clamp value to 32 bit address range */
      clamp = (1ULL << 32) - 1;
      addr = (uint32_t) xen_cr3_to_pfn_x86_32 (ctx.ctrlreg[3]) << PAGE_SHIFT;
    }
  Debug (15, "page table base address is 0x%lx\n", addr);
  /* See AMD64 Architecture Programmer's Manual, Volume 2: System Programming,
   * rev 3.22, p. 127, Fig. 5-9 for 32-bit and p, 132, Fig. 5-17 for 64-bit. */
  /* Each page table considers a 9-bit range.
   * The lowest level considers bits 12-21 (hence the <<12 shift),
   * each higher level the next-significant 9 bits.
   * Note that the highest level is truncated and only considers
   * 2 bits for 32-bit archictures. */
  mask = ((((1ULL << 9) - 1) << 12) << ((levels - 1) * 9));

  for (i = levels; i > 0; i--)
    {
      /* See AMD64 Architecture Programmer's Manual, Volume 2: System
       * Programming, rev 3.22, p. 128, Figs. 5-10 to 5-12 for 32-bit
       * and p, 133, Figs. 5-18 to 5-21 for 64-bit. */
      /* Take the respective bits for the level, shift them to the
       * very right, and interpret them as a "page table entry number".
       * Since PTEs are 8 bytes for both 64-bit and 32-bit (Xen doesn't
       * seem to emulate legacy non-PAE setups with 4-byte PTEs),
       * multiply the page table entry number by 8. */
      offset = ((virt & mask) >> (ffsll (mask) - 1)) * 8;
      Debug (15, "level %d page walk gives us offset 0x%lx\n", i, offset);
      /* But before we can read from there, we will need to map in that memory */
      pfn = addr >> PAGE_SHIFT;
      map = xenforeignmemory_map (ui->fmemh, ui->domid, PROT_READ, 1, &pfn, NULL);
      memcpy (&addr, map + offset, 8);
      xenforeignmemory_unmap (ui->fmemh, map, 1);
      /* However, addr is not really an address right now, but rather a PTE,
       * which contains the base address in bits 51..12. No shifting necessary,
       * because the base address in the PTE is a PFN. */
      addr &= 0x000FFFFFFFFFF000ULL;
      Debug (15, "level %d page table tells us to look at address 0x%lx\n", i, addr);
      /* Move the mask by 9 bits, and go on to the next round */
      mask >>= 9;
    }
  /* We now have the machine addres. But actually, we want an
   * MFN, so shift the address accordingly. */
  addr >>= PAGE_SHIFT;
  Debug (15, "found section entry for %llx to mfn 0x%lx\n", virt, addr);
  xenforeignmemory_unmap (ui->fmemh, map, 1);
  return addr;
}

static void
xen_map_domu_page (struct UXEN_info *ui, uint64_t addr, unsigned long *mfn, void **buf)
{
  int err = 0;
  *mfn = xen_translate_foreign_address (ui, addr);
  // This works since size is 1, so the array has size 1, so it's just a pointer to an int
  *buf = xenforeignmemory_map (ui->fmemh, ui->domid, PROT_READ, 1, (xen_pfn_t *) mfn, &err);
}

static void *
guest_to_host (struct UXEN_info *ui, unw_word_t gaddr)
{
  static mapped_page_t *map_head = NULL;
  mapped_page_t *map_iter;
  mapped_page_t *new_item;
  unw_word_t base = gaddr & PAGE_MASK;
  unw_word_t offset = gaddr & ~PAGE_MASK;

  map_iter = map_head;
  while (map_iter != NULL)
    {
      if (base == map_iter->base)
        return map_iter->buf + offset;
      // preserve last item in map_iter by jumping out
      if (map_iter->next == NULL)
        break;
      map_iter = map_iter->next;
    }

  // no matching page found, we need to map a new one.
  // At this pointer, map_iter conveniently points to the last item.
  new_item = malloc (sizeof (mapped_page_t));
  if (new_item == NULL)
    {
      fprintf (stderr, "failed to allocate memory for page struct.\n");
      return NULL;
    }
  new_item->base = base;
  xen_map_domu_page (ui, base, &new_item->mfn, &new_item->buf);
  Debug (1, "mapping new page %#" PRIx64 "->%p\n", new_item->base, new_item->buf);
  if (new_item->buf == NULL)
    {
      fprintf (stderr, "failed to allocate memory mapping page.\n");
      return NULL;
    }
  new_item->next = NULL;
  if (map_head == NULL)
    map_head = new_item;
  else
    map_iter->next = new_item;
  return new_item->buf + offset;
}

int
_UXEN_access_mem (unw_addr_space_t as, unw_word_t addr, unw_word_t * valp, int write, void *arg)
{
  struct UXEN_info *ui = arg;
  void *xenp;

  Debug (16, "as=%p, addr=0x%lx, valp=0x%p, write=%d, arg=%p\n", as, addr, valp, write, arg);
  xenp = guest_to_host (ui, addr);
  memcpy (valp, xenp, sizeof (unw_word_t));
  Debug (15, "xen translation: 0x%lx->0x%p, content 0x%lx\n", addr, xenp, *valp);
  return 0;
}
