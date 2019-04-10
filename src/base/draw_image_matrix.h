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

#ifndef TK_DRAW_IMAGE_MATRIX
#define TK_DRAW_IMAGE_MATRIX

#include "tkc/rect.h"
#include "tkc/matrix.h"
#include "base/bitmap.h"
#include "base/vgcanvas.h"

BEGIN_C_DECLS

typedef struct _draw_image_info_t {
  bitmap_t* img;
  rect_t clip;
  matrix_t matrix;
} draw_image_info_t;

#ifdef WITH_PIXMAN
ret_t pixman_draw_image_matrix(bitmap_t* fb, draw_image_info_t* info);
#endif /*WITH_PIXMAN*/

#ifdef WITH_VGCANVAS
ret_t vgcanvas_draw_image_matrix(vgcanvas_t* vg, draw_image_info_t* info);
#endif /*WITH_VGCANVAS*/

END_C_DECLS

#endif /*TK_DRAW_IMAGE_MATRIX*/
