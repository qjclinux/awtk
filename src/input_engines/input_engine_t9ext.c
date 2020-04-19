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

#include "ime_utils.h"
#include "t9ext_zh_cn.inc"
#include "pinyin_table.inc"

typedef struct _input_engine_t9ext_t {
  input_engine_t input_engine;

  char pre_candidates[128];
  uint32_t pre_candidates_nr;
} input_engine_t9ext_t;

static ret_t input_engine_t9ext_reset_input(input_engine_t* engine) {
  input_engine_t9ext_t* t9 = (input_engine_t9ext_t*)engine;

  input_method_dispatch_pre_candidates(engine->im, t9->pre_candidates, 0);
  input_method_dispatch_candidates(engine->im, engine->candidates, 0);

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
  const char* first = NULL;
  uint32_t keys_size = strlen(keys);
  input_engine_t9ext_t* t9 = (input_engine_t9ext_t*)engine;

  if (keys_size < 1) {
    input_engine_reset_input(engine);
    return RET_OK;
  }

  log_debug("keys:%s\n", keys);
  if (isdigit(keys[0])) {
    const table_entry_t* items = s_t9ext_numbers_pinyin;
    uint32_t items_nr = ARRAY_SIZE(s_t9ext_numbers_pinyin);

    wbuffer_init(&wb, (uint8_t*)(t9->pre_candidates), sizeof(t9->pre_candidates));
    if (keys_size == 1) {
      t9->pre_candidates_nr = input_engine_t9ext_add_chars(engine, keys[0], &wb);
    } else {
      t9->pre_candidates_nr = 0;
    }

    first = (const char*)(wb.data);
    t9->pre_candidates_nr += table_search(items, items_nr, keys, &wb, FALSE);
    if (t9->pre_candidates_nr == 0) {
      input_engine_reset_input(engine);
    } else {
      input_method_dispatch_pre_candidates(engine->im, t9->pre_candidates, t9->pre_candidates_nr);
      if(keys_size > 1 && *first) {
        input_engine_t9ext_search(engine, first);
      }
    }
  } else {
    const table_entry_t* items = s_pinyin_chinese_items;
    uint32_t items_nr = ARRAY_SIZE(s_pinyin_chinese_items);

    wbuffer_init(&wb, (uint8_t*)(engine->candidates), sizeof(engine->candidates));
    engine->candidates_nr = table_search(items, items_nr, keys, &wb, TRUE);
    if (engine->candidates_nr == 0) {
      engine->candidates_nr = 1;
      wbuffer_write_string(&wb, keys);
    }
    input_method_dispatch_candidates(engine->im, engine->candidates, engine->candidates_nr);
  }

  return RET_OK;
}

static ret_t input_engine_t9ext_set_lang(input_engine_t* engine, const char* lang) {
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

  return engine;
}

ret_t input_engine_destroy(input_engine_t* engine) {
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);
  str_reset(&(engine->keys));
  TKMEM_FREE(engine);

  return RET_OK;
}

#endif /*WITH_IME_T9EXT*/
