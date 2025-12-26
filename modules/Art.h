#ifndef ART_H
#define ART_H

// RGBA vector colors (linear 0–1)

#define COLOR_CLEAR        vector4(0.0f, 0.0f, 0.0f, 0.0f)

#define COLOR_WHITE        vector4(1.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_BLACK        vector4(0.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GRAY         vector4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_DARK_GRAY    vector4(0.25f, 0.25f, 0.25f, 1.0f)
#define COLOR_LIGHT_GRAY   vector4(0.75f, 0.75f, 0.75f, 1.0f)

#define COLOR_RED          vector4(1.0f, 0.0f, 0.0f, 1.0f)
#define COLOR_GREEN        vector4(0.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_BLUE         vector4(0.0f, 0.0f, 1.0f, 1.0f)

#define COLOR_YELLOW       vector4(1.0f, 1.0f, 0.0f, 1.0f)
#define COLOR_CYAN         vector4(0.0f, 1.0f, 1.0f, 1.0f)
#define COLOR_MAGENTA      vector4(1.0f, 0.0f, 1.0f, 1.0f)

#define COLOR_ORANGE       vector4(1.0f, 0.5f, 0.0f, 1.0f)
#define COLOR_PURPLE       vector4(0.5f, 0.0f, 0.5f, 1.0f)
#define COLOR_PINK         vector4(1.0f, 0.75f, 0.8f, 1.0f)

#define COLOR_BROWN        vector4(0.6f, 0.4f, 0.2f, 1.0f)

// Hex color strings (#RRGGBB)

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

#endif // ART_H