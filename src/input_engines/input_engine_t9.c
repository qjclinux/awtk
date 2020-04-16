/**
 * File:   input_engine_t9.c
 * Author: AWTK Develop Team
 * Brief:  t9 input method engine
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

#include "t9.h"
#include "t9_zh_cn.inc"

#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "base/input_engine.h"

#define MAX_WORD_LEN 32

typedef struct _input_engine_t9_t {
  input_engine_t input_engine;

  const t9_item_info_t* items;
  uint32_t items_nr;
} input_engine_t9_t;

static ret_t input_engine_t9_reset_input(input_engine_t* engine) {
  return RET_OK;
}

static ret_t input_engine_t9_input(input_engine_t* engine, int c) {
  wbuffer_t wb;
  str_t* keys = &(engine->keys);
  input_engine_t9_t* t9 = (input_engine_t9_t*)engine;
  wbuffer_init(&wb, (uint8_t*)(engine->candidates), sizeof(engine->candidates));

  engine->candidates_nr = t9_search(t9->items, t9->items_nr, keys->str, &wb);

  log_debug("key=%s %d\n", keys->str, engine->candidates_nr);
  if (engine->candidates_nr == 0) {
    input_engine_reset_input(engine);
  }

  return RET_OK;
}

input_engine_t* input_engine_create(void) {
  input_engine_t9_t* t9 = TKMEM_ZALLOC(input_engine_t9_t);
  input_engine_t* engine = (input_engine_t*)t9;

  return_value_if_fail(engine != NULL, NULL);

  str_init(&(engine->keys), TK_IM_MAX_INPUT_CHARS + 1);
  engine->reset_input = input_engine_t9_reset_input;
  engine->input = input_engine_t9_input;

  t9->items = s_zh_cn_items;
  t9->items_nr = ARRAY_SIZE(s_zh_cn_items);

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  str_reset(&(engine->keys));
  TKMEM_FREE(engine);

  return RET_OK;
}
