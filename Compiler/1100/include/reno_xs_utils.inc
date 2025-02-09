stock const XS_X = 0;
stock const XS_Y = 1;
stock const XS_Z = 2;

/**
 * Linearly interpolates between two values.
 *
 * Linearly interpolates between a and b by t.
 * When t = 0 returns a.
 * When t = 1 return b.
 * When t = 0.5 returns the midpoint of a and b.
 *
 * @param a         Start value, returned when t = 0.
 * @param b         End value, returned when t = 1.
 * @param t         Value used to interpolate between a and b.
 *
 * @return          The interpolated float result between the two float values.
 */
stock Float:_xs_lerp(const Float: a, const Float: b, const Float: t) {
    return a + (b - a) * t;
}

/**
 * Determines where a value lies between two points.
 * The a and b values define the start and end of a linear numeric range.
 * The "value" parameter you supply represents a value which might lie somewhere within that range.
 * This method calculates where, within the specified range, the "value" parameter falls.
 * If the "value" parameter is within the range, _xs_inverse_lerp() returns a value between zero and one,
 * proportional to the value's position within the range.
 * If the "value" parameter falls outside of the range, _xs_inverse_lerp() returns either zero or one,
 * depending on whether it falls before the start of the range or after the end of the range.
 *
 * @param a         The a of the range.
 * @param b         The b of the range.
 * @param t         The point within the range you want to calculate.
 *
 * @return          A value between zero and one,
 *                  representing where the "value" parameter
 *                  falls within the range defined by `a` and `b`.
 */
stock Float:_xs_inverse_lerp(const Float: a, const Float: b, const Float: value) {
    return (value - a) / (b - a)
}

/**
 * Linearly interpolates between two vectors.
 *
 * @param a         Start value, returned when t = 0.
 * @param b         End value, returned when t = 1.
 * @param out       The output vector. Can be one of the input vectors.
 * @param t         Value used to interpolate between a and b.
 *
 * @noreturn
 */
stock _xs_vec_lerp(Float: a[], Float: b[], Float: out[], const Float: t, const size = sizeof(out)) {
    for (new i; i < size; i++) {
        out[i] = a[i] + (b[i] - a[i]) * t;
    }
}

/**
 * Linearly interpolates between two angles.
 * This method returns the shortest path between the specified angles.
 * This method wraps around values that are outside the range [-180, 180].
 * For example, _xs_vec_lerp_angle(1.0, 190.0, 1.0) returns -170.0.
 * To find the longest path use xs_lerp().
 *
 * @param a         The a angle. A float expressed in degrees.
 * @param b         The b angle. A float expressed in degrees.
 * @param t         Value used to interpolate between a and b.
 *
 * @return          Returns the interpolated float result between angle `a` and angle `b`,
 *                  based on the interpolation value `t`.
 */
stock Float:_xs_vec_lerp_angle(Float: a, Float: b, const Float: t) {
    while (a < -180.0)
        a += 360.0;
    while (a > 180.0)
        a -= 360.0;

    while (b < -180.0)
        b += 360.0;
    while (b > 180.0)
        b -= 360.0;

    new Float: angleDiff = b - a;

    if (angleDiff > 180.0)
        angleDiff -= 360.0;
    else if (angleDiff < -180.0)
        angleDiff += 360.0;

    new Float: result = a + t * angleDiff;

    while (result < -180.0)
        result += 360.0;
    while (result > 180.0)
        result -= 360.0;

    return result;
}

/**
 * Converts a vector represented as an array of floats to a formatted string.
 *
 * @param vec       The vector represented as an array of floats.
 * @param output    The output string where the formatted vector will be stored.
 * @param len       The maximum length of the output string.
 * @param precision The number of decimal places for each vector component.
 * @param size      The size of the vector array.
 *
 * @return          Returns the number of characters written to the output string.
 *                  It is the length of the formatted vector string.
 */
stock _xs_vec_to_string(const Float: vec[], output[], const len, const precision = 1, const size = sizeof(vec)) {
    new _len = copy(output, len, "(");

    for (new i = 0; i < size && _len < len; ++i) {
        _len += format(output[_len], (len - _len),
            fmt("%%.%if%s", precision, (i < size - 1) ? ", " : ")"),
            vec[i]
        );
    }

    return _len;
}
