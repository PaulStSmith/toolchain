#include <stdbool.h>
#include <stdint.h>

typedef union F64_pun {
    long double flt;
    uint64_t bin;
    struct {
        uint16_t low16;
        uint32_t mid32;
		uint16_t exp16;
    } split;
} F64_pun;

#define Float64_bias      1023
#define Float64_mant_bits 52
#define uint32_max_exp    31
#define uint48_bits       48

long double _ultod_c(uint32_t x) {
	if (x == 0) {
		return 0.0L;
	}
	F64_pun ret;
	uint32_t val = (uint32_t)(x);
	unsigned clz_result = __builtin_clzl(val);
	// clears the MSB since the float will be normalized
	val <<= clz_result + 1;

	ret.split.low16 = 0;
	ret.split.mid32 = val;
	ret.split.exp16 = Float64_bias + (uint32_max_exp - clz_result);
	
	ret.bin <<= Float64_mant_bits - uint48_bits;

	return ret.flt;
}
