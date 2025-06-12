#include "Terminal.hpp"
#include "../Libraries/flanterm/src/flanterm_backends/fb.h"
#include "../Libraries/flanterm/src/flanterm.h"

#include "../Libraries/String.hpp"

namespace Kt {
    flanterm_context *ctx;

    void Initialize(std::uint32_t *framebuffer, std::size_t width, std::size_t height, std::size_t pitch,
        std::uint8_t red_mask_size, std::uint8_t red_mask_shift,
        std::uint8_t green_mask_size, std::uint8_t green_mask_shift,
        std::uint8_t blue_mask_size, std::uint8_t blue_mask_shift
        )
    {
        ctx = flanterm_fb_init(
            NULL,
            NULL,
            framebuffer,
            width, height, pitch,
            red_mask_size, red_mask_shift,
            green_mask_size, green_mask_shift,
            blue_mask_size, blue_mask_shift,
            NULL,
            NULL, NULL,
            NULL, NULL,
            NULL, NULL,
            NULL, 0, 0, 1,
            0, 0,
            0
        );
    }

    void Putchar(char c) {
        flanterm_write(ctx, &c, 1);
    }

    void Print(const char *text) {
        flanterm_write(ctx, text, Lib::strlen(text));
    }
};