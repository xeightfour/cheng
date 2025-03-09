#pragma once

extern "C" {
#include <jpeglib.h>
#include <setjmp.h>
}

#include <vector>
#include <string>
#include <stdexcept>
#include <cstdio>

struct img_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

void img_error_exit(j_common_ptr cinfo) {
	img_error_mgr* err = (img_error_mgr*)cinfo->err;
	(*cinfo->err->output_message)(cinfo);
	longjmp(err->setjmp_buffer, 1);
}

class JpegLoader {
public:
	JpegLoader(const std::string& filename);
	~JpegLoader() = default;

	const std::vector<unsigned char>& getData() const {
		return data;
	}
	int getWidth() const {
		return width;
	}
	int getHeight() const {
		return height;
	}
	int getChannels() const {
		return channels;
	}

private:
	std::vector<unsigned char> data;
	int width, height, channels;
};

JpegLoader::JpegLoader(const std::string& filename) {
	struct jpeg_decompress_struct cinfo;
	struct img_error_mgr jerr;
	FILE* infile;
	JSAMPARRAY buffer;
	int row_stride;

	if ((infile = std::fopen(filename.c_str(), "rb")) == nullptr) {
		throw std::runtime_error("Can't open " + filename);
	}

	// Set up error handling
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = img_error_exit;
	if (setjmp(jerr.setjmp_buffer)) {
		jpeg_destroy_decompress(&cinfo);
		std::fclose(infile);
		throw std::runtime_error("JPEG decompression error");
	}

	// Initialize and read the JPEG
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	jpeg_read_header(&cinfo, TRUE);
	cinfo.out_color_space = JCS_RGB; // For RGB output
	jpeg_start_decompress(&cinfo);

	// Store image dimensions
	width = cinfo.output_width;
	height = cinfo.output_height;
	channels = cinfo.output_components;

	// Allocate buffer and read scanlines
	row_stride = width * channels;
	data.resize(row_stride * height);
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
	unsigned char* p = data.data();

	while (cinfo.output_scanline < height) {
		jpeg_read_scanlines(&cinfo, buffer, 1);
		std::memcpy(p, buffer[0], row_stride);
		p += row_stride;
	}

	// Clean up
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	std::fclose(infile);
}
