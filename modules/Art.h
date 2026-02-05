#ifndef ART_H
#define ART_H

// @Section: RGBA vector colors (linear 0–1)

#define COLOR_CLEAR              v4f32(0.0f, 0.0f, 0.0f, 0.0f)

#define COLOR_WHITE(alpha)       v4f32(1.0f, 1.0f, 1.0f, (f32)(alpha))
#define COLOR_BLACK(alpha)       v4f32(0.0f, 0.0f, 0.0f, (f32)(alpha))
#define COLOR_GRAY(alpha)        v4f32(0.5f, 0.5f, 0.5f, (f32)(alpha))
#define COLOR_DARK_GRAY(alpha)   v4f32(0.25f, 0.25f, 0.25f, (f32)(alpha))
#define COLOR_LIGHT_GRAY(alpha)  v4f32(0.75f, 0.75f, 0.75f, (f32)(alpha))

#define COLOR_RED(alpha)         v4f32(1.0f, 0.0f, 0.0f, (f32)(alpha))
#define COLOR_GREEN(alpha)       v4f32(0.0f, 1.0f, 0.0f, (f32)(alpha))
#define COLOR_BLUE(alpha)        v4f32(0.0f, 0.0f, 1.0f, (f32)(alpha))

#define COLOR_YELLOW(alpha)      v4f32(1.0f, 1.0f, 0.0f, (f32)(alpha))
#define COLOR_CYAN(alpha)        v4f32(0.0f, 1.0f, 1.0f, (f32)(alpha))
#define COLOR_MAGENTA(alpha)     v4f32(1.0f, 0.0f, 1.0f, (f32)(alpha))

#define COLOR_ORANGE(alpha)      v4f32(1.0f, 0.5f, 0.0f, (f32)(alpha))
#define COLOR_PURPLE(alpha)      v4f32(0.5f, 0.0f, 0.5f, (f32)(alpha))
#define COLOR_PINK(alpha)        v4f32(1.0f, 0.75f, 0.8f, (f32)(alpha))

#define COLOR_BROWN(alpha)       v4f32(0.6f, 0.4f, 0.2f, (f32)(alpha))

// @Section: Packed colors (Alpha 1)

#define COLOR_PACKED_CLEAR      0x00000000u

#define COLOR_PACKED_WHITE      0xFFFFFFFFu
#define COLOR_PACKED_BLACK      0xFF000000u
#define COLOR_PACKED_GRAY       0xFF7F7F7Fu
#define COLOR_PACKED_DARK_GRAY  0xFF3F3F3Fu
#define COLOR_PACKED_LIGHT_GRAY 0xFFBFBFBFu

#define COLOR_PACKED_RED        0xFF0000FFu
#define COLOR_PACKED_GREEN      0xFF00FF00u
#define COLOR_PACKED_BLUE       0xFFFF0000u

#define COLOR_PACKED_YELLOW     0xFF00FFFFu
#define COLOR_PACKED_CYAN       0xFFFFFF00u
#define COLOR_PACKED_MAGENTA    0xFFFF00FFu

#define COLOR_PACKED_ORANGE     0xFF007FFFu
#define COLOR_PACKED_PURPLE     0xFF7F007Fu
#define COLOR_PACKED_PINK       0xFFCCBFFFu

#define COLOR_PACKED_BROWN      0xFF336699u

// @Section: Hex color strings (#RRGGBB)

#define COLOR_HEX_WHITE        "#FFFFFF"
#define COLOR_HEX_BLACK        "#000000"
#define COLOR_HEX_GRAY         "#808080"
#define COLOR_HEX_DARK_GRAY    "#404040"
#define COLOR_HEX_LIGHT_GRAY   "#C0C0C0"

#define COLOR_HEX_RED          "#FF0000"
#define COLOR_HEX_GREEN        "#00FF00"
#define COLOR_HEX_BLUE         "#0000FF"

#define COLOR_HEX_YELLOW       "#FFFF00"
#define COLOR_HEX_CYAN         "#00FFFF"
#define COLOR_HEX_MAGENTA      "#FF00FF"

#define COLOR_HEX_ORANGE       "#FF8000"
#define COLOR_HEX_PURPLE       "#800080"
#define COLOR_HEX_PINK         "#FFC0CB"

#define COLOR_HEX_BROWN        "#996633"


// @Section: Helper functions

function u32     color_pack(V4f32 color); /* Packs a V4f32 RGBA color into an unsigned 32 bit integer */
function V4f32 color_unpack(u32 color); /* Unpacks a unsigned 32 bit integer into an RGBA V4f32 */

function V3f32 hsv_to_rgb(f32 h, f32 s, f32 v);
function V4f32 rainbow_color(f32 delta_time, f32 speed, f32 alpha);

// @Section: Implementation

function u32
color_pack(V4f32 color)
{
  u8 r = (u8)(color.x * 255.0f);
  u8 g = (u8)(color.y * 255.0f);
  u8 b = (u8)(color.z * 255.0f);
  u8 a = (u8)(color.w * 255.0f);
  return (a << 24) | (b << 16) | (g << 8) | r;
}

function V4f32
color_unpack(u32 color)
{
  V4f32 result;
  result.x = (f32)((color >>  0) & 0xFF) * (1.0f / 255.0f);
  result.y = (f32)((color >>  8) & 0xFF) * (1.0f / 255.0f);
  result.z = (f32)((color >> 16) & 0xFF) * (1.0f / 255.0f);
  result.w = (f32)((color >> 24) & 0xFF) * (1.0f / 255.0f);
  return result;
}

function V3f32
hsv_to_rgb(f32 h, f32 s, f32 v)
{
  f32 r = 0, g = 0, b = 0;

  f32 i = floorf(h * 6.0f);
  f32 f = h * 6.0f - i;
  f32 p = v * (1.0f - s);
  f32 q = v * (1.0f - f * s);
  f32 t = v * (1.0f - (1.0f - f) * s);

  switch ((s32)i % 6)
  {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }

  return v3f32(r, g, b);
}

function V4f32
rainbow_color(f32 delta_time, f32 speed, f32 alpha)
{
  local_persist f32 hue = 0.0f;
  hue += speed * delta_time;
  if (hue >= 1.0f) hue -= 1.0f;
  V3f32 rgb = hsv_to_rgb(hue, 1.0f, 1.0f);
  return v4f32(rgb.x, rgb.y, rgb.z, alpha);
}

#endif // ART_H