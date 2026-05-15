#define DRAW_DEC 10u
#define DRAW_HEX 16u

const uint DIGIT_MASK[16] = uint[16](
    // 0-9
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9

    // A-F
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
);
bool draw_digit(uint mask, vec2 v, vec2 o, float s)
{
    vec2 p = (v - o) / s;

    float x = p.x;
    float y = p.y;

    return
        ((mask & 1u) != 0u &&
            x > 5.0 && x < 45.0 &&
            y > 0.0 && y < 10.0)

        || ((mask & 2u) != 0u &&
            x > 40.0 && x < 50.0 &&
            y > 10.0 && y < 50.0)

        || ((mask & 4u) != 0u &&
            x > 40.0 && x < 50.0 &&
            y > 60.0 && y < 100.0)

        || ((mask & 8u) != 0u &&
            x > 5.0 && x < 45.0 &&
            y > 100.0 && y < 110.0)

        || ((mask & 16u) != 0u &&
            x > 0.0 && x < 10.0 &&
            y > 60.0 && y < 100.0)

        || ((mask & 32u) != 0u &&
            x > 0.0 && x < 10.0 &&
            y > 10.0 && y < 50.0)

        || ((mask & 64u) != 0u &&
            x > 5.0 && x < 45.0 &&
            y > 50.0 && y < 60.0);
}
bool draw_value(uint val, vec2 current_frag, vec2 output_pos, float size,uint type)
{
    uint d0;
    uint d1;
    uint d2;
    uint d3;

    if(type == DRAW_DEC)
    {
        val = min(val, 9999u);

        d0 = DIGIT_MASK[(val / 1000u) % 10u];
        d1 = DIGIT_MASK[(val / 100u) % 10u];
        d2 = DIGIT_MASK[(val / 10u) % 10u];
        d3 = DIGIT_MASK[val % 10u];
    }
    else // DRAW_HEX
    {
        d0 = DIGIT_MASK[(val >> 12u) & 0xFu];
        d1 = DIGIT_MASK[(val >>  8u) & 0xFu];
        d2 = DIGIT_MASK[(val >>  4u) & 0xFu];
        d3 = DIGIT_MASK[(val >>  0u) & 0xFu];
    }

    float w = 60.0 * size;

    return
        draw_digit(d0, current_frag, output_pos + vec2(0.0 * w, 0.0), size) ||
        draw_digit(d1, current_frag, output_pos + vec2(1.0 * w, 0.0), size) ||
        draw_digit(d2, current_frag, output_pos + vec2(2.0 * w, 0.0), size) ||
        draw_digit(d3, current_frag, output_pos + vec2(3.0 * w, 0.0), size);
}
