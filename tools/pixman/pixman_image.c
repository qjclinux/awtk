#include <awtk.h>
#include "tkc/mem.h"
#include <pixman.h>
#include "utils.inc"

int main(void) {
  matrix_t m;
  bitmap_t img;
  draw_image_info_t info;
  bitmap_t* dst = bitmap_create_ex(400, 400, 0, BITMAP_FMT_RGBA8888);

  bitmap_set_line_length(dst, 0);

  load_image("tools/pixman/cn.png", &img);
  matrix_init(&m);

  matrix_scale(&m, 2, 2);
  matrix_translate(&m, 20, 20);
  matrix_rotate(&m, 0.1);

  info.img = &img;
  info.matrix = m;
  info.clip = rect_init(0, 0, 100, 100);

  pixman_draw_image_matrix(dst, &info);
  write_image("/Users/jim/Downloads/test.png", dst);

  bitmap_destroy(&img);
  bitmap_destroy(dst);

  return 0;
}

#define D2F(d) pixman_double_to_fixed(d)

void transform_test(void) {
  matrix_t m;
  pixman_transform_t t;
  pixman_transform_t t1;

  matrix_init(&m);
  matrix_scale(&m, 2, 3);
  pixman_transform_init_scale(&t, D2F(2), D2F(3));
  to_pixman_transfrom(&t1, &m);

  matrix_init(&m);
  matrix_translate(&m, 2, 3);
  pixman_transform_init_translate(&t, D2F(2), D2F(3));
  to_pixman_transfrom(&t1, &m);

  matrix_init(&m);
  matrix_rotate(&m, 2);
  pixman_transform_init_rotate(&t, D2F(cos(2)), D2F(sin(2)));
  to_pixman_transfrom(&t1, &m);

  matrix_init(&m);
}

int main1(void) {
  bitmap_t img;
  bitmap_t* dst = bitmap_create_ex(400, 400, 0, BITMAP_FMT_RGBA8888);
  load_image("tools/pixman/cn.png", &img);
  pixman_image_t* s = to_pixman_image(&img);
  pixman_image_t* d = to_pixman_image(dst);
  pixman_transform_t transform;

  matrix_t m;
  matrix_init(&m);

  matrix_scale(&m, 2, 2);
  matrix_translate(&m, 20, 20);
  matrix_rotate(&m, 0.1);
  to_pixman_transfrom(&transform, &m);

  /*
  pixman_fixed_t cc = pixman_double_to_fixed(cos(0.1f));
  pixman_fixed_t ss = pixman_double_to_fixed(sin(0.1f));
  pixman_fixed_t scale_x = pixman_double_to_fixed(2.0f);
  pixman_fixed_t scale_y = pixman_double_to_fixed(2.0f);
  pixman_fixed_t translate_x = pixman_double_to_fixed(20.0f);
  pixman_fixed_t translate_y = pixman_double_to_fixed(20.0f);

  pixman_transform_init_scale (&transform, scale_x, scale_y);
  pixman_transform_translate (&transform, NULL, translate_x, translate_y);
  pixman_transform_rotate(&transform, NULL, cc, ss);

  pixman_transform_invert(&transform, &transform);
  */
  pixman_transform_invert(&transform, &transform);
  pixman_image_set_transform(s, &transform);

  pixman_image_composite(PIXMAN_OP_OVER, s, NULL, d, 0, 0, 0, 0, 0, 0, dst->w, dst->h);

  write_image("/Users/jim/Downloads/test.png", dst);

  pixman_image_unref(s);
  pixman_image_unref(d);
  bitmap_destroy(&img);
  bitmap_destroy(dst);

  transform_test();

  return 0;
}
