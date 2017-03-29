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
