/**
 * File:   draw_image_matrix.c
 * Author: AWTK Develop Team
 * Brief:  draw_image_matrix
 *
 * Copyright (c) 2018 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-04-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "draw_image_matrix.h"

#ifdef WITH_PIXMAN
#include "pixman/pixman/pixman.h"

static ret_t to_pixman_transfrom(pixman_transform_t* t, matrix_t* m) {
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

static pixman_format_code_t to_pixman_format(bitmap_format_t fmt) {
  switch (fmt) {
    case BITMAP_FMT_RGBA8888: {
      return PIXMAN_r8g8b8a8;
    }
    case BITMAP_FMT_ABGR8888: {
      return PIXMAN_a8b8g8r8;
    }
    case BITMAP_FMT_BGRA8888: {
      return PIXMAN_b8g8r8a8;
    }
    case BITMAP_FMT_ARGB8888: {
      return PIXMAN_a8b8g8r8;
    }
    case BITMAP_FMT_RGB565: {
      return PIXMAN_b5g6r5;
    }
    case BITMAP_FMT_BGR565: {
      return PIXMAN_r5g6b5;
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

static pixman_image_t* to_pixman_image(bitmap_t* img) {
  pixman_format_code_t fmt = to_pixman_format(img->format);
  uint32_t stride = bitmap_get_line_length(img);

  return pixman_image_create_bits_no_clear(fmt, img->w, img->h, (uint32_t*)(img->data), stride);
}

ret_t pixman_draw_image_matrix(bitmap_t* fb, draw_image_info_t* info) {
  pixman_region16_t clip;
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
  pixman_image_set_filter(s, PIXMAN_FILTER_BILINEAR, NULL, 0);
  pixman_image_composite(PIXMAN_OP_OVER, s, NULL, d, 0, 0, 0, 0, 0, 0, fb->w, fb->h);

  pixman_image_unref(s);
  pixman_image_unref(d);

  return RET_OK;
}
#endif /*WITH_PIXMAN*/

#ifdef WITH_VGCANVAS
ret_t vgcanvas_draw_image_matrix(vgcanvas_t* vg, draw_image_info_t* info) {
  bitmap_t* img = info->img;
  matrix_t* m = &(info->matrix);
  rect_t r = info->clip;
  rect_t s = rect_init(0, 0, img->w, img->h);
  rect_t d = rect_init(0, 0, img->w, img->h);

  vgcanvas_save(vg);
  vgcanvas_clip_rect(vg, r.x, r.y, r.w, r.h);
  vgcanvas_set_transform(vg, m->a0, m->a1, m->a2, m->a3, m->a4, m->a5);
  vgcanvas_draw_image(vg, img, s.x, s.y, s.w, s.h, d.x, d.y, d.w, d.h);
  vgcanvas_restore(vg);

  return RET_OK;
}
#endif /*WITH_VGCANVAS*/
