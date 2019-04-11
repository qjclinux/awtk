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
#include "pixman_helper.h"
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
