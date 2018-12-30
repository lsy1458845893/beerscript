

#include "memory-inspector.h"
#include "bsctx.h"
#include "bsstring.h"
#include <stdio.h>

int main(int argc, char const *argv[]) {
  memory_inspector_t inspector = {0, 0, 0};
  bsctx_t ctx;
  bsctx_init(&ctx, memory_inspector_alloc, memory_inspector_cleanup, (void *)&inspector);

  bsstring_t *a = bsstring_create_c_const(&ctx, "test");
  bsstring_t *b = bsstring_create_c_buffer(&ctx, "test");
  bsstring_t *c = bsstring_create_c_const(&ctx, "test");

  bsstring_unrefer(&ctx, b);
  bsstring_refer(&ctx, a);
  bsstring_unrefer(&ctx, a);
  bsstring_unrefer(&ctx, c);
  bsstring_unrefer(&ctx, a);

  bsctx_deinit(&ctx);
  return 0;
}
