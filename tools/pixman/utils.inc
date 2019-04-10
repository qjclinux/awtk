#include <pixman.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_FREE TKMEM_FREE
#define STBI_MALLOC TKMEM_ALLOC
#define STBI_REALLOC(p, s) TKMEM_REALLOC(p, s)

#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

ret_t to_pixman_transfrom(pixman_transform_t* t, matrix_t* m) {
  return_value_if_fail(t != NULL && m != NULL, RET_BAD_PARAMS);

  t->matrix[0][0] = pixman_double_to_fixed(m->a0);
  t->matrix[0][1] = pixman_double_to_fixed(m->a2);
  t->matrix[0][2] = pixman_double_to_fixed(m->a4);
  t->matrix[1][0] = pixman_double_to_fixed(m->a1);
  t->matrix[1][1] = pixman_double_to_fixed(m->a3);
  t->matrix[1][2] = pixman_double_to_fixed(m->a5);

  t->matrix[2][0] = 0;
  t->matrix[2][1] = 0;
  t->matrix[2][2] = pixman_double_to_fixed(1);

  return RET_OK;
}

pixman_format_code_t to_pixman_format(bitmap_format_t fmt) {
  switch (fmt) {
    case BITMAP_FMT_RGBA8888: {
      return PIXMAN_a8b8g8r8;
    }
    case BITMAP_FMT_ABGR8888: {
      return PIXMAN_r8g8b8a8;
    }
    case BITMAP_FMT_BGRA8888: {
      return PIXMAN_b8g8r8a8;
    }
    case BITMAP_FMT_ARGB8888: {
      return PIXMAN_a8b8g8r8;
    }
    case BITMAP_FMT_RGB565: {
      return PIXMAN_r5g6b5;
    }
    case BITMAP_FMT_BGR565: {
      return PIXMAN_b5g6r5;
    }
    case BITMAP_FMT_RGB888: {
      return PIXMAN_r8g8b8;
    }
    case BITMAP_FMT_BGR888: {
      return PIXMAN_b8g8r8;
    }
    default: {
      assert(!"not supported");
      return 0;
    }
  }
}

pixman_image_t* to_pixman_image(bitmap_t* img) {
  pixman_format_code_t fmt = to_pixman_format(img->format);
  uint32_t stride = bitmap_get_line_length(img);

  return pixman_image_create_bits_no_clear(fmt, img->w, img->h, (uint32_t*)(img->data), stride);
}

ret_t pixman_draw_image_matrix(bitmap_t* fb, draw_image_info_t* info) {
  pixman_region16_t clip;
  pixman_box16_t clip_box;
  rect_t* r = &(info->clip);
  bitmap_t* img = info->img;
  matrix_t* m = &(info->matrix);
  pixman_transform_t transform;
  pixman_image_t* s = to_pixman_image(img);
  pixman_image_t* d = to_pixman_image(fb);

  pixman_region_init_rect(&clip, r->x, r->y, r->w, r->h);
  pixman_image_set_clip_region(d, &clip);

  to_pixman_transfrom(&transform, m);
  pixman_transform_invert(&transform, &transform);
  pixman_image_set_transform(s, &transform);

  pixman_image_composite(PIXMAN_OP_OVER, s, NULL, d, 0, 0, 0, 0, 0, 0, fb->w, fb->h);

  pixman_image_unref(s);
  pixman_image_unref(d);

  return RET_OK;
}

ret_t load_image(const char* filename, bitmap_t* img) {
  int w = 0;
  int h = 0;
  int c = 0;
  void* data = stbi_load(filename, &w, &h, &c, 4);

  memset(img, 0x00, sizeof(bitmap_t));
  img->w = w;
  img->h = h;
  img->format = BITMAP_FMT_RGBA8888;
  img->data = data;
  img->should_free_data = TRUE;
  img->line_length = w * 4;

  return RET_OK;
}

ret_t write_image(const char* filename, bitmap_t* img) {
  stbi_write_png(filename, img->w, img->h, 4, img->data, 0);

  return RET_OK;
}
