#include "ui.h"
#include "logger.h"

#include <stdlib.h>


Font global_font_ui;
Font global_font_hud;

void init_ui_theme(void) {
    global_font_ui = LoadFontEx("../assets/fonts/Inter/static/Inter_28pt-Regular.ttf", 150, 0, 0);
    global_font_hud = LoadFontEx("../assets/fonts/JetBrains_Mono/static/JetBrainsMono-Regular.ttf", 64, 0, 0);

    // TODO: keep trying
    // GenTextureMipmaps(&font_ui.texture);
    // GenTextureMipmaps(&font_hud.texture);
    // SetTextureFilter(font_ui.texture, TEXTURE_FILTER_TRILINEAR);
    // SetTextureFilter(font_hud.texture, TEXTURE_FILTER_TRILINEAR);

    SetTextureFilter(global_font_ui.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(global_font_hud.texture, TEXTURE_FILTER_BILINEAR);

    if (global_font_ui.texture.id == 0 || global_font_hud.texture.id == 0) {
        LOG_WARN("[SYS] Custom fonts failed to load. Using default Raylib font. Check 'assets/fonts/' directory.");
    } else {
        LOG_INFO("[SYS] UI Theme and custom fonts loaded successfully.");
    }
}

void unload_ui_theme(void) {
    UnloadFont(global_font_ui);
    UnloadFont(global_font_hud);
    LOG_INFO("[SYS] UI Theme unloaded.");
}

AppState *init_state(void) {
    AppState *s = calloc(1, sizeof(AppState));
    if (!s) {
        LOG_FATAL("[SYS] Memory allocation failed -> Target: AppState struct in init_state()");
        return NULL;
    }

    s->current_screen = SCREEN_MENU;
    s->flags = APP_FLAG_SHOW_TRAIL;

    return s;
}

void destroy_state(AppState *state) {
    if (state) free(state);
}


