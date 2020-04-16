
/**
 * File:   t9.c
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

#include "t9.h"

int32_t t9_search_index(const t9_item_info_t* items, uint32_t items_nr, const char* key,
                        bool_t exact) {
  int r = 0;
  int32_t low = 0;
  int32_t high = items_nr - 1;
  uint32_t key_len = strlen(key);

  while (low <= high) {
    uint32_t mid = (low + high) / 2;
    const t9_item_info_t* iter = items + mid;
    if (exact) {
      r = strcmp(iter->key, key);
    } else {
      r = strncmp(iter->key, key, key_len);
    }
    if (r == 0) {
      return mid;
    } else if (r < 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return -1;
}

uint32_t t9_search(const t9_item_info_t* items, uint32_t items_nr, const char* key,
                   wbuffer_t* result) {
  uint32_t i = 0;
  uint32_t nr = 0;
  int32_t found = 0;
  const t9_item_info_t* iter = NULL;
  return_value_if_fail(items != NULL && key != NULL && result != NULL, 0);

  result->cursor = 0;
  found = t9_search_index(items, items_nr, key, TRUE);
  if (found >= 0) {
    iter = items + found;
    while (iter->words[i] != NULL) {
      if (wbuffer_write_string(result, iter->words[i]) != RET_OK) {
        break;
      }
      i++;
      nr++;
    }

    return nr;
  }

  found = t9_search_index(items, items_nr, key, FALSE);
  if (found >= 0) {
    uint32_t end = 0;
    uint32_t key_len = strlen(key);

    for (i = 0; i < 10; i++) {
      for (end = found; end < items_nr; end++) {
        iter = items + found;
        if (strncmp(iter->key, key, key_len) == 0) {
          if (wbuffer_write_string(result, iter->words[i]) != RET_OK) {
            return nr;
          }
          nr++;
        } else {
          break;
        }
      }
    }
  }

  return nr;
}
