/// \file bit_constants.cpp
/// Source file with pre-defined constant bit patterns
/// All the constants are designed to speed slim/unslim

// --------------------------------------------------------
// WARNING!  This file is automatically generated by
// program build_bit_constants_h.cpp  Do not edit directly.
// --------------------------------------------------------

#include "bit_constants.h"

/// Constants with only bit #N set.
const Word_t bitNset[64] = {
  0x00000001ll, 0x00000002ll, 0x00000004ll, 0x00000008ll,
  0x00000010ll, 0x00000020ll, 0x00000040ll, 0x00000080ll,
  0x00000100ll, 0x00000200ll, 0x00000400ll, 0x00000800ll,
  0x00001000ll, 0x00002000ll, 0x00004000ll, 0x00008000ll,
  0x00010000ll, 0x00020000ll, 0x00040000ll, 0x00080000ll,
  0x00100000ll, 0x00200000ll, 0x00400000ll, 0x00800000ll,
  0x01000000ll, 0x02000000ll, 0x04000000ll, 0x08000000ll,
  0x10000000ll, 0x20000000ll, 0x40000000ll, 0x80000000ll,
  0x0000000100000000ll, 0x0000000200000000ll,
  0x0000000400000000ll, 0x0000000800000000ll,
  0x0000001000000000ll, 0x0000002000000000ll,
  0x0000004000000000ll, 0x0000008000000000ll,
  0x0000010000000000ll, 0x0000020000000000ll,
  0x0000040000000000ll, 0x0000080000000000ll,
  0x0000100000000000ll, 0x0000200000000000ll,
  0x0000400000000000ll, 0x0000800000000000ll,
  0x0001000000000000ll, 0x0002000000000000ll,
  0x0004000000000000ll, 0x0008000000000000ll,
  0x0010000000000000ll, 0x0020000000000000ll,
  0x0040000000000000ll, 0x0080000000000000ll,
  0x0100000000000000ll, 0x0200000000000000ll,
  0x0400000000000000ll, 0x0800000000000000ll,
  0x1000000000000000ll, 0x2000000000000000ll,
  0x4000000000000000ll, 0x8000000000000000ll,
};


/// Constants with their lowest N bits set.
const Word_t lowestNset[65] = {
  0x0,
  0x00000001ll, 0x00000003ll, 0x00000007ll, 0x0000000fll,
  0x0000001fll, 0x0000003fll, 0x0000007fll, 0x000000ffll,
  0x000001ffll, 0x000003ffll, 0x000007ffll, 0x00000fffll,
  0x00001fffll, 0x00003fffll, 0x00007fffll, 0x0000ffffll,
  0x0001ffffll, 0x0003ffffll, 0x0007ffffll, 0x000fffffll,
  0x001fffffll, 0x003fffffll, 0x007fffffll, 0x00ffffffll,
  0x01ffffffll, 0x03ffffffll, 0x07ffffffll, 0x0fffffffll,
  0x1fffffffll, 0x3fffffffll, 0x7fffffffll, 0xffffffffll,
  0x00000001ffffffffll, 0x00000003ffffffffll,
  0x00000007ffffffffll, 0x0000000fffffffffll,
  0x0000001fffffffffll, 0x0000003fffffffffll,
  0x0000007fffffffffll, 0x000000ffffffffffll,
  0x000001ffffffffffll, 0x000003ffffffffffll,
  0x000007ffffffffffll, 0x00000fffffffffffll,
  0x00001fffffffffffll, 0x00003fffffffffffll,
  0x00007fffffffffffll, 0x0000ffffffffffffll,
  0x0001ffffffffffffll, 0x0003ffffffffffffll,
  0x0007ffffffffffffll, 0x000fffffffffffffll,
  0x001fffffffffffffll, 0x003fffffffffffffll,
  0x007fffffffffffffll, 0x00ffffffffffffffll,
  0x01ffffffffffffffll, 0x03ffffffffffffffll,
  0x07ffffffffffffffll, 0x0fffffffffffffffll,
  0x1fffffffffffffffll, 0x3fffffffffffffffll,
  0x7fffffffffffffffll, 0xffffffffffffffffll,
};


/// 32-bit constants with their lowest N bits set.
const int32_t _dummy_lowestNset32bits[33] = {
  0x0,
  0x00000001, 0x00000003, 0x00000007, 0x0000000f,
  0x0000001f, 0x0000003f, 0x0000007f, 0x000000ff,
  0x000001ff, 0x000003ff, 0x000007ff, 0x00000fff,
  0x00001fff, 0x00003fff, 0x00007fff, 0x0000ffff,
  0x0001ffff, 0x0003ffff, 0x0007ffff, 0x000fffff,
  0x001fffff, 0x003fffff, 0x007fffff, 0x00ffffff,
  0x01ffffff, 0x03ffffff, 0x07ffffff, 0x0fffffff,
  0x1fffffff, 0x3fffffff, 0x7fffffff, 0xffffffff,
};
const int32_t *lowestNset32bits = _dummy_lowestNset32bits;


/// Constants with their highest N bit set.
const Word_t highestNset[65] = {
  0x0,
  0x8000000000000000ll, 0xc000000000000000ll,
  0xe000000000000000ll, 0xf000000000000000ll,
  0xf800000000000000ll, 0xfc00000000000000ll,
  0xfe00000000000000ll, 0xff00000000000000ll,
  0xff80000000000000ll, 0xffc0000000000000ll,
  0xffe0000000000000ll, 0xfff0000000000000ll,
  0xfff8000000000000ll, 0xfffc000000000000ll,
  0xfffe000000000000ll, 0xffff000000000000ll,
  0xffff800000000000ll, 0xffffc00000000000ll,
  0xffffe00000000000ll, 0xfffff00000000000ll,
  0xfffff80000000000ll, 0xfffffc0000000000ll,
  0xfffffe0000000000ll, 0xffffff0000000000ll,
  0xffffff8000000000ll, 0xffffffc000000000ll,
  0xffffffe000000000ll, 0xfffffff000000000ll,
  0xfffffff800000000ll, 0xfffffffc00000000ll,
  0xfffffffe00000000ll, 0xffffffff00000000ll,
  0xffffffff80000000ll, 0xffffffffc0000000ll,
  0xffffffffe0000000ll, 0xfffffffff0000000ll,
  0xfffffffff8000000ll, 0xfffffffffc000000ll,
  0xfffffffffe000000ll, 0xffffffffff000000ll,
  0xffffffffff800000ll, 0xffffffffffc00000ll,
  0xffffffffffe00000ll, 0xfffffffffff00000ll,
  0xfffffffffff80000ll, 0xfffffffffffc0000ll,
  0xfffffffffffe0000ll, 0xffffffffffff0000ll,
  0xffffffffffff8000ll, 0xffffffffffffc000ll,
  0xffffffffffffe000ll, 0xfffffffffffff000ll,
  0xfffffffffffff800ll, 0xfffffffffffffc00ll,
  0xfffffffffffffe00ll, 0xffffffffffffff00ll,
  0xffffffffffffff80ll, 0xffffffffffffffc0ll,
  0xffffffffffffffe0ll, 0xfffffffffffffff0ll,
  0xfffffffffffffff8ll, 0xfffffffffffffffcll,
  0xfffffffffffffffell, 0xffffffffffffffffll,
};

