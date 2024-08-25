#pragma once

#include <jpeglib.h>
#include <jerror.h>

#include <print>

struct Image {
	unsigned int numComponents;
	unsigned int width, height;
	unsigned char *data;
};

Image *acsLoadImageFile(const char *fileName) {
	jpeg_decompress_struct deck;
	jpeg_error_mgr err;

	Image *newImage;

	unsigned int imgWidth, imgHeight, imgComponents;

	unsigned int bufferBytes;
	unsigned char *imgData;

	unsigned char *rowBuffer[1];

	FILE *file = fopen(fileName, "rb");
	if (file == nullptr) {
		std::print("[ERROR] File {}, line {},\nFailed to read file {}\n", __FILE__, __LINE__, fileName);
		return nullptr;
	}

	deck.err = jpeg_std_error(&err);
	jpeg_create_decompress(&deck);

	jpeg_stdio_src(&deck, file);
	jpeg_read_header(&deck, true);

	jpeg_start_decompress(&deck);

	imgWidth = deck.output_width;
	imgHeight = deck.output_height;
	imgComponents = deck.num_components;

	std::print("Read JPEG with dimensions {} x {} and {} components\n", imgWidth, imgHeight, imgComponents);

	bufferBytes = imgWidth * imgHeight * 3;
	imgData = (unsigned char*)malloc(sizeof(unsigned char) * bufferBytes);

	newImage = (Image*)malloc(sizeof(Image));

	newImage->numComponents = imgComponents;
	newImage->width = imgWidth;
	newImage->height = imgHeight;
	newImage->data = imgData;

	while(deck.output_scanline < deck.output_height) {
		rowBuffer[0] = (unsigned char*)(&imgData[3 * deck.output_width * deck.output_scanline]);
		jpeg_read_scanlines(&deck, rowBuffer, 1);
	}

	jpeg_finish_decompress(&deck);
	jpeg_destroy_decompress(&deck);
	fclose(file);

	return newImage;
}

int acsSaveImageFile(Image *image, const char *fileName, int quality = 90) {
	jpeg_compress_struct deck;
	jpeg_error_mgr err;

	unsigned char *rowBuffer[1];

	FILE *file = fopen(fileName, "wb");
	if(file == nullptr) {
		std::print("[ERROR] File {}, line {},\nFailed to open output file {}\n", __FILE__, __LINE__, fileName);
		return 1;
	}

	deck.err = jpeg_std_error(&err);
	jpeg_create_compress(&deck);

	jpeg_stdio_dest(&deck, file);

	deck.image_width = image->width;
	deck.image_height = image->height;
	deck.input_components = 3;
	deck.in_color_space = JCS_RGB;

	jpeg_set_defaults(&deck);
	jpeg_set_quality(&deck, quality, true);

	jpeg_start_compress(&deck, true);

	while(deck.next_scanline < deck.image_height) {
		rowBuffer[0] = &(image->data[deck.next_scanline * (image->width * 3)]);
		jpeg_write_scanlines(&deck, rowBuffer, 1);
	}

	jpeg_finish_compress(&deck);
	jpeg_destroy_compress(&deck);
	fclose(file);

	return 0;
}

void acsFreeImage(Image *image) {
	free(image->data);
	free(image);
}
