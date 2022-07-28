/*
simple usage:
   PNGData data = {0};
   png_read(filename, &data);
   png_data_convert(&data, 4);
   if (!data.success) {
       printf("error: %s", data.error);
       exit(-1);
   }
   unsigned char *pixels = data.pixels;
   unsigned int width = data.width, height = data.height;
   // (pixels contains RGBARGBARGBA...)
   png_data_free(&data);
   
*/
#ifndef FI_PNG_H_
#define FI_PNG_H_

#include <limits.h>
#include <stdio.h>

typedef unsigned char      FI_uint8;
typedef unsigned           FI_uint32;
typedef int                FI_int32;
#if ULONG_MAX == 0xffffffffffffffffu
typedef unsigned long      FI_uint64;
#elif ULLONG_MAX == 0xffffffffffffffffu
typedef unsigned long long FI_uint64;
#else
#error "no 64-bit unsigned type"
#endif
typedef size_t FI_uintptr;

typedef struct {
	/* INPUT */
	/* generate an error if image width x height is greater than or equal to this
	   (if 0, no limit is imposed). it is recommended to set this if
	   you are loading untrusted images. */
	FI_uint32 area_limit;
	/* perform CRC checks */
	unsigned char check_crcs;
	/* only set width,height,bit_depth,channels */
	unsigned char metadata_only;

	/* OUTPUT */
	FI_uint32 width, height;
	/*
	PIXELS
	if bit_depth == 16, this should be casted to (unsigned short *).
	if palette_size != 0, then pixels[y * width + x] is the palette index at (x, y)
	otherwise, pixels[nsamples * (y*width+x) + s] is the `s`th sample of the pixel at (x, y)
	   (e.g. for an RGB image, pixels[3 * (5*width+3) + 1] is the green channel of (3, 5))
	allocation details:
		if NULL, it will be allocated with malloc.
		if you want to use your own memory, you must do the following:
		       data->metadata_only = 1;
		       png_read_memory_rw(memory, size, data);
		       data->pixels = my_allocate(png_pixels_allocate_size(data))
		       data->metadata_only = 0;
		       png_read_memory_rw(memory, size, data);
		(you could also replace png_read_memory_rw with png_read_file, but that could be a problem
		 if someone changes the file metadata between the two calls)
		the allocation size is not what you might expect it to be, since extra memory is used temporarily.
	*/
	unsigned char *pixels;
	unsigned char success; /* 1 if successful, 0 otherwise */
	char error[127]; /* error message, or empty string if successful */
	/* 0 if image doesn't have indexed color
	  (non-indexed (e.g. RGB) images are allowed to have palettes, but this will still be 0 for those)
	*/
	unsigned short palette_size;
	unsigned char palette[256][3]; /* RGB RGB RGB RGB...*/
	/* 8 or 16 (# of bits per sample) 
	   if the original image has bit depth 1, 2, or 4,
	   it will be converted to 8 bits per sample.
	*/
	unsigned char bit_depth;
	/*
	1 = grayscale / (indexed if palette_size != 0)
	2 = grayscale+alpha
	3 = rgb
	4 = rgba
	*/
	unsigned char channels;
} PNGData;

void png_read(const char *filename, PNGData *data);
void png_read_file(FILE *fp, PNGData *data);
/*
 read PNG data loaded at `memory`.
 NOTE: to avoid allocating extra space, this WILL overwrite memory.
*/
void png_read_memory_rw(unsigned char *memory, size_t size, PNGData *data);
/*
   convert `data` to 8-bits per channel with the given number of channels
   WARNING: don't use this function if you custom-allocated data->pixels.
*/
void png_data_convert(PNGData *data, unsigned channels_out);
/* free data->pixels (don't use this if you custom-allocated data->pixels.) */
void png_data_free(PNGData *data);
/* for custom memory allocation -- given the metadata in `data`, how many bytes should be allocated for data->pixels? */
size_t png_pixels_allocate_size(const PNGData *data);

#endif /* FI_PNG_H_ */
