#include "fi_png.h"

#include <string.h>
#include <stdlib.h>
#include <assert.h>

#if _MSC_VER
#include <intrin.h>
#pragma intrinsic(__movsb)
#endif


#ifdef __has_builtin
	#if __has_builtin(__builtin_bswap32)
	#define FI_byteswap32 __builtin_bswap32
	#endif
	#if __has_builtin(__builtin_expect)
	#define FI_UNLIKELY(x) (__builtin_expect((x), 0))
	#define FI_LIKELY(x) (__builtin_expect((x), 1))
	#endif
#endif

#ifndef FI_UNLIKELY
#define FI_UNLIKELY(x) (x)
#endif
#ifndef FI_LIKELY
#define FI_LIKELY(x) (x)
#endif

#ifndef FI_byteswap32
	static FI_uint32 FI_byteswap32(FI_uint32 x) {
		return ( x        & 0xff) << 24
		     | ((x >>  8) & 0xff) << 16
		     | ((x >> 16) & 0xff) << 8
		     |  (x >> 24);
	}
#endif

#define FI_readu32le(p)  (*(const FI_uint32 *)(p))
#define FI_readu32be(p)  (FI_byteswap32(FI_readu32le(p)))

/* table of PNG CRCs for all possible 8-bit messages */
static const FI_uint32 png_crc_table[256] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

/* returns 1 if this chunk is OK 
   This function checks:
    - that the chunk data doesn't go past memory_end
    - CRCs, assuming CRC checks are enabled
*/
static int png_check_chunk(PNGData *data, const unsigned char *chunk, const unsigned char *memory_end) {
	FI_uint32 c;
	const unsigned char *crc;
	const unsigned char *p, *end;
	FI_uint32 chunk_len;
	
	if (chunk + 12 /* length,type,crc*/ > memory_end) {
		bad_png_chunk:
		strcpy(data->error, "bad PNG chunk.");
		return 0;
	}
	
	chunk_len = FI_readu32be(chunk);
	chunk += 4; /* skip length */
	
	if (chunk_len + 8 > ~(FI_uintptr)chunk)
		goto bad_png_chunk; /* addition below would overflow */
	if (chunk + chunk_len + 8/*type,crc*/ > memory_end)
		goto bad_png_chunk;
	
	if (!data->check_crcs)
		return 1;
	
	/* check CRC */
	c = 0xffffffffu;
	end = chunk + chunk_len + 4/* include type in CRC */;
	/* this could be optimized by reading multiple bytes at a time.
	   but CRC checks aren't even on by default, so whatever. */
	for (p = chunk; p < end; ++p)
		c = png_crc_table[(c ^ *p) & 0xff] ^ (c >> 8);
	c = ~c;
	
	crc = chunk + chunk_len + 4/*type*/;
	if (FI_readu32be(crc) != c) {
		strcpy(data->error, "CRC check failed.");
		return 0;
	}
	return 1;
}

/*
Huffman tables are stored as arrays of unsigned shorts.
table[i] = length | code << 7
The length of the table is 2 to the power of the max code length
So to decode some stuff follow this procedure:
   - a = the next max_code_length bits
   - entry = huffman_table[a]
   - output (entry >> 7)
   - skip (entry & 127) bits
*/
static const unsigned short deflate_default_ll_huffman_table[] = {
    32775, 10248, 2056, 35848, 34823, 14344, 6152, 24585, 33799, 12296, 4104, 20489, 8, 16392, 8200, 28681,
    33287, 11272, 3080, 18441, 35335, 15368, 7176, 26633, 34311, 13320, 5128, 22537, 1032, 17416, 9224, 30729,
    33031, 10760, 2568, 36360, 35079, 14856, 6664, 25609, 34055, 12808, 4616, 21513, 520, 16904, 8712, 29705,
    33543, 11784, 3592, 19465, 35591, 15880, 7688, 27657, 34567, 13832, 5640, 23561, 1544, 17928, 9736, 31753,
    32903, 10504, 2312, 36104, 34951, 14600, 6408, 25097, 33927, 12552, 4360, 21001, 264, 16648, 8456, 29193,
    33415, 11528, 3336, 18953, 35463, 15624, 7432, 27145, 34439, 13576, 5384, 23049, 1288, 17672, 9480, 31241,
    33159, 11016, 2824, 36616, 35207, 15112, 6920, 26121, 34183, 13064, 4872, 22025, 776, 17160, 8968, 30217,
    33671, 12040, 3848, 19977, 35719, 16136, 7944, 28169, 34695, 14088, 5896, 24073, 1800, 18184, 9992, 32265,
    32775, 10376, 2184, 35976, 34823, 14472, 6280, 24841, 33799, 12424, 4232, 20745, 136, 16520, 8328, 28937,
    33287, 11400, 3208, 18697, 35335, 15496, 7304, 26889, 34311, 13448, 5256, 22793, 1160, 17544, 9352, 30985,
    33031, 10888, 2696, 36488, 35079, 14984, 6792, 25865, 34055, 12936, 4744, 21769, 648, 17032, 8840, 29961,
    33543, 11912, 3720, 19721, 35591, 16008, 7816, 27913, 34567, 13960, 5768, 23817, 1672, 18056, 9864, 32009,
    32903, 10632, 2440, 36232, 34951, 14728, 6536, 25353, 33927, 12680, 4488, 21257, 392, 16776, 8584, 29449,
    33415, 11656, 3464, 19209, 35463, 15752, 7560, 27401, 34439, 13704, 5512, 23305, 1416, 17800, 9608, 31497,
    33159, 11144, 2952, 36744, 35207, 15240, 7048, 26377, 34183, 13192, 5000, 22281, 904, 17288, 9096, 30473,
    33671, 12168, 3976, 20233, 35719, 16264, 8072, 28425, 34695, 14216, 6024, 24329, 1928, 18312, 10120, 32521,
    32775, 10248, 2056, 35848, 34823, 14344, 6152, 24713, 33799, 12296, 4104, 20617, 8, 16392, 8200, 28809,
    33287, 11272, 3080, 18569, 35335, 15368, 7176, 26761, 34311, 13320, 5128, 22665, 1032, 17416, 9224, 30857,
    33031, 10760, 2568, 36360, 35079, 14856, 6664, 25737, 34055, 12808, 4616, 21641, 520, 16904, 8712, 29833,
    33543, 11784, 3592, 19593, 35591, 15880, 7688, 27785, 34567, 13832, 5640, 23689, 1544, 17928, 9736, 31881,
    32903, 10504, 2312, 36104, 34951, 14600, 6408, 25225, 33927, 12552, 4360, 21129, 264, 16648, 8456, 29321,
    33415, 11528, 3336, 19081, 35463, 15624, 7432, 27273, 34439, 13576, 5384, 23177, 1288, 17672, 9480, 31369,
    33159, 11016, 2824, 36616, 35207, 15112, 6920, 26249, 34183, 13064, 4872, 22153, 776, 17160, 8968, 30345,
    33671, 12040, 3848, 20105, 35719, 16136, 7944, 28297, 34695, 14088, 5896, 24201, 1800, 18184, 9992, 32393,
    32775, 10376, 2184, 35976, 34823, 14472, 6280, 24969, 33799, 12424, 4232, 20873, 136, 16520, 8328, 29065,
    33287, 11400, 3208, 18825, 35335, 15496, 7304, 27017, 34311, 13448, 5256, 22921, 1160, 17544, 9352, 31113,
    33031, 10888, 2696, 36488, 35079, 14984, 6792, 25993, 34055, 12936, 4744, 21897, 648, 17032, 8840, 30089,
    33543, 11912, 3720, 19849, 35591, 16008, 7816, 28041, 34567, 13960, 5768, 23945, 1672, 18056, 9864, 32137,
    32903, 10632, 2440, 36232, 34951, 14728, 6536, 25481, 33927, 12680, 4488, 21385, 392, 16776, 8584, 29577,
    33415, 11656, 3464, 19337, 35463, 15752, 7560, 27529, 34439, 13704, 5512, 23433, 1416, 17800, 9608, 31625,
    33159, 11144, 2952, 36744, 35207, 15240, 7048, 26505, 34183, 13192, 5000, 22409, 904, 17288, 9096, 30601,
    33671, 12168, 3976, 20361, 35719, 16264, 8072, 28553, 34695, 14216, 6024, 24457, 1928, 18312, 10120, 32649
};
/* in theory this would be 0, 1, 2, ..., 31
but DEFLATE huffman codes are big-endian and computers nowadays are little-endian */
static const unsigned short deflate_default_dist_huffman_table[] = {
    5, 2053, 1029, 3077, 517, 2565, 1541, 3589, 261, 2309, 1285, 3333, 773, 2821, 1797, 3845, 133, 2181, 1157, 3205, 645, 2693, 1669, 3717, 389, 2437, 1413, 3461, 901, 2949, 1925, 3973
};

/* huffman_table should have room for 1<<m entries,
          where m is the maximum value in code_lengths[0..ncode_lengths-1],  */
void deflate_code_lengths_to_huffman_table(const unsigned char *code_lengths,
	unsigned ncode_lengths, unsigned short *huffman_table, unsigned *huffman_table_bits) {
	unsigned i, j, l, c, max_code_length = 0;
	unsigned short bl_count[16] = {0};
	unsigned short next_code[16];
	
	if FI_UNLIKELY (ncode_lengths >= 300) goto bad;
	
	/*
	see https://datatracker.ietf.org/doc/html/rfc1951 section 3.2.2
	(most of this function is borrowed from there)
	*/
	
	/* count number of times each length occurs (in bl_count) */
	for (i = 0; i < ncode_lengths; ++i) {
		l = code_lengths[i];
		if FI_UNLIKELY (l >= 16) goto bad;
		if (l > max_code_length)
			max_code_length = code_lengths[i];
		bl_count[l] += 1;
	}
	
	{
	/* assign first code to each length */
	unsigned short code = 0;
	bl_count[0] = 0;
	for (i = 1; i <= 15; ++i) {
		code = (unsigned short)((code + bl_count[i-1]) << 1);
		next_code[i] = code;
	}
	}
	
	for (i = 0; i < ncode_lengths; ++i) {
		l = code_lengths[i];
		if (l) {
			unsigned code = next_code[l]++;
			unsigned b;
			
			unsigned jskip = 1u << l;
			unsigned jmax = 1u << max_code_length;
			unsigned short entry = (unsigned short)(i<<7 | l);
			
			
			/*
			`code` will be something like
			   10011
			and let's say max_code_length = 9
			we want to fill all huffman table entries corresponding to
			   10011XXXX
			with `entry`. however, we need to reverse the bit order, so
			we actually need to fill entries
			   XXXX11001
			to do this, we first reverse `code`:
			       11001
			then add some multiple of (1u << l).
			    + 100000 * n,  with n going from 0000 to 1111.
			*/
			
			/* reverse code into c */
			c = 0;
			for (b = 0; b < l; ++b) {
				c <<= 1;
				c |= (code >> b) & 1;
			}
			
			/* fill those entries */
			for (j = c; j < jmax; j += jskip)
				huffman_table[j] = entry;
		}
	}
	
	*huffman_table_bits = max_code_length;

	return;	
bad:
	assert(0);
	/* fuck
	this should never happen
	just put some random values there */
	*huffman_table_bits=1;
	huffman_table[0]=huffman_table[1]=1;
	return;
}

/* offset_x = byte offset from one pixel to the next, horizontally
   offset_y = byte offset from one pixel to the next, vertically */
static int png_apply_filter(PNGData *data,
	unsigned filter_type, int is_first_row,
	unsigned char *dest,
	const unsigned char *src, FI_uint32 sample_count,
	long offset_x, long offset_y) {
	
	FI_uint32 s;
	int x,a,b,c,pp,pa,pb,pc;
	
	/* @OPTIMIZE: this could be optimized by handling the first row/col separately.
	   we should check if a significant amount of time is being spent here. */
	
	switch (filter_type) {
	case 0: /* no filter */
		memmove(dest, src, sample_count);
		break;
	case 1: /* subtract */
		for (s = 0; s < sample_count; ++s) {
			x = *src++;
			a = s < offset_x ? 0 : dest[-offset_x];
			*dest++ = (unsigned char)(x + a);
		}
		break;
	case 2: /* up */
		for (s = 0; s < sample_count; ++s) {
			x = *src++;
			b = is_first_row ? 0 : dest[-offset_y];
			*dest++ = (unsigned char)(x + b);
		}
		break;
	case 3: /* average */
		for (s = 0; s < sample_count; ++s) {
			x = *src++;
			a = s < offset_x ? 0 : dest[-offset_x];
			b = is_first_row ? 0 : dest[-offset_y];
			*dest++ = (unsigned char)(x + ((a + b) >> 1));
		}
		break;
	case 4: /* paeth */
		for (s = 0; s < sample_count; ++s) {
			x = *src++;
			a = s < offset_x ? 0 : dest[-offset_x];
			b = is_first_row ? 0 : dest[-offset_y];
			c = is_first_row || s < offset_x ? 0 : dest[-offset_x-offset_y];
			pp = a + b - c;
			pa = abs(pp - a);
			pb = abs(pp - b);
			pc = abs(pp - c);
			*dest++ = (unsigned char)(x + (pa <= pb && pa <= pc
				? a
				: (pb <= pc
				? b
				: c)));
		}
		break;
	default:
		strcpy(data->error, "bad PNG filter type");
		return 0;
	}
	return 1;
}

/*
internal version of png_read_memory_rw which doesn't free data->pixels on failure
*/
static void png_read_memory_rw_internal(unsigned char *memory, size_t size, PNGData *data) {
	typedef struct {
		FI_uint32 ident1;
		FI_uint32 ident2;
		FI_uint32 ihdr_len;
		FI_uint32 ihdr_type;
		FI_uint32 width;
		FI_uint32 height;
		FI_uint8 bit_depth;
		FI_uint8 color_type;
		FI_uint8 compression;
		FI_uint8 filter;
		FI_uint8 interlace;
		FI_uint8 padding[3];
	} PNGHeader; /* sizeof(PNGHeader) == 32 */
	
	typedef struct {
		unsigned char x0, y0;
		unsigned char dx, dy;
	} InterlacePass;
	static const InterlacePass adam7_passes[] = {
		{0, 0, 8, 8},
		{4, 0, 8, 8},
		{0, 4, 4, 8},
		{2, 0, 4, 4},
		{0, 2, 2, 4},
		{1, 0, 2, 2},
		{0, 1, 1, 2},
		{0}
	};
	static const InterlacePass normal_passes[] = {
		{0, 0, 1, 1},
		{0}
	};
	
	const InterlacePass *passes;
	const unsigned char *const memory_end = memory + size;
	const unsigned char *chunk;
	unsigned char *idat_end = memory;
	PNGHeader header;
	size_t decompressed_size = 0;
	unsigned channels = 0; /* number of channels in image (1 if indexed) */
	unsigned in_bits_per_pixel = 0; /* bits per pixel in the original image (this might be as small as 1) */
	unsigned bytes_per_pixel = 0; /* bytes per pixel in output image */
	unsigned out_sample_bytes_per_scanline = 0; /* number of bytes in each row, excluding the filter type byte */
	unsigned short code_length_huffman_table[128];
	unsigned char code_lengths[318 /* = 286 ll+32 dist */];
	/* both the literal/length and distance huffman tables
	are stored here. we almost never use all 65536 entries. */
	unsigned short dynamic_huffman_tables[65536];
	
	if (size < 37) goto not_png;
	memcpy(&header, memory, sizeof header);
	header.width = FI_byteswap32(header.width);
	header.height = FI_byteswap32(header.height);
	header.ihdr_len = FI_byteswap32(header.ihdr_len);
	
	
	if (header.ident1 != 0x474e5089 /* PNG identifier 1 */
		|| header.ident2 != 0x0a1a0a0d /* PNG identifier 2 */
		|| header.ihdr_len < 13 /* IHDR must have length 13 -- we'll allow files with longer IHDR lengths */
		|| header.ihdr_type != 0x52444849 /* IHDR */
		|| header.filter != 0
		|| header.compression != 0
		|| header.interlace > 1
		|| header.width == 0 || header.height == 0
		|| (header.bit_depth != 1 && header.bit_depth != 2 && header.bit_depth != 4 && header.bit_depth != 8 && header.bit_depth != 16)
		|| (header.color_type != 0 && header.color_type != 2 && header.color_type != 3 && header.color_type != 4 && header.color_type != 6)
		) {
		not_png:
		strcpy(data->error, "not a PNG file.");
		return;
	}
	
	passes = header.interlace ? adam7_passes : normal_passes;
	
	if (!png_check_chunk(data, memory + 8, memory_end))
		return;
	
	if FI_UNLIKELY ((data->area_limit && data->area_limit / header.width >= header.height)
		|| header.width > 100000000
		|| header.height > 100000000) {
		image_too_large:
		strcpy(data->error, "image too large.");
		return;
	}
	
	switch (header.color_type) {
	case 0: channels = 1; break; /* grayscale */
	case 2: channels = 3; break; /* RGB */
	case 3: channels = 1; break; /* palette */
	case 4: channels = 2; break; /* grayscale+alpha */
	case 6: channels = 4; break; /* RGB+alpha */
	}
	
	in_bits_per_pixel = channels * header.bit_depth;
	bytes_per_pixel = channels * (unsigned)((header.bit_depth + 7) / 8);
	
	data->width = header.width;
	data->height = header.height;
	data->bit_depth = header.bit_depth == 16 ? 16 : 8;
	data->channels = (unsigned char)channels;
	
	
	out_sample_bytes_per_scanline = header.width * bytes_per_pixel;
	if (header.height >= ~(size_t)0 / (out_sample_bytes_per_scanline + 10))
		goto image_too_large;
	
	if (data->metadata_only) {
		data->success = 1;
		return;
	}
	
	if (!data->pixels) {
		data->pixels = malloc(png_pixels_allocate_size(data));
		if (!data->pixels) goto out_of_memory;
	}
	
	decompressed_size = 0;
	{ /* compute decompressed size */
		int i;
		for (i = 0; passes[i].dx; ++i) {
			const InterlacePass *pass = &passes[i];
			FI_uint32 width     = ((FI_uint32)header.width  + pass->dx-1-pass->x0) / pass->dx;
			FI_uint32 scanlines = ((FI_uint32)header.height + pass->dy-1-pass->y0) / pass->dy;
			if (width == 0) continue; /* no filter bytes for empty scanlines */
			decompressed_size +=
				(1 + (width * in_bits_per_pixel + 7) / 8) * scanlines;
		}
	}
	
	#if FI_DEBUG
	printf("%ux%u depth:%u  colortype:%u  compression:%u filter:%u  interlace:%u\n",
		header.width, header.height, header.bit_depth,
		header.color_type, header.compression, header.filter, header.interlace);
	#endif
	
	chunk = memory + 20 + header.ihdr_len;
	while (1) {
		FI_uint32 chunk_type;
		FI_uint32 chunk_length;
		if (!png_check_chunk(data, chunk, memory_end)) break;
		chunk_length = FI_readu32be(chunk);
		chunk_type = FI_readu32be(chunk + 4);
		
		if (chunk_type == 0x504c5445 /* PLTE */) {
			/* palette */
			if FI_UNLIKELY (header.color_type == 0 || header.color_type == 4) {
				strcpy(data->error, "palette in image which shouldn't have one.");
				break;
			}
			if (header.color_type == 3) {
				/* color types 2 and 6 can have PLTE chunks, but they're only relevant if
				   the truecolor image "cannot be displayed directly" */
				FI_uint32 ncolors = chunk_length / 3;
				if FI_UNLIKELY (chunk_length % 3 != 0 || ncolors == 0 || ncolors > 256) {
					strcpy(data->error, "bad palette chunk size.");
					return;
				}
				data->palette_size = (unsigned short)ncolors;
				memcpy(data->palette, chunk + 8, ncolors * 3);
			}
		} else if (chunk_type == 0x49444154 /* IDAT */) {
			/* image data */
			memmove(idat_end, chunk + 8, chunk_length);
			idat_end += chunk_length;
		} else if (chunk_type == 0x49454e44 /* IEND */) {
			/* end of image */
			break;
		} else if FI_UNLIKELY (!(chunk_type & 0x20000000)) {
			/* critical chunk but not a normal one. this is bad */
			strcpy(data->error, "unrecognized critical chunk.");
			return;
		}
		
		chunk += 12 + chunk_length;
	}
	
	if FI_UNLIKELY (idat_end - memory < 3) {
		strcpy(data->error, "not enough data in IDAT chunks.");
		return;
	}
	
	assert(!data->error[0]);
	
	{ /* deal with IDAT */
		unsigned char *out = data->pixels, *out_end = out + decompressed_size;
		unsigned char zlib_h1 = memory[0], zlib_h2 = memory[1];
		const FI_uint32 *in;
		FI_uint64 bits = 0; unsigned nbits = 0;
		unsigned block_hdr;
		const unsigned short *ll_huffman_table;
		unsigned ll_huffman_table_bits;
		const unsigned short *dist_huffman_table;
		unsigned dist_huffman_table_bits;
		
		if FI_UNLIKELY ((zlib_h1 & 0xf) != 8 || (zlib_h1 & 0x80) || (zlib_h2 & 0x20/* PNG requires no preset dictionary */)) {
			strcpy(data->error, "bad PNG zlib header.");
			return;
		}
		
		{ /* ensure `in` is aligned to 4 bytes */
			const unsigned char *p = memory + 2;
			while ((FI_uintptr)p & 3) {
				bits |= (FI_uint64)*p << nbits;
				++p;
				nbits += 8;
			}
			in = (const FI_uint32 *)p;
		}
		
		#define peek_bits(n, v) do { if (nbits < n) {\
			if FI_UNLIKELY (in >= (FI_uint32 *)memory_end) goto unexpected_eos;\
			bits |= (FI_uint64)*in << nbits;\
			nbits += 32;\
			++in;}\
			v = (FI_uint32)(bits & (((FI_uint64)1<<n)-1));\
			} while (0)
		#define get_bits(n, v) do {\
			peek_bits(n, v);\
			nbits -= n;\
			bits >>= n;\
			} while (0)
		do {
			get_bits(3, block_hdr);
			
			switch (block_hdr >> 1) {
			unsigned value, len, code;
			
			case 0: { /* no compression */
				unsigned nlen;
				unsigned trash_bits = nbits & 7;
				if (trash_bits)
					get_bits(trash_bits, value); /* skip to end of byte */
				get_bits(16, len);
				get_bits(16, nlen);
				if (out + len > out_end) goto too_much_data;
				if FI_UNLIKELY ((nlen ^ len) != 0xffff) {
					strcpy(data->error, "DEFLATE LEN and NLEN do not match.");
					return;
				}
				
				while (len > 4) {
					get_bits(32, value);
					*out++ = value & 0xff;
					*out++ = (value >> 8) & 0xff;
					*out++ = (value >> 16) & 0xff;
					*out++ = (unsigned char)((value >> 24) & 0xff);
					len -= 4;
				}
				
				while (len > 0) {
					get_bits(8, value);
					*out++ = (unsigned char)value;
					--len;
				}
				} break;
			case 1: /* fixed huffman encoding */
				ll_huffman_table = deflate_default_ll_huffman_table;
				ll_huffman_table_bits = 9;
				dist_huffman_table = deflate_default_dist_huffman_table;
				dist_huffman_table_bits = 5;
				goto compressed;
			case 2: { /* dynamic huffman encoding */
				unsigned i, hlit, hdist, hclen;
				static const unsigned char code_length_order[19] = {
					16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11,
					4, 12, 3, 13, 2, 14, 1, 15
				};
				unsigned char code_length_code_lengths[19] = {0};
				unsigned code_length_huffman_table_bits = 0;
				unsigned code_length;
				unsigned char *pcode_length, *code_lengths_end;
				
				get_bits(5, hlit); (void)hlit;
				get_bits(5, hdist); (void)hdist;
				get_bits(4, hclen);
				
				/* the huffman table is itself compressed with a dynamic huffman encoding... */
				for (i = 0; i < hclen + 4; ++i) {
					get_bits(3, code_length);
					code_length_code_lengths[code_length_order[i]] = (unsigned char)code_length;
				}
				
				#if 0
				printf("code length code lengths: ");
				for (i = 0; i < 19; ++i) {
					printf("%d:%u ",i,code_length_code_lengths[i]);
				}
				printf("\n");
				#endif
				
				deflate_code_lengths_to_huffman_table(
					code_length_code_lengths, 19,
					code_length_huffman_table, &code_length_huffman_table_bits);
				
				pcode_length = code_lengths;
				code_lengths_end = code_lengths + hlit+257 + hdist+1;
				while (pcode_length < code_lengths_end) {
					/* read a code length operation (DEFLATE RFC section 3.2.7) */
					unsigned code_length_op;
					peek_bits(code_length_huffman_table_bits, code);
					value = code_length_huffman_table[code];
					len = value & 127;
					if FI_UNLIKELY (!len) goto bad_huffman_code;
					assert(len <= code_length_huffman_table_bits);
					code_length_op = value >> 7;
					nbits -= len;
					bits >>= len;
					
					assert(code_length_op <= 18);
					if (code_length_op <= 15) {
						/* just a code length */
						*pcode_length++ = (unsigned char)code_length_op;
					} else if (code_length_op == 16) {
						/* copy previous code length */
						unsigned repetitions, repeated;
						get_bits(2, repetitions);
						repetitions += 3;
						if FI_UNLIKELY (pcode_length == code_lengths
							|| pcode_length + repetitions > code_lengths_end)
							goto bad_huffman_table;
						repeated = pcode_length[-1];
						for (i = 0; i < repetitions; ++i)
							*pcode_length++ = (unsigned char)repeated;
					} else if (code_length_op == 17) {
						/* a buncha zeros */
						unsigned repetitions;
						get_bits(3, repetitions);
						repetitions += 3;
						if FI_UNLIKELY (pcode_length + repetitions > code_lengths_end)
							goto bad_huffman_table;
						for (i = 0; i < repetitions; ++i)
							*pcode_length++ = 0;
					} else if (code_length_op == 18) {
						/* a buncha zeros */
						unsigned repetitions;
						get_bits(7, repetitions);
						repetitions += 11;
						if FI_UNLIKELY (pcode_length + repetitions > code_lengths_end)
							goto bad_huffman_table;
						for (i = 0; i < repetitions; ++i)
							*pcode_length++ = 0;
					}
				}
				if (pcode_length != code_lengths_end) goto bad_huffman_table;
				
				deflate_code_lengths_to_huffman_table(
					code_lengths, hlit+257,
					dynamic_huffman_tables, &ll_huffman_table_bits);
				ll_huffman_table = dynamic_huffman_tables;
				
				deflate_code_lengths_to_huffman_table(
					code_lengths + hlit+257, hdist+1,
					dynamic_huffman_tables + (1u<<ll_huffman_table_bits), &dist_huffman_table_bits);
				dist_huffman_table = dynamic_huffman_tables + (1u<<ll_huffman_table_bits);
				goto compressed;
			} break;
			compressed:
				while (1) {
					unsigned literal_length;
					peek_bits(ll_huffman_table_bits, code);
					value = ll_huffman_table[code];
					len = value & 127;
					if FI_UNLIKELY (!len) goto bad_huffman_code;
					assert(len <= ll_huffman_table_bits);
					literal_length = value >> 7;
					nbits -= len;
					bits >>= len;
					if (literal_length == 256) break;
					
					if (literal_length < 256) {
						if FI_UNLIKELY (out >= out_end) goto too_much_data;
						*out++ = (unsigned char)literal_length;
						#if 0
						printf("OUT: %u\n",literal_length);
						#endif
					} else {
						unsigned extra_bit_count, copy_length, distance_code, distance;
						const unsigned char *copy_from;
						
						if (literal_length < 265) {
							copy_length = literal_length - 254;
						} else if (literal_length < 285) {
							static const unsigned char base[20] = {
								11, 13, 15, 17,
								19, 23, 27, 31,
								35, 43, 51, 59,
								67, 83, 99, 115,
								131, 163, 195, 227
							};
							extra_bit_count = (literal_length - 261) >> 2;
							get_bits(extra_bit_count, copy_length);
							copy_length += base[literal_length - 265];
						} else if (literal_length == 285) {
							copy_length = 258;
						} else {
							goto bad_huffman_code;
						}
						
						peek_bits(dist_huffman_table_bits, code);
						value = dist_huffman_table[code];
						len = value & 127;
						if FI_UNLIKELY (!len) goto bad_huffman_code;
						assert(len <= dist_huffman_table_bits);
						distance_code = value >> 7;
						nbits -= len;
						bits >>= len;
						
						if (distance_code < 4) {
							distance = distance_code + 1;
						} else if (distance_code < 30) {
							static const unsigned short base[26] = {
								5, 7, 9, 13, 17, 25,
								33, 49, 65, 97, 129, 193,
								257, 385, 513, 769, 1025, 1537,
								2049, 3073, 4097, 6145, 8193, 12289,
								16385, 24577
							};
							extra_bit_count = (distance_code - 2) >> 1;
							get_bits(extra_bit_count, distance);
							distance += base[distance_code - 4];
						} else {
							goto bad_huffman_code;
						}
						
						if FI_UNLIKELY (out < data->pixels + distance)
							goto bad_distance;
						if FI_UNLIKELY (out + copy_length > out_end)
							goto too_much_data;
						
						copy_from = out - distance;
						/*
						 now we want to copy `copy_length` bytes from `copy_from` to `out`.
						 we can't just do a memcpy because the source & dest can overlap.
						 fortunately x86 `rep movsb` has exactly the behavior we want. */
						#if __GNUC__ && (__x86_64__ || __i386__)
							__asm__ volatile ("rep movsb"
					                : "=D" (out),
					                  "=S" (copy_from),
					                  "=c" (copy_length)
					                : "0" (out),
					                  "1" (copy_from),
					                  "2" (copy_length)
					                : "memory");
					                out += copy_length;
					                copy_from += copy_length;
				                #elif _MSC_VER && (_M_IX86 || _M_X64)
				                	__movsb(out, copy_from, copy_length);
					                out += copy_length;
					                copy_from += copy_length;
				                #else
							for (i = 0; i < copy_length; ++i)
								*out++ = *copy_from++;
						#endif
					}
				}
				break;
			default:
				strcpy(data->error, "bad DEFLATE block header.");
				return;
			}
		} while (!(block_hdr & 1));
		
		if ((size_t)(out - data->pixels) != decompressed_size) {
			strcpy(data->error, "not enough image data in DEFLATE stream.");
			return;
		}
		
		
		if (data->check_crcs) {
			FI_uint32 adler32 = 0;
			FI_uint32 value;
			FI_uint32 s1 = 1, s2 = 0;
			unsigned i, trash;
			const unsigned char *p;
		
			trash = nbits & 7;
			get_bits(trash, value); /* skip to next byte boundary */
			
			get_bits(32, value);
			/* convert endianness */
			for (i = 0; i < 32; i += 8)
				adler32 <<= 8, adler32 |= (value>>i) & 0xff;
			
			/* compute adler-32 of uncompressed data */
			for (p = data->pixels; p < out_end; ++p) {
				s1 += *p;
				if (s1 > 65521) s1 -= 65521;
				s2 += s1;
				if (s2 > 65521) s2 -= 65521;
			}
			if (adler32 != (s2 << 16 | s1)) {
				strcpy(data->error, "bad zlib checksum.");
				return;
			}
		}
	}
	
	{ /* apply filters */
		const unsigned char *p;
		unsigned char *out;
		unsigned pass_idx;
		
		#if 0
		{
		FI_uint32 y,s;
		printf("UNFLITERED:\n");
		p = data->pixels;
		if (header.interlace) {
			for (s = 0; s < decompressed_size; ++s) {
				printf("%02x ", *p++);
				if (s % 16 == 15) printf("\n");
			}
			printf("\n");
		} else {
		
		
			for (y = 0; y < header.height; ++y) {
				printf("%02x|", *p++); /* filter byte */
				for (s = 0; s < out_sample_bytes_per_scanline; ++s)
					printf("%02x ",*p++);
				printf("\n");
			}
		}
		}
		#endif
		
		p = data->pixels;
		out = data->pixels;
		/* NB: we are not deinterlacing yet
		       but interlacing matters here because sub-images have their own filter bytes.
		      (we have to split it into a separate stage because we're reading
		       from the same buffer we're writing to)
		 */
		for (pass_idx = 0; passes[pass_idx].dx; ++pass_idx) {
			const InterlacePass *pass = &passes[pass_idx];
			FI_uint32 y;
			FI_uint32 width     = ((FI_uint32)header.width  + pass->dx-1-pass->x0) / pass->dx;
			FI_uint32 scanlines = ((FI_uint32)header.height + pass->dy-1-pass->y0) / pass->dy;
			FI_uint32 in_sample_bytes_per_scanline = (width * in_bits_per_pixel + 7) / 8;
			long offset_x = (in_bits_per_pixel + 7) / 8;
			long offset_y = in_sample_bytes_per_scanline;
			
			if (!width) continue;
			
			for (y = 0; y < scanlines; ++y) {
				unsigned filter_type = *p++;
				if (!png_apply_filter(data, filter_type, y == 0,
					out,
					p, in_sample_bytes_per_scanline,
					offset_x, offset_y))
					return;
				p += in_sample_bytes_per_scanline;
				out += in_sample_bytes_per_scanline;
			}		
		}
	}
	
	if (header.bit_depth < 8) {
		/* expand 1/2/4 bit-per-sample images to 8 bits per sample
		we do this ``in reverse'' because we are writing to the same
		buffer we're reading from */
		FI_uint32 i, y;
		size_t in_bytes_per_row = (header.width * in_bits_per_pixel + 7) / 8;
		unsigned char *out;
		const unsigned char *in;
		
		for (y = header.height - 1; y + 1 > 0; --y) {
			unsigned char bits = 0;
			int nbits = 0;
			/* start on the last pixel of row y */
			out = data->pixels + ((size_t)header.width * (y+1) - 1);
			in = data->pixels + (in_bytes_per_row * (y+1) - 1);
			
			if ((header.width * header.bit_depth) % 8) {
				/* row ends in the middle of a byte */
				bits = *in--;
				nbits = (header.width * header.bit_depth) % 8;
				bits >>= 8-nbits;
			}
			
			for (i = 0; i < header.width; ++i) {
				if (nbits == 0) {
					nbits = 8;
					bits = *in--;
				}
				*out-- = (unsigned char)(bits & ((1u << header.bit_depth)-1));
				nbits -= header.bit_depth;
				bits >>= header.bit_depth;
			}
		}
	}
	
	
	if (header.interlace) {
		size_t image_bytes = (size_t)header.height * out_sample_bytes_per_scanline;
		unsigned char *deinterlaced, *out;
		const unsigned char *in;
		FI_uint32 x, y;
		int pass_idx;
		
		/*
		we don't de-interlace in place.
		  most PNGs aren't interlaced, though.
		*/
		deinterlaced = malloc(image_bytes);
		if (!deinterlaced) goto out_of_memory;
		
		in = data->pixels;
		for (pass_idx = 0; passes[pass_idx].dx; ++pass_idx) {
			const InterlacePass *pass = &passes[pass_idx];
			unsigned x0 = pass->x0, y0 = pass->y0, dx = pass->dx, dy = pass->dy;
			FI_uint32 w = header.width, h = header.height;
			
			
			for (y = y0; y < h; y += dy) {
				out = &deinterlaced[bytes_per_pixel*(y*w+x0)];
				for (x = x0; x < w; x += dx) {
					memcpy(out, in, bytes_per_pixel);
					in += bytes_per_pixel;
					out += bytes_per_pixel*dx;
				}
			}
		}
		
		memcpy(data->pixels, deinterlaced, image_bytes);
		free(deinterlaced);
	}
	
	
	if (data->palette_size) {
		/* check palette indices */
		unsigned palette_size = data->palette_size;
		const unsigned char *p, *p_end;
		
		unsigned char byte;
		
		p = data->pixels;
		p_end = p + (FI_uint64)header.height * out_sample_bytes_per_scanline;
		
		while (p < p_end) {
			byte = *p++;
			if FI_UNLIKELY (byte >= palette_size)
				goto bad_palette_index;
		}
	}
	
	
	if (!data->error[0]) data->success = 1;
	return;
	bad_huffman_table:
		strcpy(data->error, "bad DEFLATE huffman table.");
		return;
	unexpected_eos:
		strcpy(data->error, "unexpected end of DEFLATE stream.");
		return;
	too_much_data:
		strcpy(data->error, "too much data in DEFLATE stream.");
		return;
	bad_huffman_code:
		strcpy(data->error, "bad DEFLATE huffman code.");
		return;
	bad_distance:
		strcpy(data->error, "bad lz77 reference");
		return;
	bad_palette_index:
		strcpy(data->error, "bad palette index");
		return;
	out_of_memory:
		strcpy(data->error, "out of memory");
		return;
}

size_t png_pixels_allocate_size(const PNGData *data) {
	/* in adam7-interlaced images, there can be up to
	   4 filter bytes for a single row of pixels in the output image.
	   in practice, this is a slight over-allocation, but whatever. */
	return data->height * ((size_t)data->width * (data->bit_depth / 8) * data->channels + 4);
}

void png_read_memory_rw(unsigned char *memory, size_t size, PNGData *data) {
	png_read_memory_rw_internal(memory, size, data);
	if (!data->success) {
		free(data->pixels);
		data->pixels = 0;
	}
}

void png_read_file(FILE *fp, PNGData *data) {
	void *memory = 0;
	size_t file_size = 0;
	
	if (!fp) {
		strcpy(data->error, "couldn't open file");
		return;
	}
	
	fseek(fp, 0, SEEK_END);
	file_size = (size_t)ftell(fp);
	fseek(fp, 0, SEEK_SET);
	
	/* we could mmap the memory (with PROT_WRITE and MAP_SHARED),
	   but that's slower in practice */
	
	memory = malloc(file_size);
	if (fread(memory, 1, file_size, fp) != file_size) {
		strcpy(data->error, "error reading file");
		free(memory);
		return;
	}
	
	png_read_memory_rw(memory, file_size, data);
	free(memory);
}

void png_read(const char *filename, PNGData *data) {
	FILE *fp = fopen(filename, "rb");
	if (!fp) {
		strcpy(data->error, "couldn't open file.");
		return;
	}
	png_read_file(fp, data);
	fclose(fp);
}

void png_data_free(PNGData *data) {
	free(data->pixels);
	data->pixels = NULL;
}

void png_data_convert(PNGData *data, unsigned channels_out) {
	size_t s;
	FI_uint32 x, y, width = data->width, height = data->height,
		channels_in = data->channels;
	unsigned char *output, *out;
	const unsigned char *p;
	
	if (*data->error) return;
	
	if FI_UNLIKELY (channels_out < 1 || channels_out > 4) {
		strcpy(data->error, "bad channel count for conversion.");
		data->success = 0;
		return;
	}
	
	if (data->bit_depth == 16) {
		/* first, reduce image bit depth to 8 */
		size_t samples = (size_t)width * height * channels_in;
		
		out = data->pixels;
		p = data->pixels;
		
		for (s = 0; s < samples; ++s) {
			++p; /* skip lower-order byte */
			*out++ = *p++;
		}
	}
	
	if (channels_in == channels_out && !data->palette_size)
		return; /* nothing to do */
	
	output = malloc((size_t)width * (size_t)height * (size_t)channels_out);
	if FI_UNLIKELY (!output) {
		strcpy(data->error, "not enough memory to allocate output buffer.");
		data->success = 0;
		return;
	}
	
	
	out = output;
	p = data->pixels;
	for (y = 0; y < height; ++y) {
		for (x = 0; x < width; ++x) {
			unsigned char r, g, b, a = 255;
			r = *p++;
			switch (channels_in) {
			case 1:
				if (data->palette_size) {
					const unsigned char *c = data->palette[r];
					r = c[0];
					g = c[1];
					b = c[2];
				} else {
					g = b = r;
				}
				break;
			case 2:
				g = b = r;
				a = *p++;
				break;
			case 3:
				g = *p++;
				b = *p++;
				break;
			case 4:
				g = *p++;
				b = *p++;
				a = *p++;
				break;
			default: r=g=b=0; break; // MSVC is complaining that stuff is uninitialized
			}
			
			
			/* output */
			switch (channels_out) {
			case 1:
				*out++ = (unsigned char)((r + g + b) / 3);
				break;
			case 2:
				*out++ = (unsigned char)((r + g + b) / 3);
				*out++ = a;
				break;
			case 3:
				*out++ = r;
				*out++ = g;
				*out++ = b;
				break;
			case 4:
				*out++ = r;
				*out++ = g;
				*out++ = b;
				*out++ = a;
				break;
			}
		}
	}
	
	free(data->pixels);
	data->pixels = output;
	data->channels = (unsigned char)channels_out;
	data->bit_depth = 8;
	data->palette_size = 0;
}
