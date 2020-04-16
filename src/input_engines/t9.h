
/**
 * File:   t9.h
 * Author: AWTK Develop Team
 * Brief:  t9 input method helper functions
 *
 * Copyright (c) 2018 - 2020  Guangzhou ZHIYUAN Electronics Co.,Ltd.
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
 * 2020-04-16 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"

#ifndef TK_T9_H
#define TK_T9_H

BEGIN_C_DECLS

typedef struct _t9_item_info_t {
  const char* key;
  const char** words;
} t9_item_info_t;

uint32_t t9_search(const t9_item_info_t* items, uint32_t items_nr, const char* keys,
                   wbuffer_t* result);

END_C_DECLS

#endif /*TK_T9_H*/
