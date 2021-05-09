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
using namespace std;

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

struct Color{
	union {
		struct {
			uint8_t b_, g_, r_, a_;
		};
		uint8_t row_[4];
		uint32_t val_;
	};
	Color() {

	}

	Color(const uint8_t R,const uint8_t G, const uint8_t B, const uint8_t A = 255) : b_(B), g_(G), r_(R), a_(A) {
	}

	Color(const uint8_t* p,int bpp) : val_(0) {
		for (int i = 0; i < bpp; i++) {
			row_[i] = p[i];
		}
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
	uint32_t width_;
	uint32_t height_;
	uint32_t bytes_per_pixel_;
public:
	TGAImage();
	TGAImage(const char* filename);
	TGAImage(int width, int height, int bpp);
	TGAImage(const TGAImage& img);
	bool writeTGAFile(const char* filename, bool rle = true);
	Color get(int x,int y);// origin pixel (0,0)
	void set(int x,int y,Color val);// origin pixel (0,0)
	void flipVertically(); // origin is left top (0,0)
	void flipHorizontally();
	uint32_t getWidth();
	uint32_t getHeight();
private:	
	bool readTGAFile(const char* filename);
	void readHeader(ifstream& in);
	void readImageId(ifstream& in);
	void readColorMap(ifstream& in);
	void readImageData(ifstream& in);
	void readEncodedData(ifstream& in,const uint64_t& bytes_num);
	void writeHeader(ofstream& out,bool rle);
	void writeImageData(ofstream& out,bool rle);
	void writeEncodedData(ofstream& out,const uint64_t& bytes_num);
	bool isEncoded();
};

} // aba

