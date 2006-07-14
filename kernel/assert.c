#include <assert.h>
#include <system.h>
#include <video.h>

void
_assert(const char *file, int lineno, const char *e)
{
    printf("assertion \"%s\" failed.\nfile: %s, line %d\n",
            e, file, lineno);
    halt();
}
