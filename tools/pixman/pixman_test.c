#include <awtk.h>
#include "tkc/mem.h"
#include "utils.inc"

int main(void) {
  bitmap_t img;
  pixman_color_t c = {.red = 0xee00, .green = 0xdd00, .blue = 0xbb00, .alpha = 0xffff};
  bitmap_t* dst = bitmap_create_ex(400, 400, 0, BITMAP_FMT_RGBA8888);
  pixman_image_t* d = to_pixman_image(dst);
  pixman_image_t* s = pixman_image_create_solid_fill(&c);
  color_t* p = (color_t*)(dst->data);

  pixman_image_composite(PIXMAN_OP_OVER, s, NULL, d, 0, 0, 0, 0, 0, 0, dst->w, dst->h);

  printf("%08x r=%02x g=%02x b=%02x a=%02x\n", p->color, p->rgba.r, p->rgba.g, p->rgba.b,
         p->rgba.a);

  write_image("/Users/jim/Downloads/test.png", dst);

  pixman_image_unref(d);
  bitmap_destroy(dst);

  return 0;
}
