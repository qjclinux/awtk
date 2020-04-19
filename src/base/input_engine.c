/**
 * File:   input_engine.
 * Author: AWTK Develop Team
 * Brief:  input method engine
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
 * 2018-06-23 Li XianJing <xianjimli@hotmail.com> created
 *
 */
#include "base/keys.h"
#include "base/input_engine.h"

ret_t input_engine_reset_input(input_engine_t* engine) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);

  engine->keys.size = 0;
  engine->keys.str[0] = '\0';
  engine->candidates_nr = 0;

  if (engine->reset_input) {
    ret = engine->reset_input(engine);
  }

  return ret;
}

ret_t input_engine_set_lang(input_engine_t* engine, const char* lang) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);

  if (engine->set_lang) {
    ret = engine->set_lang(engine, lang);
  }

  return ret;
}

ret_t input_engine_search(input_engine_t* engine, const char* keys) {
  return_value_if_fail(engine != NULL && engine->search != NULL && keys != NULL, RET_BAD_PARAMS);

  return engine->search(engine, keys);
}

ret_t input_engine_search_with_keys(input_engine_t* engine, const char* keys) {
  return_value_if_fail(engine != NULL && keys != NULL, RET_BAD_PARAMS);

  return input_engine_search(engine, keys);
}

ret_t input_engine_input(input_engine_t* engine, int key) {
  ret_t ret = RET_OK;
  return_value_if_fail(engine != NULL, RET_BAD_PARAMS);

  if (key == TK_KEY_BACKSPACE || key == TK_KEY_DELETE) {
    if (engine->keys.size > 0) {
      engine->keys.size--;
      engine->keys.str[engine->keys.size] = '\0';
    } else {
      input_engine_reset_input(engine);
      return RET_FAIL;
    }
  } else {
    if (engine->keys.size >= TK_IM_MAX_INPUT_CHARS) {
      return RET_BAD_PARAMS;
    }

    if (engine->input != NULL) {
      if (engine->input(engine, key) == RET_OK) {
        str_append_char(&(engine->keys), (char)key);
      } else {
        return RET_FAIL;
      }
    } else {
      str_append_char(&(engine->keys), (char)key);
    }
  }

  if (engine->search) {
    ret = engine->search(engine, engine->keys.str);
  }

  return ret;
}
