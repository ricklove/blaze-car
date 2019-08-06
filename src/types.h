#pragma once

typedef unsigned long long u64;
typedef long long i64;
typedef double f64;

typedef unsigned int u32;
typedef int i32;
typedef float f32;

typedef short i16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef char i8;

typedef unsigned char BYTE;

#define ASSERT_SAME_SIZE(SA, SB) typedef char _size_check_struct[(!!(sizeof(SA) == sizeof(SB))) * 2-1]
