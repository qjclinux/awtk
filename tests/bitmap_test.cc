#include "base/bitmap.h"
#include "gtest/gtest.h"
#include "src/blend/image_g2d.h"
#include "common.h"

TEST(Bitmap, basic) {
  uint32_t n = 100;
  uint32_t i = 0;
  for (i = 0; i < n; i++) {
    bitmap_t* b = bitmap_create_ex(i + 1, i + 1, 0, BITMAP_FMT_BGRA8888);
    ASSERT_EQ(((uint64_t)(b->data)) % BITMAP_ALIGN_SIZE, 0);
    ASSERT_EQ(bitmap_get_line_length(b), b->w * 4);
    bitmap_destroy(b);
  }

  for (i = 0; i < n; i++) {
    bitmap_t* b = bitmap_create_ex(i + 1, i + 1, 0, BITMAP_FMT_BGR565);
    ASSERT_EQ(((uint64_t)(b->data)) % BITMAP_ALIGN_SIZE, 0);
    ASSERT_EQ(bitmap_get_line_length(b), b->w * 2);
    bitmap_destroy(b);
  }
}

static void test_premulti_alpha(bitmap_format_t fmt) {
  color_t c = color_init(100, 128, 168, 100);
  color_t e = color_init((c.rgba.r * c.rgba.a) >> 8, (c.rgba.g * c.rgba.a) >> 8,
                         (c.rgba.b * c.rgba.a) >> 8, c.rgba.a);
  rect_t r = rect_init(0, 0, 2, 2);
  bitmap_t* b = bitmap_create_ex(r.w, r.h, 0, fmt);
  image_clear(b, &r, c); 
  bitmap_dump(b);

  ASSERT_EQ(bitmap_premulti_alpha(b), RET_OK);
  bitmap_dump(b);
  bitmap_check(b, &r, e.rgba);

  bitmap_destroy(b);
}

TEST(Bitmap, premulti) {
  test_premulti_alpha(BITMAP_FMT_BGRA8888);
  test_premulti_alpha(BITMAP_FMT_ARGB8888);
  test_premulti_alpha(BITMAP_FMT_ABGR8888);
  test_premulti_alpha(BITMAP_FMT_RGBA8888);
}
