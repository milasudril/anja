#ifdef __WAND__
target[name[includebinary.h] type[include]]
#endif

#ifndef INCLUDEBINARY_H
#define INCLUDEBINARY_H

#include <cstdint>
extern "C"
{
#if __gnu_linux__

#define INCLUDE_BINARY(identifier,filename) \
asm(".pushsection .data\n" \
	"\t.local " #identifier "_begin\n" \
	"\t.type " #identifier "_begin, @object\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.local " #identifier "_end\n" \
	"\t.type " #identifier "_end, @object\n" \
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.popsection\n"); \
\
extern uint8_t identifier##_begin[];\
extern const uint8_t identifier##_end[]

#elif _WIN32

#define INCLUDE_BINARY(identifier,filename) \
asm(".data\n" \
	"\t.align 16\n" \
	#identifier "_begin:\n" \
	"\t.incbin \"" filename "\"\n\n" \
\
	"\t.align 1\n" \
	#identifier "_end:\n" \
	"\t.byte 0\n" \
	"\t.text\n"); \
\
extern uint8_t identifier##_begin[];\
extern const uint8_t identifier##_end[]

#else
	#error "Cannot include binary files"
#endif
}
#endif
