/**
 * File:   pixman_helper.h
 * Author: AWTK Develop Team
 * Brief:  pixman helper
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
 * 2019-04-11 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef TK_PIXMAN_HELPER_H
#define TK_PIXMAN_HELPER_H

#ifdef WITH_PIXMAN
#include "tkc/matrix.h"
#include "pixman/pixman/pixman.h"

static inline pixman_color_t to_pixman_color(color_t c) {
  pixman_color_t pc;
  pc.red = c.rgba.r << 8;
  pc.green = c.rgba.g << 8;
  pc.blue = c.rgba.b << 8;
  pc.alpha = c.rgba.a << 8;

  return pc;
}

static inline ret_t to_pixman_transfrom(pixman_transform_t* t, matrix_t* m) {
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

static inline pixman_format_code_t to_pixman_format(bitmap_format_t fmt) {
  switch (fmt) {
    case BITMAP_FMT_RGBA8888: {
      return PIXMAN_a8b8g8r8;
    }
    case BITMAP_FMT_ABGR8888: {
      return PIXMAN_r8g8b8a8;
    }
    case BITMAP_FMT_BGRA8888: {
      return PIXMAN_a8r8g8b8;
    }
    case BITMAP_FMT_ARGB8888: {
      return PIXMAN_b8g8r8a8;
    }
    case BITMAP_FMT_RGB565: {
      return PIXMAN_b5g6r5;
    }
    case BITMAP_FMT_BGR565: {
      return PIXMAN_r5g6b5;
    }
    case BITMAP_FMT_RGB888: {
      return PIXMAN_b8g8r8;
    }
    case BITMAP_FMT_BGR888: {
      return PIXMAN_r8g8b8;
    }
    default: {
      assert(!"not supported");
      return 0;
    }
  }
}

static inline pixman_image_t* to_pixman_image(bitmap_t* img) {
  pixman_format_code_t fmt = to_pixman_format(img->format);
  uint32_t stride = bitmap_get_line_length(img);

  return pixman_image_create_bits_no_clear(fmt, img->w, img->h, (uint32_t*)(img->data), stride);
}

#endif /*WITH_PIXMAN*/
#endif /*TK_PIXMAN_HELPER_H*/
