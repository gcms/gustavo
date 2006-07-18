#include <system.h>
#include <video.h>

extern void halt_impl(void);

void
halt(void)
{
    printf("Halted.");
    halt_impl();
}
