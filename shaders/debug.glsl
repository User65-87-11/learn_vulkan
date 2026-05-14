#ifdef _OLD2_
bool seg_a(vec2 v, vec2 o) {
    return (v.x > o.x + 5 && v.x < o.x + 5 + 40 && v.y > o.y && v.y < o.y + 10);
}
bool seg_b(vec2 v, vec2 o) {
    return (v.x > o.x + 40.0 && v.x < o.x + 40.0 + 10.0 && v.y > o.y + 10 && v.y < o.y + 10 + 40);
}
bool seg_c(vec2 v, vec2 o) {
    return (v.x > o.x + 40.0 && v.x < o.x + 40.0 + 10.0 && v.y > o.y + 10 + 40 + 10 && v.y < o.y + 10 + 40 + 10 + 40);
}
bool seg_d(vec2 v, vec2 o) {
    return (v.x > o.x + 5 && v.x < o.x + 5 + 40 && v.y > o.y + 10 + 40 + 10 + 40 && v.y < o.y + 10 + 40 + 10 + 40 + 10);
}
bool seg_e(vec2 v, vec2 o) {
    return (v.x > o.x && v.x < o.x + 10 && v.y > o.y + 10 + 40 + 10 && v.y < o.y + 10 + 40 + 10 + 40);
}
bool seg_f(vec2 v, vec2 o) {
    return (v.x > o.x && v.x < o.x + 10 && v.y > o.y + 10 && v.y < o.y + 10 + 40);
}
bool seg_g(vec2 v, vec2 o) {
    return (v.x > o.x + 5 && v.x < o.x + 5 + 40 && v.y > o.y + 10 + 40 && v.y < o.y + 10 + 40 + 10);
}
bool draw_digit(uint digit, vec2 v, vec2 o)
{
    bool a = false, b = false, c = false, d = false, e = false, f = false, g = false;

    if (digit == 0) {
        a = f = b = c = d = e = true;
    }
    if (digit == 1) {
        b = c = true;
    }
    if (digit == 2) {
        a = b = g = e = d = true;
    }
    if (digit == 3) {
        a = b = c = d = g = true;
    }
    if (digit == 4) {
        f = g = b = c = true;
    }
    if (digit == 5) {
        a = f = g = c = d = true;
    }
    if (digit == 6) {
        a = f = g = e = c = d = true;
    }
    if (digit == 7) {
        a = b = c = true;
    }
    if (digit == 8) {
        a = b = c = d = e = f = g = true;
    }
    if (digit == 9) {
        a = b = c = d = f = g = true;
    }

    return (a && seg_a(v, o)) ||
        (b && seg_b(v, o)) ||
        (c && seg_c(v, o)) ||
        (d && seg_d(v, o)) ||
        (e && seg_e(v, o)) ||
        (f && seg_f(v, o)) ||
        (g && seg_g(v, o));
}

bool draw_digit2(uint digit, vec2 v, vec2 o)
{
    uint mask = DIGIT_MASK[digit];

    float x = v.x;
    float y = v.y;

    float ox = o.x;
    float oy = o.y;

    return
    // a
    ((mask & 1u) != 0u &&
        x > ox + 5.0 && x < ox + 45.0 &&
        y > oy + 0.0 && y < oy + 10.0)
        || // b
        ((mask & 2u) != 0u &&
            x > ox + 40.0 && x < ox + 50.0 &&
            y > oy + 10.0 && y < oy + 50.0)
        || // c
        ((mask & 4u) != 0u &&
            x > ox + 40.0 && x < ox + 50.0 &&
            y > oy + 60.0 && y < oy + 100.0)
        || // d
        ((mask & 8u) != 0u &&
            x > ox + 5.0 && x < ox + 45.0 &&
            y > oy + 100.0 && y < oy + 110.0)
        || // e
        ((mask & 16u) != 0u &&
            x > ox + 0.0 && x < ox + 10.0 &&
            y > oy + 60.0 && y < oy + 100.0)
        || // f
        ((mask & 32u) != 0u &&
            x > ox + 0.0 && x < ox + 10.0 &&
            y > oy + 10.0 && y < oy + 50.0)
        || // g
        ((mask & 64u) != 0u &&
            x > ox + 5.0 && x < ox + 45.0 &&
            y > oy + 50.0 && y < oy + 60.0);
}
#endif

const uint DIGIT_MASK[10] = uint[10](
        0x3F, 0x06, 0x5B, 0x4F, 0x66,
        0x6D, 0x7D, 0x07, 0x7F, 0x6F
    );

bool draw_digit3(uint digit, vec2 v, vec2 o, float s)
{
    uint mask = DIGIT_MASK[digit];

    // transform into local digit space
    vec2 p = (v - o) / s;

    float x = p.x;
    float y = p.y;

    return ((mask & 1u) != 0u &&
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

bool draw_9999(uint val, vec2 v, vec2 o, float s)
{
    val = min(val, 9999u);

    uint a = (val / 1000u) % 10u;
    uint b = (val / 100u) % 10u;
    uint c = (val / 10u) % 10u;
    uint d = val % 10u;

    float w = 60.0 * s;

    return draw_digit3(a, v, o + vec2(0.0 * w, 0.0), s) ||
        draw_digit3(b, v, o + vec2(1.0 * w, 0.0), s) ||
        draw_digit3(c, v, o + vec2(2.0 * w, 0.0), s) ||
        draw_digit3(d, v, o + vec2(3.0 * w, 0.0), s);
}
#ifdef _OLD2_
bool draw_9999_2(uint val, vec2 v, vec2 o) {
    val = min(val, 9999u);
    uint a = (val / 1000u) % 10u;
    uint b = (val / 100u) % 10u;
    uint c = (val / 10u) % 10u;
    uint d = val % 10u;

    float y = o.y;

    bool result =
        draw_digit2(a, v, vec2(o.x + 0.0, y)) ||
            draw_digit2(b, v, vec2(o.x + 60.0, y)) ||
            draw_digit2(c, v, vec2(o.x + 120.0, y)) ||
            draw_digit2(d, v, vec2(o.x + 180.0, y));
    return result;
}
#endif
