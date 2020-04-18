/**
 * File:   input_engine_t9ext.c
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
#include "tkc/mem.h"
#include "tkc/utf8.h"
#include "tkc/buffer.h"
#include "base/input_engine.h"
#include "base/input_method.h"

#ifdef WITH_IME_T9EXT

#include "t9.h"
#include "t9ext_zh_cn.inc"
#include "pinyin_table.inc"

typedef struct _input_engine_t9ext_t {
  input_engine_t input_engine;

  uint32_t items_nr;
  const t9_item_info_t* items;

  char pre_candidates[128];
  uint32_t pre_candidates_nr;
} input_engine_t9ext_t;

static ret_t input_engine_t9ext_reset_input(input_engine_t* engine) {
  return RET_OK;
}

static const char* num_chars[] = {
    "", "ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ", "",
};

static ret_t input_engine_t9ext_add_chars(input_engine_t* engine, int c, wbuffer_t* wb) {
  char str[2];
  uint32_t n = 0;
  const char* p = NULL;
  uint32_t index = c - '0';
  return_value_if_fail(index < 9, RET_BAD_PARAMS);

  p = num_chars[index];
  memset(str, 0x00, sizeof(str));

  while (*p) {
    str[0] = *p;
    if (wbuffer_write_string(wb, str) != RET_OK) {
      break;
    }
    n++;
    p++;
  }

  return n;
}

static ret_t input_engine_t9ext_search(input_engine_t* engine, const char* keys) {
  wbuffer_t wb;
  uint32_t keys_size = strlen(keys);
  input_engine_t9ext_t* t9 = (input_engine_t9ext_t*)engine;

  if(keys_size < 1) {
    return RET_OK;
  }

  if (isdigit(keys[0])) {
    wbuffer_init(&wb, (uint8_t*)(t9->pre_candidates), sizeof(t9->pre_candidates));
    if(keys_size == 1) {
      t9->pre_candidates_nr = input_engine_t9ext_add_chars(engine, keys[0], &wb); 
    }

    t9->pre_candidates_nr += t9_search(t9->items, t9->items_nr, keys, &wb);
    input_method_dispatch_pre_candidates(engine->im, t9->pre_candidates, t9->pre_candidates_nr);
  } else {
  }

  return RET_OK;
}

static ret_t input_engine_t9ext_set_lang(input_engine_t* engine, const char* lang) {
  input_engine_t9ext_t* t9 = (input_engine_t9ext_t*)engine;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);


  return RET_OK;
}

input_engine_t* input_engine_create(input_method_t* im) {
  input_engine_t9ext_t* t9 = TKMEM_ZALLOC(input_engine_t9ext_t);
  input_engine_t* engine = (input_engine_t*)t9;

  return_value_if_fail(engine != NULL, NULL);

  str_init(&(engine->keys), TK_IM_MAX_INPUT_CHARS + 1);
  engine->reset_input = input_engine_t9ext_reset_input;
  engine->search = input_engine_t9ext_search;
  engine->set_lang = input_engine_t9ext_set_lang;
  engine->im = im;

  t9->items = s_pinyin_items;
  t9->items_nr = ARRAY_SIZE(s_pinyin_items);

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  str_reset(&(engine->keys));
  TKMEM_FREE(engine);

  return RET_OK;
}

#endif /*WITH_IME_T9EXT*/
