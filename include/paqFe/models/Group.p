# 0 "Group.hpp"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 0 "<command-line>" 2
# 1 "Group.hpp"
       

# 1 "../types.hpp" 1
       

# 1 "/usr/include/c++/11/cstddef" 1 3
# 42 "/usr/include/c++/11/cstddef" 3
       
# 43 "/usr/include/c++/11/cstddef" 3






# 1 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 1 3
# 278 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 3

# 278 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 3
namespace std
{
  typedef long unsigned int size_t;
  typedef long int ptrdiff_t;


  typedef decltype(nullptr) nullptr_t;

}
# 300 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 3
namespace std
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
namespace __gnu_cxx
{
  inline namespace __cxx11 __attribute__((__abi_tag__ ("cxx11"))) { }
}
# 586 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 3
# 1 "/usr/include/x86_64-linux-gnu/c++/11/bits/os_defines.h" 1 3
# 39 "/usr/include/x86_64-linux-gnu/c++/11/bits/os_defines.h" 3
# 1 "/usr/include/features.h" 1 3 4
# 392 "/usr/include/features.h" 3 4
# 1 "/usr/include/features-time64.h" 1 3 4
# 20 "/usr/include/features-time64.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 21 "/usr/include/features-time64.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4
# 22 "/usr/include/features-time64.h" 2 3 4
# 393 "/usr/include/features.h" 2 3 4
# 486 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 1 3 4
# 559 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 560 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/long-double.h" 1 3 4
# 561 "/usr/include/x86_64-linux-gnu/sys/cdefs.h" 2 3 4
# 487 "/usr/include/features.h" 2 3 4
# 510 "/usr/include/features.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 1 3 4
# 10 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/gnu/stubs-64.h" 1 3 4
# 11 "/usr/include/x86_64-linux-gnu/gnu/stubs.h" 2 3 4
# 511 "/usr/include/features.h" 2 3 4
# 40 "/usr/include/x86_64-linux-gnu/c++/11/bits/os_defines.h" 2 3
# 587 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 2 3


# 1 "/usr/include/x86_64-linux-gnu/c++/11/bits/cpu_defines.h" 1 3
# 590 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 2 3
# 777 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 3
# 1 "/usr/include/c++/11/pstl/pstl_config.h" 1 3
# 778 "/usr/include/x86_64-linux-gnu/c++/11/bits/c++config.h" 2 3
# 50 "/usr/include/c++/11/cstddef" 2 3
# 1 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h" 1 3 4
# 143 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 209 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 415 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 426 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stddef.h" 3 4
} max_align_t;






  typedef decltype(nullptr) nullptr_t;
# 51 "/usr/include/c++/11/cstddef" 2 3

extern "C++"
{

namespace std
{

  using ::max_align_t;
}



namespace std
{




  enum class byte : unsigned char {};

  template<typename _IntegerType> struct __byte_operand { };
  template<> struct __byte_operand<bool> { using __type = byte; };
  template<> struct __byte_operand<char> { using __type = byte; };
  template<> struct __byte_operand<signed char> { using __type = byte; };
  template<> struct __byte_operand<unsigned char> { using __type = byte; };

  template<> struct __byte_operand<wchar_t> { using __type = byte; };




  template<> struct __byte_operand<char16_t> { using __type = byte; };
  template<> struct __byte_operand<char32_t> { using __type = byte; };
  template<> struct __byte_operand<short> { using __type = byte; };
  template<> struct __byte_operand<unsigned short> { using __type = byte; };
  template<> struct __byte_operand<int> { using __type = byte; };
  template<> struct __byte_operand<unsigned int> { using __type = byte; };
  template<> struct __byte_operand<long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long> { using __type = byte; };
  template<> struct __byte_operand<long long> { using __type = byte; };
  template<> struct __byte_operand<unsigned long long> { using __type = byte; };

  template<> struct __byte_operand<__int128>
  { using __type = byte; };
  template<> struct __byte_operand<unsigned __int128>
  { using __type = byte; };
# 110 "/usr/include/c++/11/cstddef" 3
  template<typename _IntegerType>
    struct __byte_operand<const _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<volatile _IntegerType>
    : __byte_operand<_IntegerType> { };
  template<typename _IntegerType>
    struct __byte_operand<const volatile _IntegerType>
    : __byte_operand<_IntegerType> { };

  template<typename _IntegerType>
    using __byte_op_t = typename __byte_operand<_IntegerType>::__type;

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>
    operator<<(byte __b, _IntegerType __shift) noexcept
    { return (byte)(unsigned char)((unsigned)__b << __shift); }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>
    operator>>(byte __b, _IntegerType __shift) noexcept
    { return (byte)(unsigned char)((unsigned)__b >> __shift); }

  constexpr byte
  operator|(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l | (unsigned)__r); }

  constexpr byte
  operator&(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l & (unsigned)__r); }

  constexpr byte
  operator^(byte __l, byte __r) noexcept
  { return (byte)(unsigned char)((unsigned)__l ^ (unsigned)__r); }

  constexpr byte
  operator~(byte __b) noexcept
  { return (byte)(unsigned char)~(unsigned)__b; }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>&
    operator<<=(byte& __b, _IntegerType __shift) noexcept
    { return __b = __b << __shift; }

  template<typename _IntegerType>
    constexpr __byte_op_t<_IntegerType>&
    operator>>=(byte& __b, _IntegerType __shift) noexcept
    { return __b = __b >> __shift; }

  constexpr byte&
  operator|=(byte& __l, byte __r) noexcept
  { return __l = __l | __r; }

  constexpr byte&
  operator&=(byte& __l, byte __r) noexcept
  { return __l = __l & __r; }

  constexpr byte&
  operator^=(byte& __l, byte __r) noexcept
  { return __l = __l ^ __r; }

  template<typename _IntegerType>
    [[nodiscard]]
    constexpr _IntegerType
    to_integer(__byte_op_t<_IntegerType> __b) noexcept
    { return _IntegerType(__b); }


}

}
# 4 "../types.hpp" 2
# 1 "/usr/include/c++/11/cstdint" 1 3
# 32 "/usr/include/c++/11/cstdint" 3
       
# 33 "/usr/include/c++/11/cstdint" 3
# 41 "/usr/include/c++/11/cstdint" 3
# 1 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h" 3 4
# 1 "/usr/include/stdint.h" 1 3 4
# 26 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/libc-header-start.h" 1 3 4
# 27 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/types.h" 1 3 4
# 27 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 28 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 1 3 4
# 19 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 20 "/usr/include/x86_64-linux-gnu/bits/timesize.h" 2 3 4
# 29 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned char __u_char;
typedef unsigned short int __u_short;
typedef unsigned int __u_int;
typedef unsigned long int __u_long;


typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef signed short int __int16_t;
typedef unsigned short int __uint16_t;
typedef signed int __int32_t;
typedef unsigned int __uint32_t;

typedef signed long int __int64_t;
typedef unsigned long int __uint64_t;






typedef __int8_t __int_least8_t;
typedef __uint8_t __uint_least8_t;
typedef __int16_t __int_least16_t;
typedef __uint16_t __uint_least16_t;
typedef __int32_t __int_least32_t;
typedef __uint32_t __uint_least32_t;
typedef __int64_t __int_least64_t;
typedef __uint64_t __uint_least64_t;



typedef long int __quad_t;
typedef unsigned long int __u_quad_t;







typedef long int __intmax_t;
typedef unsigned long int __uintmax_t;
# 141 "/usr/include/x86_64-linux-gnu/bits/types.h" 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/typesizes.h" 1 3 4
# 142 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/time64.h" 1 3 4
# 143 "/usr/include/x86_64-linux-gnu/bits/types.h" 2 3 4


typedef unsigned long int __dev_t;
typedef unsigned int __uid_t;
typedef unsigned int __gid_t;
typedef unsigned long int __ino_t;
typedef unsigned long int __ino64_t;
typedef unsigned int __mode_t;
typedef unsigned long int __nlink_t;
typedef long int __off_t;
typedef long int __off64_t;
typedef int __pid_t;
typedef struct { int __val[2]; } __fsid_t;
typedef long int __clock_t;
typedef unsigned long int __rlim_t;
typedef unsigned long int __rlim64_t;
typedef unsigned int __id_t;
typedef long int __time_t;
typedef unsigned int __useconds_t;
typedef long int __suseconds_t;
typedef long int __suseconds64_t;

typedef int __daddr_t;
typedef int __key_t;


typedef int __clockid_t;


typedef void * __timer_t;


typedef long int __blksize_t;




typedef long int __blkcnt_t;
typedef long int __blkcnt64_t;


typedef unsigned long int __fsblkcnt_t;
typedef unsigned long int __fsblkcnt64_t;


typedef unsigned long int __fsfilcnt_t;
typedef unsigned long int __fsfilcnt64_t;


typedef long int __fsword_t;

typedef long int __ssize_t;


typedef long int __syscall_slong_t;

typedef unsigned long int __syscall_ulong_t;



typedef __off64_t __loff_t;
typedef char *__caddr_t;


typedef long int __intptr_t;


typedef unsigned int __socklen_t;




typedef int __sig_atomic_t;
# 28 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wchar.h" 1 3 4
# 29 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/x86_64-linux-gnu/bits/wordsize.h" 1 3 4
# 30 "/usr/include/stdint.h" 2 3 4




# 1 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/stdint-intn.h" 3 4
typedef __int8_t int8_t;
typedef __int16_t int16_t;
typedef __int32_t int32_t;
typedef __int64_t int64_t;
# 35 "/usr/include/stdint.h" 2 3 4


# 1 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h" 1 3 4
# 24 "/usr/include/x86_64-linux-gnu/bits/stdint-uintn.h" 3 4
typedef __uint8_t uint8_t;
typedef __uint16_t uint16_t;
typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
# 38 "/usr/include/stdint.h" 2 3 4





typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;


typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;





typedef signed char int_fast8_t;

typedef long int int_fast16_t;
typedef long int int_fast32_t;
typedef long int int_fast64_t;
# 71 "/usr/include/stdint.h" 3 4
typedef unsigned char uint_fast8_t;

typedef unsigned long int uint_fast16_t;
typedef unsigned long int uint_fast32_t;
typedef unsigned long int uint_fast64_t;
# 87 "/usr/include/stdint.h" 3 4
typedef long int intptr_t;


typedef unsigned long int uintptr_t;
# 101 "/usr/include/stdint.h" 3 4
typedef __intmax_t intmax_t;
typedef __uintmax_t uintmax_t;
# 10 "/usr/lib/gcc/x86_64-linux-gnu/11/include/stdint.h" 2 3 4
# 42 "/usr/include/c++/11/cstdint" 2 3


namespace std
{

  using ::int8_t;
  using ::int16_t;
  using ::int32_t;
  using ::int64_t;

  using ::int_fast8_t;
  using ::int_fast16_t;
  using ::int_fast32_t;
  using ::int_fast64_t;

  using ::int_least8_t;
  using ::int_least16_t;
  using ::int_least32_t;
  using ::int_least64_t;

  using ::intmax_t;
  using ::intptr_t;

  using ::uint8_t;
  using ::uint16_t;
  using ::uint32_t;
  using ::uint64_t;

  using ::uint_fast8_t;
  using ::uint_fast16_t;
  using ::uint_fast32_t;
  using ::uint_fast64_t;

  using ::uint_least8_t;
  using ::uint_least16_t;
  using ::uint_least32_t;
  using ::uint_least64_t;

  using ::uintmax_t;
  using ::uintptr_t;





}
# 5 "../types.hpp" 2

# 1 "../FixedPoint.hpp" 1
       




# 5 "../FixedPoint.hpp"
namespace paqFe::internal {

class SquashStretch {
  static uint32_t stretch_lut[4096];

private:

  void init_stretch() {
    uint32_t *lut = (uint32_t*)stretch_lut;

    int pi=0;
    for (int x=-2047; x<=2047; ++x) {
      int i=squash(x);
      for (int j=pi; j<=i; ++j)
        lut[j]=x;
      pi=i+1;
    }
    lut[4095]=2047;
  }

public:
  SquashStretch() {
    init_stretch();
  }

  int squash(int d) {
    static const int t[33]={
      1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
      1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
      4050,4068,4079,4085,4089,4092,4093,4094};
    if (d>2047) return 4095;
    if (d<-2047) return 0;
    int w=d&127;
    d=(d>>7)+16;
    return (t[d]*(128-w)+t[(d+1)]*w+64) >> 7;
  }

  int stretch(int x) {
    return stretch_lut[x];
  }
};

uint32_t SquashStretch::stretch_lut[4096] = {};
SquashStretch LUT;

}
# 7 "../types.hpp" 2

namespace paqFe {

enum OpMode {
  Compress,
  Decompress
};

namespace internal {

typedef int32_t Weight;
typedef uint32_t Prob;
typedef uint32_t Context;

constexpr Prob ProbEven = 2048;
constexpr Prob ProbFirst = 2048;

template<typename T>
constexpr bool isPow2(const T& t) {
  return (t > 0 && (((t & (t - 1)) == 0)));
}

static constexpr int max(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

uint8_t BitSel(uint8_t x, int i) {
  return (x >> (7 - i)) & 0x1;
}

}
}
# 4 "Group.hpp" 2

namespace paqFe::internal {

template<typename ... Models>
class ModelGroup : Models... {
private:
# 21 "Group.hpp"
  template<typename M, typename ... Rest> static constexpr int nProbSum() { if constexpr (sizeof...(Rest)) { return nProbSum<Rest...>() + M::nProb; } else { return M::nProb; } }
  template<typename M, typename ... Rest> static constexpr int nCtxSum() { if constexpr (sizeof...(Rest)) { return nCtxSum<Rest...>() + M::nCtx; } else { return M::nCtx; } }


public:


  static constexpr int nProb = nProbSum<Models...>();
  static constexpr int nCtx = nCtxSum<Models...>();

  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    predict<Models...>(bit, pp, pctx);
  }

  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    predict_byte<Models...>(byte, pp, pctx);
  }

  Context ContextMix(const Context *pctx) {
    return ContextMix<Models...>(pctx);
  }

private:

  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    M::predict(bit, pp, pctx);

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, pp + M::nProb, pctx + M::nCtx);
    }
  }

  template<typename M, typename ... Rest>
  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    M::predict_byte(byte, pp, pctx, nProb, nCtx);

    if constexpr (sizeof...(Rest)) {
      predict_byte<Rest...>(byte, pp + M::nProb, pctx + M::nCtx);
    }
  }

  template<typename M, typename ... Rest>
  Context ContextMix(const Context *pctx) {
    Context c = 0;
    for(int i=0;i<nCtx;i++) {
      c = (c << 1) + pctx[i];
    }
    return c;
# 85 "Group.hpp"
  }

};

}
