/*
 * SmartMatrix Library - Common Definitions
 *
 * Copyright (c) 2015 Louis Beaudoin (Pixelmatix)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _MATRIX_COMMON_H_
#define _MATRIX_COMMON_H_

#include <stdint.h>

#ifdef ARDUINO_ARCH_AVR
#include "Arduino.h"
#endif

// struct definitions for rgb24 and rgb48 with assignment operators
// between them; adding rgb36 didn't seem to make sense because even when
// packed with bitfields, it would only save 1 byte over rgb48.
struct rgb8;
struct rgb16;
struct rgb24;
struct rgb48;

// when converting colours from coarser to finer colour spaces
// avoid left shifts for colour channel conversions 
// ( eg.: when converting 5 bit to 8 bit:  dest = source << 3 => [0, 31] => [0, 248] )
// better use the whole destination range instead: eg: [0, 31] => [0, 255]
// a fast way to do so is by using pre-calculated conversion arrays:
//   perl code to generate these arrays:
//     my ($frombits, $tobits) = (5, 8);
//     my $vartype = ($tobits > 8) ? "uint16_t" : "uint8_t";
//     my @arr = ();
//     for (my $i = 0 ; $i < 2 ** $frombits; $i++) {
//       $arr[$i] = int($i * (2**$tobits - 1) / ( ( 1 << $frombits ) - 1 ));
//     };
//     print "const ${vartype} cs_scale${frombits}to${tobits}[] = {\n  ".join(", ", @arr)."\n};\n";
const uint8_t cs_scale2to5[] = {
  0, 10, 20, 31
};
const uint8_t cs_scale2to8[] = {
  0, 85, 170, 255
};
const uint8_t cs_scale3to5[] = {
  0, 4, 8, 13, 17, 22, 26, 31
};
const uint8_t cs_scale3to6[] = {
  0, 9, 18, 27, 36, 45, 54, 63
};
const uint8_t cs_scale3to8[] = {
  0, 36, 72, 109, 145, 182, 218, 255
};
const uint8_t cs_scale5to8[] = {
  0, 8, 16, 24, 32, 41, 49, 57, 65, 74, 82, 90, 98, 106, 115, 123, 131, 139, 148,
  156, 164, 172, 180, 189, 197, 205, 213, 222, 230, 238, 246, 255
};
const uint8_t cs_scale6to8[] = {
  0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80,
  85, 89, 93, 97, 101, 105, 109, 113, 117, 121, 125, 129, 133, 137, 141, 145, 149,
  153, 157, 161, 165, 170, 174, 178, 182, 186, 190, 194, 198, 202, 206, 210, 214,
  218, 222, 226, 230, 234, 238, 242, 246, 250, 255
};
// same with uint16_t-based colour depths
const uint16_t cs_scale2to16[] = {
  0, 21845, 43690, 65535
};
const uint16_t cs_scale3to16[] = {
  0, 9362, 18724, 28086, 37448, 46810, 56172, 65535
};
const uint16_t cs_scale5to16[] = {
  0, 2114, 4228, 6342, 8456, 10570, 12684, 14798, 16912, 19026, 21140, 23254, 25368,
  27482, 29596, 31710, 33824, 35938, 38052, 40166, 42280, 44394, 46508, 48622, 50736,
  52850, 54964, 57078, 59192, 61306, 63420, 65535
};
const uint16_t cs_scale6to16[] = {
  0, 1040, 2080, 3120, 4160, 5201, 6241, 7281, 8321, 9362, 10402, 11442, 12482, 13523,
  14563, 15603, 16643, 17684, 18724, 19764, 20804, 21845, 22885, 23925, 24965, 26005, 27046,
  28086, 29126, 30166, 31207, 32247, 33287, 34327, 35368, 36408, 37448, 38488, 39529, 40569,
  41609, 42649, 43690, 44730, 45770, 46810, 47850, 48891, 49931, 50971, 52011, 53052, 54092,
  55132, 56172, 57213, 58253, 59293, 60333, 61374, 62414, 63454, 64494, 65535
};


typedef struct rgb8 {  // RGB332
    rgb8() : rgb8(0,0,0) {}

    rgb8(float r, float g, float b, float t) { red = r * 7.0; green = g * 7.0; blue = b * 3.0; }

    rgb8(uint8_t r, uint8_t g, uint8_t b) {
        red = r; green = g; blue = b;
    }
    rgb8& operator=(const rgb8& col);
    rgb8& operator=(const rgb16& col);
    rgb8& operator=(const rgb24& col);
    rgb8& operator=(const rgb48& col);

    rgb8( const rgb16& col );
    rgb8( const rgb24& col );
    rgb8( const rgb48& col );

    union {
      struct {
        uint8_t blue  :2;
        uint8_t green :3;
        uint8_t red   :3;
      };
      uint8_t rgb;
    };
} rgb8;

typedef struct rgb16 { // RGB656
    rgb16() : rgb16(0,0,0) {}

    rgb16(float r, float g, float b, float t) { red = r * 31.0; green = g * 63.0; blue = b * 31.0; }

    rgb16(uint8_t r, uint8_t g, uint8_t b) {
        red = r; green = g; blue = b;
    }
    rgb16& operator=(const rgb8& col);
    rgb16& operator=(const rgb16& col);
    rgb16& operator=(const rgb24& col);
    rgb16& operator=(const rgb48& col);

    rgb16( const rgb8& col );
    rgb16( const rgb24& col );
    rgb16( const rgb48& col );

    union {
      struct {
        uint16_t blue  :5;
        uint16_t green :6;
        uint16_t red   :5;
      };
      uint16_t rgb;
    };
} rgb16;

typedef struct rgb24 {  // RGB888
    rgb24() : rgb24(0,0,0) {}

    rgb24(float r, float g, float b, float t) { red = r * 255.0; green = g * 255.0; blue = b * 255.0; }

    rgb24(uint8_t r, uint8_t g, uint8_t b) {
        red = r; green = g; blue = b;
    }

    rgb24& operator=(const rgb8& col);
    rgb24& operator=(const rgb16& col);
    rgb24& operator=(const rgb24& col);
    rgb24& operator=(const rgb48& col);

    rgb24( const rgb8& col);
    rgb24( const rgb16& col);
    rgb24( const rgb24& col);
    rgb24( const rgb48& col);

    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgb24;

typedef struct rgb48 {
    rgb48() : rgb48(0,0,0) {}

    rgb48(float r, float g, float b, float t) {
        red = (uint16_t)(r * 65535.0); green = (uint16_t)(g * 65535.0); blue = (uint16_t)(b * 65535.0);
    }

    rgb48(uint16_t r, uint16_t g, uint16_t b) {
        red = r; green = g; blue = b;
    }
    rgb48& operator=(const rgb8& col);
    rgb48& operator=(const rgb16& col);
    rgb48& operator=(const rgb24& col);

    rgb48( const rgb8& col);
    rgb48( const rgb16& col);
    rgb48( const rgb24& col);
    rgb48( const rgb48& col);

    uint16_t red;
    uint16_t green;
    uint16_t blue;
} rgb48;

// todo: why is this assignment operator needed?  Implicitly defined assignment operator causes crashes when drawing to last pixel of last buffer of background bitmap


// rgb8-methods:

inline rgb8& rgb8::operator=(const rgb8& col) {
    rgb = col.rgb;
    return *this;
}

inline rgb8& rgb8::operator=(const rgb16& col) {
    red   = col.red   >> 2;    /* 5 -> 3 */
    green = col.green >> 3;    /* 6 -> 3 */
    blue  = col.blue  >> 3;    /* 5 -> 2 */
    return *this;
}

inline rgb8& rgb8::operator=(const rgb24& col) {
    red   = col.red   >> 5;    /* 8 -> 3 */
    green = col.green >> 5;    /* 8 -> 3 */
    blue  = col.blue  >> 6;    /* 8 -> 2 */
    return *this;
}

inline rgb8& rgb8::operator=(const rgb48& col) {
    red   = col.red   >> 13;   /* 16 -> 3 */
    green = col.green >> 13;   /* 16 -> 3 */
    blue  = col.blue  >> 14;   /* 16 -> 2 */
    return *this;
}

inline rgb8::rgb8(const rgb16& col) {
    red   = col.red   >> 2;    /* 5 -> 3 */
    green = col.green >> 3;    /* 6 -> 3 */
    blue  = col.blue  >> 3;    /* 5 -> 2 */
}
inline rgb8::rgb8(const rgb24& col) {
    red   = col.red   >> 5;    /* 8 -> 3 */
    green = col.green >> 5;    /* 8 -> 3 */
    blue  = col.blue  >> 6;    /* 8 -> 2 */
}
inline rgb8::rgb8(const rgb48& col) {
    red   = col.red   >> 13;   /* 16 -> 3 */
    green = col.green >> 13;   /* 16 -> 3 */
    blue  = col.blue  >> 14;   /* 16 -> 2 */
}


// rgb16-methods:
inline rgb16& rgb16::operator=(const rgb8& col) {
    red =   cs_scale3to5[col.red];      // 3 -> 5
    green = cs_scale3to6[col.green];    // 3 -> 6
    blue =  cs_scale2to5[col.blue];     // 2 -> 5
    return *this;
}

inline rgb16& rgb16::operator=(const rgb16& col) {
    rgb = col.rgb;
    return *this;
}

inline rgb16& rgb16::operator=(const rgb24& col) {
    red = col.red >> 3;      // 8 -> 5
    green = col.green >> 2;  // 8 -> 6
    blue = col.blue >> 3;    // 8 -> 5
    return *this;
}

inline rgb16& rgb16::operator=(const rgb48& col) {
    red = col.red >> 11;     // 16 -> 5
    green = col.green >> 10; // 16 -> 6
    blue = col.blue >> 11;   // 16 -> 5
    return *this;
}

inline rgb16::rgb16(const rgb8& col) {
    red =   cs_scale3to5[col.red];      // 3 -> 5
    green = cs_scale3to6[col.green];    // 3 -> 6
    blue =  cs_scale2to5[col.blue];     // 2 -> 5
}
inline rgb16::rgb16(const rgb24& col) {
    red = col.red >> 3;      // 8 -> 5
    green = col.green >> 2;  // 8 -> 6
    blue = col.blue >> 3;    // 8 -> 5
}
inline rgb16::rgb16(const rgb48& col) {
    red = col.red >> 11;     // 16 -> 5
    green = col.green >> 10; // 16 -> 6
    blue = col.blue >> 11;   // 16 -> 5
}


// rgb24-methods:
inline rgb24& rgb24::operator=(const rgb8& col) {
    red =   cs_scale3to8[col.red];      // 3 -> 8
    green = cs_scale3to8[col.green];    // 3 -> 8
    blue =  cs_scale2to8[col.blue];     // 2 -> 8
    return *this;
}

inline rgb24& rgb24::operator=(const rgb16& col) {
    red =   cs_scale5to8[col.red];      // 5 -> 8
    green = cs_scale6to8[col.green];    // 6 -> 8
    blue =  cs_scale5to8[col.blue];     // 5 -> 8
    return *this;
}

inline rgb24& rgb24::operator=(const rgb24& col) {
    red = col.red;
    green = col.green;
    blue = col.blue;
    return *this;
}

inline rgb24& rgb24::operator=(const rgb48& col) {
    red = col.red >> 8;
    green = col.green >> 8;
    blue = col.blue >> 8;
    return *this;
}

inline rgb24::rgb24(const rgb8& col) {
    red =   cs_scale3to8[col.red];      // 3 -> 8
    green = cs_scale3to8[col.green];    // 3 -> 8
    blue =  cs_scale2to8[col.blue];     // 2 -> 8
}

inline rgb24::rgb24(const rgb16& col) {
    red =   cs_scale5to8[col.red];      // 5 -> 8
    green = cs_scale6to8[col.green];    // 6 -> 8
    blue =  cs_scale5to8[col.blue];     // 5 -> 8
}

inline rgb24::rgb24(const rgb24& col) {
    red = col.red;
    green = col.green;
    blue = col.blue;
}

inline rgb24::rgb24(const rgb48& col) {
    red = col.red >> 8;
    green = col.green >> 8;
    blue = col.blue >> 8;
}


// rgb48-methods:
inline rgb48& rgb48::operator=(const rgb8& col) {
    red =   cs_scale3to16[col.red];     // 3 -> 16
    green = cs_scale3to16[col.green];   // 3 -> 16
    blue =  cs_scale2to16[col.blue];    // 2 -> 16
    return *this;
}

inline rgb48& rgb48::operator=(const rgb16& col) {
    red =   cs_scale5to16[col.red];     // 5 -> 16
    green = cs_scale6to16[col.green];   // 6 -> 16
    blue =  cs_scale5to16[col.blue];    // 5 -> 16
    return *this;
}

/* cheap trick to extend destination range from
 * [0x000000000000 .. 0xFF00FF00FF00] to [0x000000000000 .. 0xFFFFFFFFFFFF]
 * and maintain some level of accuracy:
 * write source colour to higher AND lower part of destination uint16_t:
 * 0x000000 stays 0x000000000000, but 0xFFFFFF will correctly be 0xFFFFFFFFFFFF
 */
inline rgb48& rgb48::operator=(const rgb24& col) {
    red = (col.red << 8) | col.red;
    green = (col.green << 8)  | col.green;
    blue = (col.blue << 8) | col.blue;
    return *this;
}

inline rgb48::rgb48(const rgb8& col) {
    red =   cs_scale3to16[col.red];     // 3 -> 16
    green = cs_scale3to16[col.green];   // 3 -> 16
    blue =  cs_scale2to16[col.blue];    // 2 -> 16
}

inline rgb48::rgb48(const rgb16& col) {
    red =   cs_scale5to16[col.red];     // 5 -> 16
    green = cs_scale6to16[col.green];   // 6 -> 16
    blue =  cs_scale5to16[col.blue];    // 5 -> 16
}

inline rgb48::rgb48(const rgb24& col) {
    red = (col.red << 8) | col.red;
    green = (col.green << 8)  | col.green;
    blue = (col.blue << 8) | col.blue;
}

inline rgb48::rgb48(const rgb48& col) {
    red = col.red;
    green = col.green;
    blue = col.blue;
}

#define NAME2(fun,suffix) fun ## suffix
#define NAME1(fun,suffix) NAME2(fun,suffix)
#define RGB_TYPE(depth) NAME1(rgb,depth)

//#if COLOR_DEPTH_RGB > 24
//#define Chan8ToColor( c ) ((c) << 8)
//#else
//#define Chan8ToColor( c ) (c)
//#endif


//#if COLOR_DEPTH_RGB > 24
#define color_chan_t uint16_t
//#else
//#define color_chan_t uint8_t
//#endif


// source - somewhere on the internet (arduino forum?)
static const uint8_t lightPowerMap8bit[256] = {
    0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 10, 11,
    11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 15, 15, 15, 16, 16, 17,
    17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 23, 24, 24,
    25, 25, 26, 26, 27, 28, 28, 29, 29, 30, 31, 31, 32, 32, 33, 34,
    34, 35, 36, 37, 37, 38, 39, 39, 40, 41, 42, 43, 43, 44, 45, 46,
    47, 47, 48, 49, 50, 51, 52, 53, 54, 54, 55, 56, 57, 58, 59, 60,
    61, 62, 63, 64, 65, 66, 67, 68, 70, 71, 72, 73, 74, 75, 76, 77,
    79, 80, 81, 82, 83, 85, 86, 87, 88, 90, 91, 92, 94, 95, 96, 98,
    99, 100, 102, 103, 105, 106, 108, 109, 110, 112, 113, 115, 116, 118, 120, 121,
    123, 124, 126, 128, 129, 131, 132, 134, 136, 138, 139, 141, 143, 145, 146, 148,
    150, 152, 154, 155, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179,
    181, 183, 185, 187, 189, 191, 193, 196, 198, 200, 202, 204, 207, 209, 211, 214,
    216, 218, 220, 223, 225, 228, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252
};


// generated by adafruit utility included with matrix library
// options: planes = 16 and GAMMA = 2.5
static const uint16_t lightPowerMap16bit[] = {
    0x00, 0x00, 0x00, 0x01, 0x02, 0x04, 0x06, 0x08,
    0x0b, 0x0f, 0x14, 0x19, 0x1f, 0x26, 0x2e, 0x37,
    0x41, 0x4b, 0x57, 0x63, 0x71, 0x80, 0x8f, 0xa0,
    0xb2, 0xc5, 0xda, 0xef, 0x106, 0x11e, 0x137, 0x152,
    0x16e, 0x18b, 0x1a9, 0x1c9, 0x1eb, 0x20e, 0x232, 0x257,
    0x27f, 0x2a7, 0x2d2, 0x2fd, 0x32b, 0x359, 0x38a, 0x3bc,
    0x3ef, 0x425, 0x45c, 0x494, 0x4cf, 0x50b, 0x548, 0x588,
    0x5c9, 0x60c, 0x651, 0x698, 0x6e0, 0x72a, 0x776, 0x7c4,
    0x814, 0x866, 0x8b9, 0x90f, 0x967, 0x9c0, 0xa1b, 0xa79,
    0xad8, 0xb3a, 0xb9d, 0xc03, 0xc6a, 0xcd4, 0xd3f, 0xdad,
    0xe1d, 0xe8f, 0xf03, 0xf79, 0xff2, 0x106c, 0x10e9, 0x1168,
    0x11e9, 0x126c, 0x12f2, 0x137a, 0x1404, 0x1490, 0x151f, 0x15b0,
    0x1643, 0x16d9, 0x1771, 0x180b, 0x18a7, 0x1946, 0x19e8, 0x1a8b,
    0x1b32, 0x1bda, 0x1c85, 0x1d33, 0x1de2, 0x1e95, 0x1f49, 0x2001,
    0x20bb, 0x2177, 0x2236, 0x22f7, 0x23bb, 0x2481, 0x254a, 0x2616,
    0x26e4, 0x27b5, 0x2888, 0x295e, 0x2a36, 0x2b11, 0x2bef, 0x2cd0,
    0x2db3, 0x2e99, 0x2f81, 0x306d, 0x315a, 0x324b, 0x333f, 0x3435,
    0x352e, 0x3629, 0x3728, 0x3829, 0x392d, 0x3a33, 0x3b3d, 0x3c49,
    0x3d59, 0x3e6b, 0x3f80, 0x4097, 0x41b2, 0x42d0, 0x43f0, 0x4513,
    0x463a, 0x4763, 0x488f, 0x49be, 0x4af0, 0x4c25, 0x4d5d, 0x4e97,
    0x4fd5, 0x5116, 0x525a, 0x53a1, 0x54eb, 0x5638, 0x5787, 0x58da,
    0x5a31, 0x5b8a, 0x5ce6, 0x5e45, 0x5fa7, 0x610d, 0x6276, 0x63e1,
    0x6550, 0x66c2, 0x6837, 0x69af, 0x6b2b, 0x6caa, 0x6e2b, 0x6fb0,
    0x7139, 0x72c4, 0x7453, 0x75e5, 0x777a, 0x7912, 0x7aae, 0x7c4c,
    0x7def, 0x7f94, 0x813d, 0x82e9, 0x8498, 0x864b, 0x8801, 0x89ba,
    0x8b76, 0x8d36, 0x8efa, 0x90c0, 0x928a, 0x9458, 0x9629, 0x97fd,
    0x99d4, 0x9bb0, 0x9d8e, 0x9f70, 0xa155, 0xa33e, 0xa52a, 0xa71a,
    0xa90d, 0xab04, 0xacfe, 0xaefb, 0xb0fc, 0xb301, 0xb509, 0xb715,
    0xb924, 0xbb37, 0xbd4d, 0xbf67, 0xc184, 0xc3a5, 0xc5ca, 0xc7f2,
    0xca1e, 0xcc4d, 0xce80, 0xd0b7, 0xd2f1, 0xd52f, 0xd771, 0xd9b6,
    0xdbfe, 0xde4b, 0xe09b, 0xe2ef, 0xe547, 0xe7a2, 0xea01, 0xec63,
    0xeeca, 0xf134, 0xf3a2, 0xf613, 0xf888, 0xfb02, 0xfd7e, 0xffff
};

// adafruit matrix library
static const uint8_t lightPowerMap4bit[256] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
    0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
    0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x05,
    0x05, 0x05, 0x05, 0x06, 0x06, 0x06, 0x06, 0x06,
    0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x07,
    0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08,
    0x08, 0x08, 0x08, 0x08, 0x09, 0x09, 0x09, 0x09,
    0x09, 0x09, 0x09, 0x09, 0x09, 0x0a, 0x0a, 0x0a,
    0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0a, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0c, 0x0c,
    0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0x0d, 0x0d,
    0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x0e, 0x0e, 0x0e,
    0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x0f, 0x0f
};

inline void calculateBackgroundLUT(color_chan_t * lut, uint8_t backgroundBrightness) {
    // update background table
    for(int i=0; i<256; i++) {
//#if COLOR_DEPTH_RGB > 24
        lut[i] = (lightPowerMap16bit[i] * backgroundBrightness) / 256;
//#else
//        lut[i] = (lightPowerMap8bit[i] * backgroundBrightness) / 256;
//#endif
    }
}



template <typename RGB_IN>
void colorCorrection(const RGB_IN& in, rgb48& out) {
    out = rgb48(lightPowerMap16bit[in.red],
                lightPowerMap16bit[in.green],
                lightPowerMap16bit[in.blue]);
}

template <typename RGB_IN>
void colorCorrection(const RGB_IN& in, rgb24& out) {
    out = rgb24(lightPowerMap16bit[in.red],
                lightPowerMap16bit[in.green],
                lightPowerMap16bit[in.blue]);
}

void calculateBackgroundLUT(color_chan_t * lut, uint8_t backgroundBrightness);

// config
typedef enum rotationDegrees {
    rotation0,
    rotation90,
    rotation180,
    rotation270
} rotationDegrees;

#endif
