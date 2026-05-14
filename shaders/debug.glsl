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
bool draw_9999(uint val, vec2 v, vec2 o) {
    val = min(val, 9999u);
    uint a = (val / 1000u) % 10u;
    uint b = (val / 100u) % 10u;
    uint c = (val / 10u) % 10u;
    uint d = val % 10u;

    bool result =
        draw_digit(a, v, o + vec2(0.0, 0.0)) ||
            draw_digit(b, v, o + vec2(60.0, 0.0)) ||
            draw_digit(c, v, o + vec2(120.0, 0.0)) ||
            draw_digit(d, v, o + vec2(180.0, 0.0));
    return result;
}
