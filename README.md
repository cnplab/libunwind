Libunwind with Patches for Xen Unikernel Unwinding
==================================================

(Note: the original, upstream readme can be found in README.)

This is a fork of libunwind (http://www.nongnu.org/libunwind/), a library to
unwind the stack and create a callchain. This fork is extended by support for
Xen virtual machines. This is especially useful for Unikernels, in which the
kernel and application form a single binary sharing a common address space. By
using libunwind-xen, a unikernel's (or, in theory, any other VM's) stack can
be unwound from the outside. One of the main uses for this is the sampling of
call chains at runtime as a profiling tool.

The way libunwind does this is by using the `.eh_frame information` created by a
compiler at compile time. By comparing the stack addresses in the running VM
to those in the ELF's `.eh_frame section`, it can assess the size of the
currently running function's frame on the stack, unroll it to retrieve the
prveious instruction pointer, and iterate over this until the end of the stack
is reached. This means that you need compiler support to create an `.eh_frame`,
but the good news is that this is almost universally supported out-of-the-box
and by default. It also means that you do not need a frame pointer, so you can
produce stack traces on binaries compiles with `-fomit-frame-pointer` for higher
performance, or on architectures that do not provide a frame pointer register.

This current implementation is quite bare-bones and does not support
functionality that isn't required for stack walking. For example, there is no
support for writing memory or registers, only reading. It currently also only
supports the x86 version of Xen. Extensions of this starting point should be
easy though, if more is required at any point in the future.


# Build and Install

1. ./autogen.sh
2. ./configure
3. make
4. make install


# Use

For tracing Xen virtual machines, link your application against
libunwind-generic and libunwind-xen.
