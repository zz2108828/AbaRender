/** 
	***********************************************************
	******** My code don't support Color Map Image now ********
	***********************************************************

	TGA format specification
	http://www.dca.fee.unicamp.br/~martino/disciplinas/ea978/tgaffs.pdf
	http://www.opennet.ru/docs/formats/targa.pdf

	code ref:
	https://github.com/image-rs/image/blob/master/src/codecs/tga/header.rs
**/
#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

namespace aba{

enum Format {
	GRAYSCALE = 1, RGB = 3, RGBA = 4
};

enum TGAImageType : uint8_t{
	NoImageData = 0,
	//uncompressed image
	RawColorMap = 1,
	RawTrueMap = 2,
	RawGrayScale = 3,
	//Run-length encoded image
	RLEColorMap = 9,
	RLETrueMap = 10,
	RLEGrayScale = 11,
	unknown
};

//Make sure header is 18 bytes
#pragma pack(push,1)
struct TGAHeader {
	uint8_t  id_length;
	uint8_t  color_map_type;
	TGAImageType image_type; // uint8_t
	int16_t color_map_origin;
	int16_t color_map_length;
	uint8_t  color_map_entry_size;// size of map entries in bits
	int16_t x_origin;
	int16_t y_origin;
	int16_t image_width;
	int16_t image_height;
	uint8_t  pixel_depth;// bits per pixel(bpp)
	uint8_t  image_desc;// image descriptor
};
#pragma pack(pop)

//support RBG,RGBA
struct Color{
	union {
		struct {
			uint8_t b_, g_, r_, a_;
		};
		uint8_t row_[4];
		uint32_t val_;
	};
	Color() :b_(0), g_(0), r_(0), a_(255) {

	}

	Color(unsigned char v) :b_(0), g_(0), r_(0), a_(255) {
		for (int i = 0; i < 4; i++) row_[i] = 0;
		row_[0] = v;
	}


	Color(const uint8_t R,const uint8_t G, const uint8_t B, const uint8_t A = 255) : b_(B), g_(G), r_(R), a_(A) {
	}

	Color(const uint8_t* p,int bpp) : val_(0) {
		for (int i = 0; i < bpp; i++) {
			row_[i] = p[i];
		}
	}

	Color operator *(float intensity) const {
		Color color(*this);
		intensity = (intensity < 0.f ? 0.f : intensity);
		intensity = (intensity > 1.f ? 1.f : intensity);
		for (int i = 0; i < 4; i++) {
			color.row_[i] = static_cast<uint8_t>(color.row_[i] * intensity);
		}
		return color;
	}

	friend Color operator* (float intensity,const Color& rhs ){
		Color color(rhs);
		color = rhs * intensity;
		return color;
	}

	uint8_t& operator[](int x) {
		return row_[x];
	}

	Color& operator =(const Color& c) {
		if (this != &c) {
			val_ = c.val_;
		}
		return *this;
	}
};

class TGAImage {
private:
	TGAHeader header_ = {0};
	uint8_t* image_id_;
	uint8_t* image_data_ = nullptr; // origin is left top (0,0)
	int width_;
	int height_;
	int bytes_per_pixel_;
public:
	TGAImage();
	TGAImage(const char* filename);
	TGAImage(int width, int height, int bpp);
	TGAImage(const TGAImage& img);
	bool writeTGAFile(const char* filename, bool rle = true);
	Color get(int x,int y) const;// origin pixel (0,0)
	void set(int x,int y,Color val);// origin pixel (0,0)
	void flipVertically(); // origin is left top (0,0)
	void flipHorizontally();
	uint32_t getWidth() const;
	uint32_t getHeight() const;
	bool readTGAFile(const char* filename);
	uint8_t* getImageBuffer();
	void clear();
private:	
	void readHeader(std::ifstream& in);
	void readImageId(std::ifstream& in);
	void readColorMap(std::ifstream& in);
	void readImageData(std::ifstream& in);
	void readEncodedData(std::ifstream& in,const uint64_t& bytes_num);
	void writeHeader(std::ofstream& out,bool rle);
	void writeImageData(std::ofstream& out,bool rle);
	void writeEncodedData(std::ofstream& out,const uint64_t& bytes_num);
	bool isEncoded();
};

} // aba

