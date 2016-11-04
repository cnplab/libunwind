#ifndef libunwind_xen_h
#define libunwind_xen_h

#include <libunwind.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

struct UXEN_info;

extern void *_UXEN_create (int domid, int vcpu, char *fname);
extern void _UXEN_destroy (void *);
extern int _UXEN_find_proc_info (unw_addr_space_t, unw_word_t,
                                 unw_proc_info_t *, int, void *);
extern void _UXEN_put_unwind_info (unw_addr_space_t, unw_proc_info_t *, void *);
extern int _UXEN_get_dyn_info_list_addr (unw_addr_space_t, unw_word_t *,
                                         void *);
extern int _UXEN_access_mem (unw_addr_space_t, unw_word_t, unw_word_t *, int,
                             void *);
extern int _UXEN_access_reg (unw_addr_space_t, unw_regnum_t, unw_word_t *,
                             int, void *);
extern int _UXEN_access_fpreg (unw_addr_space_t, unw_regnum_t, unw_fpreg_t *,
                               int, void *);
extern int _UXEN_get_proc_name (unw_addr_space_t, unw_word_t, char *, size_t,
                                unw_word_t *, void *);
extern int _UXEN_resume (unw_addr_space_t, unw_cursor_t *, void *);
extern void _UXEN_change_vcpu(struct UXEN_info *ui, int vcpu);
extern unw_accessors_t _UXEN_accessors;


#if defined(__cplusplus) || defined(c_plusplus)
}
#endif

#endif /* libunwind_ptrace_h */
