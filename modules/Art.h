#ifndef ART_H
#define ART_H

typedef struct RGBA_Packed RGBA_Packed;
struct RGBA_Packed
{
  u32 v;
};

typedef struct RGBA RGBA;
struct RGBA
{
  f32 r;
  f32 g;
  f32 b;
  f32 a;
};
#define rgba(r,g,b,a) (RGBA){(r),(g),(b),(a)}

typedef struct HSV HSV;
struct HSV
{
  f32 h;
  f32 s;
  f32 v;
};
#define hsv(h,s,v) (HSV){(h),(s),(v)}

// @Section: RGBA vector colors (linear 0–1)

#define COLOR_CLEAR              rgba(0.0f, 0.0f, 0.0f, 0.0f)

#define COLOR_WHITE(alpha)       rgba(1.0f, 1.0f, 1.0f, (f32)(alpha))
#define COLOR_BLACK(alpha)       rgba(0.0f, 0.0f, 0.0f, (f32)(alpha))
#define COLOR_GRAY(alpha)        rgba(0.5f, 0.5f, 0.5f, (f32)(alpha))
#define COLOR_DARK_GRAY(alpha)   rgba(0.25f, 0.25f, 0.25f, (f32)(alpha))
#define COLOR_LIGHT_GRAY(alpha)  rgba(0.75f, 0.75f, 0.75f, (f32)(alpha))

#define COLOR_RED(alpha)         rgba(1.0f, 0.0f, 0.0f, (f32)(alpha))
#define COLOR_GREEN(alpha)       rgba(0.0f, 1.0f, 0.0f, (f32)(alpha))
#define COLOR_BLUE(alpha)        rgba(0.0f, 0.0f, 1.0f, (f32)(alpha))

#define COLOR_YELLOW(alpha)      rgba(1.0f, 1.0f, 0.0f, (f32)(alpha))
#define COLOR_CYAN(alpha)        rgba(0.0f, 1.0f, 1.0f, (f32)(alpha))
#define COLOR_MAGENTA(alpha)     rgba(1.0f, 0.0f, 1.0f, (f32)(alpha))

#define COLOR_ORANGE(alpha)      rgba(1.0f, 0.5f, 0.0f, (f32)(alpha))
#define COLOR_PURPLE(alpha)      rgba(0.5f, 0.0f, 0.5f, (f32)(alpha))
#define COLOR_PINK(alpha)        rgba(1.0f, 0.75f, 0.8f, (f32)(alpha))

#define COLOR_BROWN(alpha)       rgba(0.6f, 0.4f, 0.2f, (f32)(alpha))

// @Section: Packed colors (Alpha 1)

#define COLOR_PACKED_CLEAR      (RGBA_Packed){0x00000000u}

#define COLOR_PACKED_WHITE      (RGBA_Packed){0xFFFFFFFFu}
#define COLOR_PACKED_BLACK      (RGBA_Packed){0xFF000000u}
#define COLOR_PACKED_GRAY       (RGBA_Packed){0xFF7F7F7Fu}
#define COLOR_PACKED_DARK_GRAY  (RGBA_Packed){0xFF3F3F3Fu}
#define COLOR_PACKED_LIGHT_GRAY (RGBA_Packed){0xFFBFBFBFu}

#define COLOR_PACKED_RED        (RGBA_Packed){0xFF0000FFu}
#define COLOR_PACKED_GREEN      (RGBA_Packed){0xFF00FF00u}
#define COLOR_PACKED_BLUE       (RGBA_Packed){0xFFFF0000u}

#define COLOR_PACKED_YELLOW     (RGBA_Packed){0xFF00FFFFu}
#define COLOR_PACKED_CYAN       (RGBA_Packed){0xFFFFFF00u}
#define COLOR_PACKED_MAGENTA    (RGBA_Packed){0xFFFF00FFu}

#define COLOR_PACKED_ORANGE     (RGBA_Packed){0xFF007FFFu}
#define COLOR_PACKED_PURPLE     (RGBA_Packed){0xFF7F007Fu}
#define COLOR_PACKED_PINK       (RGBA_Packed){0xFFCCBFFFu}

#define COLOR_PACKED_BROWN      (RGBA_Packed){0xFF336699u}

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


// @Section: Implementation

fz_internal RGBA_Packed
rgba_pack(RGBA rgba)
{
  RGBA_Packed result;
  u8 r = (u8)(rgba.r * 255.0f);
  u8 g = (u8)(rgba.g * 255.0f);
  u8 b = (u8)(rgba.b * 255.0f);
  u8 a = (u8)(rgba.a * 255.0f);
  result.v = (a << 24) | (b << 16) | (g << 8) | r;
  return result;
}

fz_internal RGBA
rgba_unpack(RGBA_Packed packed)
{
  RGBA result;
  result.r = (f32)((packed.v >>  0) & 0xFF) * (1.0f / 255.0f);
  result.g = (f32)((packed.v >>  8) & 0xFF) * (1.0f / 255.0f);
  result.b = (f32)((packed.v >> 16) & 0xFF) * (1.0f / 255.0f);
  result.a = (f32)((packed.v >> 24) & 0xFF) * (1.0f / 255.0f);
  return result;
}

fz_internal RGBA
rgba_from_hsv(HSV hsv, f32 a)
{
  f32 h = hsv.h - floorf(hsv.h);
  f32 s = hsv.s;
  f32 v = hsv.v;

  f32 r = 0;
  f32 g = 0;
  f32 b = 0;

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

  return (RGBA){r, g, b, a};
}

fz_internal HSV
hsv_from_rgba(RGBA rgba)
{
  f32 r = rgba.r;
  f32 g = rgba.g;
  f32 b = rgba.b;

  f32 max = r;
  if (g > max) max = g;
  if (b > max) max = b;

  f32 min = r;
  if (g < min) min = g;
  if (b < min) min = b;

  f32 delta = max - min;

  f32 h = 0.0f;
  f32 s = 0.0f;
  f32 v = max;

  if (max > 0.0f)
  {
    s = delta / max;
  }

  if (delta > 0.0f)
  {
    if (max == r)
    {
      h = (g - b) / delta;
      if (h < 0.0f)
      {
        h += 6.0f;
      }
    }
    else if (max == g)
    {
      h = ((b - r) / delta) + 2.0f;
    }
    else
    {
      h = ((r - g) / delta) + 4.0f;
    }

    h /= 6.0f;
  }

  return (HSV){h, s, v};
}

#endif // ART_H