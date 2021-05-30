#include "tgaimage.h"
#include <fstream>
#include <iostream>

namespace aba {

TGAImage::TGAImage()
{
	bytes_per_pixel_ = width_ = height_ = 0;
	image_data_ = image_id_ = nullptr;
}

TGAImage::TGAImage(const char* filename)
{
	TGAImage();
	readTGAFile(filename);
}

TGAImage::TGAImage(int w, int h, int bpp) : width_(w), height_(h), bytes_per_pixel_(bpp)
{
	int64_t bytes_num = static_cast<int64_t>(w) * h * bpp;
	image_data_ = new uint8_t[bytes_num];
	image_id_ = nullptr;
	memset(image_data_, 0, bytes_num);
}

TGAImage::TGAImage(const TGAImage& img)
{
	width_ = img.width_;
	height_ = img.height_;
	header_ = img.header_;
	bytes_per_pixel_ = img.bytes_per_pixel_;
	image_id_ = img.image_id_;
	int64_t bytes_num = static_cast<int64_t>(width_) * height_ * bytes_per_pixel_;
	image_data_ = new uint8_t[bytes_num];
	memmove(image_data_, img.image_data_, bytes_num);
}

bool TGAImage::readTGAFile(const char* filename)
{
	std::ifstream in;
	try {
		in.open(filename, std::ios::binary);
		if (!in.is_open()) {
			throw "the file can't open";
			in.close();
		}
		readHeader(in);
		readImageId(in);
		readColorMap(in);
		readImageData(in);
		//let's image data's origin be top-left
		//bit 5 is 0,the origin is in at the bottom.
		if ((header_.image_desc & 0x20 ) == 0) {
			flipVertically();
		}
		//bit 4 is 1,the origin is on the right
		if (header_.image_desc & 0x10) {
			flipHorizontally();
		}
	}catch (const char* msg){
		std::cerr << msg << std::endl;
		in.close();
		return false;
	}
	in.close();
	return true;
}

uint8_t* TGAImage::getImageBuffer()
{
	return image_data_;
}

void TGAImage::clear()
{
	int64_t bytes_num = static_cast<int64_t>(width_) * height_ * bytes_per_pixel_;
	memset(image_data_, 0, bytes_num);
}

void TGAImage::readHeader(std::ifstream &in)
{
	in.read((char*) &header_, sizeof(header_));
	if (in.fail()) {
		throw "can't open file";
	}
	width_ = header_.image_width;
	height_ = header_.image_height;
	bytes_per_pixel_ = header_.pixel_depth >> 3; // (bits -> bytes)
	if (width_ < 0 || height_ < 0 || (bytes_per_pixel_ != RGB && bytes_per_pixel_ != RGBA && bytes_per_pixel_ != GRAYSCALE)) {
		throw "an error occured while reading the header\n";
	}
}

void TGAImage::readImageId(std::ifstream& in)
{
	int len = header_.id_length;
	if (len > 0) {
		image_id_ = new uint8_t[len];
		in.read((char*)image_id_, len);
	}
}

void TGAImage::readColorMap(std::ifstream& in)
{
	if (header_.color_map_type == 0) return;
	throw "Don't support Color Map filed";
}

void TGAImage::readImageData(std::ifstream& in)
{
	int32_t bytes_num = width_*height_*bytes_per_pixel_;
	if (isEncoded()) {
		readEncodedData(in,bytes_num);
	}
	else {
		// 1 char = 1 bytes
		image_data_ = new uint8_t[bytes_num];
		in.read((char*)image_data_,bytes_num);
		if (in.fail()) {
			throw "an error occur in reading the img data";
		}
	}
}

void TGAImage::readEncodedData(std::ifstream& in, const uint64_t& bytes_num)
{
	uint64_t cur_byte = 0;
	image_data_ = new uint8_t[bytes_num];
	char* raw_val = new char[bytes_per_pixel_];
	while (cur_byte < bytes_num) {
		// first filed is repetition count filed (1 bytes)
		uint8_t filed = 0;
		in.read((char*)&filed, sizeof(uint8_t));

		// when highest bit is 1 , the pack is Run-length packet
		//				    is 0 , the pack is Raw packet
		// the lower 7 bit is the value that 1 less than actual pixels num
		int pixel_num;
		if ((filed & 0x80) != 0) { // Run-length packet
			pixel_num = (filed & ~0x80) + 1;

			in.read((char*)raw_val, bytes_per_pixel_);
			for (int i = 0; i < pixel_num; i++) {
				memcpy(image_data_ + cur_byte, raw_val,bytes_per_pixel_);
				cur_byte += bytes_per_pixel_;
			}
		}
		else {
			//highet bit is 0
			pixel_num = filed + 1;
			uint32_t pixel_bytes_num;
			pixel_bytes_num = pixel_num * bytes_per_pixel_;
			//read the repeat pixel
			in.read((char*)(image_data_ + cur_byte), pixel_bytes_num);
			cur_byte += pixel_bytes_num;
		}

		if (!in.good()) {
			throw "an error occur in reading the img data";
		}
	}
}

bool TGAImage::writeTGAFile(const char* filename, bool rle)
{
	std::ofstream out;
	try {
		out.open(filename, std::ios::binary);
		if (out.fail()) {
			throw "can't open the file";
		}
		//Header
		writeHeader(out,rle);
		//Image Data
		writeImageData(out,rle);
		//Developer Area Extension Area TGA File Footer (OPTIONS)
	}
	catch (const char* msg) {
		std::cerr << msg << std::endl;
		out.close();
		return false;
	}
	out.close();
	return true;
}

void TGAImage::writeHeader(std::ofstream &out,bool rle)
{
	TGAHeader header;
	memset((void*) &header, 0, sizeof(header));
	header.pixel_depth = bytes_per_pixel_ << 3;
	header.image_height = height_;
	header.image_width = width_;
	header.image_desc = 0x20;//top-left origin
	header.image_type = (bytes_per_pixel_ == GRAYSCALE) ?
		(rle ? RLEGrayScale : RawGrayScale) : 
		(rle ? RLETrueMap : RawTrueMap);
	out.write((char*)& header, sizeof(header));
	if (out.fail()) {
		throw "can't wrtie tga header";
	}
}

void TGAImage::writeImageData(std::ofstream& out,bool rle)
{
	uint64_t pixel_num = static_cast<uint64_t>(width_) * height_;
	if (rle) {
		writeEncodedData(out,pixel_num);
	}
	else {
		out.write((char*) image_data_, pixel_num * bytes_per_pixel_);
		if (out.fail()) {
			throw "can't wrtie tga image data";
		}
	}
}

void TGAImage::writeEncodedData(std::ofstream& out,const uint64_t& pixel_num)
{
	uint64_t cur_pixel = 0;
	//the Raw and RLE pack all store no more than 128 pixels
	const int MAX_RUN_LENGTH = 128;

	while (cur_pixel < pixel_num) {
		//get the successive pixels are all different or same.
		uint8_t run_length = 1;
		bool raw = false;
		uint64_t cur_bytes = cur_pixel * bytes_per_pixel_;
		uint64_t t_cur_bytes = cur_bytes;
		while (cur_pixel + run_length < pixel_num && run_length < MAX_RUN_LENGTH) {
			bool same = true;
			//check the whether the two pixels are same
			for (int i = 0; i < bytes_per_pixel_; i++) {
				uint64_t left_pixel_pos = cur_bytes + i;
				uint64_t right_pixel_pos = cur_bytes + bytes_per_pixel_ + i;
				if (image_data_[left_pixel_pos] != image_data_[right_pixel_pos]) {
					same = false;
					break;
				}
			}
			cur_bytes += bytes_per_pixel_;
			
			if (!same && run_length == 1) {
				raw = true;
			}
			if (raw && same) {
				// ¡ý
				//ABB (AB is raw,but BB is RLE, we should run back)
				run_length--;
				break;
			}
			if (!raw && !same) {
				break;
			}
			run_length++;
		}

		// when highest bit indicate the packet type
		//				is 0 , the pack is Raw packet
		//				is 1 , the pack is Run-length
		// the lower 7 bit indicate the pixels num (NOTE: 1 less than actual pixels num)
		// pixels num range from 0 to 127
		// 
		// write the packet type and pixels num.
		out.put((char) (raw? run_length-1: run_length + 127));
		if (out.fail()) {
			throw "write the packet first filed error";
		}
		// write the pixel value.
		out.write((char*)(image_data_ + t_cur_bytes), raw? bytes_per_pixel_*run_length : bytes_per_pixel_);
		if (out.fail()) {
			throw "write the packet second filed error";
		}
		cur_pixel += run_length;
	}
}

bool TGAImage::isEncoded()
{
	switch (header_.image_type)
	{
	case RLEColorMap:
	case RLEGrayScale:
	case RLETrueMap:
		return true;
	case RawColorMap:
	case RawGrayScale:
	case RawTrueMap:
		return false;
	default:
		break;
	}
	return false;
}

void TGAImage::flipVertically()
{
	int bytes_per_line = width_ * bytes_per_pixel_;
	uint8_t* tmp_line = new uint8_t[bytes_per_line];
	for (int i = 0; i < height_ / 2; i++) {
		int j1 = i * bytes_per_line;
		int j2 = (height_ - i - 1) * bytes_per_line;
		memmove(tmp_line, image_data_ + j1, bytes_per_line);
		memmove(image_data_ + j1, image_data_ + j2, bytes_per_line);
		memmove(image_data_ + j2, tmp_line, bytes_per_line);
	}
	delete[] tmp_line;
}

//tranfer BGR encoding to RGB
void TGAImage::flipHorizontally()
{
	// bit 4 is 1
	if (header_.image_desc & 0x10) {
		int bytes_per_line = width_ * bytes_per_pixel_;
		uint8_t* tmp_line = new uint8_t[bytes_per_line];
		for (int i = 0; i < height_; i++) {
			for (int j = 0; j < width_/2; j++) {
				int pos_left = j;
				int pos_right = width_ - j - 1;
				Color left_pixel = get(i, pos_left);
				Color right_pixel = get(i, pos_right);
				set(i, pos_left, right_pixel);
				set(i, pos_right, left_pixel);
			}
		}
		delete[] tmp_line;
	}
}

uint32_t TGAImage::getWidth() const
{
	return width_;
}

uint32_t TGAImage::getHeight() const
{
	return height_;
}

// origin pixel (0,0) left-bottom
Color TGAImage::get(int x, int y) const
{
	if (!image_data_ || y < 0 || x < 0 || y >= width_ || x >= height_) {
		return Color();
	}
	int trans_y = height_ - 1 - y;// make y start from buttom.
	uint64_t offset = (static_cast<uint64_t>(trans_y) * width_ + x) * bytes_per_pixel_;
	return Color( image_data_ + offset, bytes_per_pixel_);
}

// origin pixel (0,0) left-bottom
void TGAImage::set(int x, int y, Color val)
{
	if (!image_data_ || y < 0 || x < 0 || y >= width_ || x >= height_) {
		throw "out of image boundary";
	}
	int trans_y = height_ - 1 - y;// make y start from buttom.
	uint64_t offset = (static_cast<uint64_t>(trans_y) * width_ + x) * bytes_per_pixel_;
	memcpy(image_data_ + offset, val.row_, bytes_per_pixel_);
}

} // aba

