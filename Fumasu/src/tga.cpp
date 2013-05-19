#include "Fumasu/tga.h"

namespace Fumasu {

void makeScreenshot(string filename, uint16_t width, uint16_t height, uint8_t bpp, uint8_t *data) {
	fstream datei;
	struct TGAHEADER {
		unsigned char ID_length;
		unsigned char colorMapType;
		unsigned char imageType;
		unsigned char colorMap[5];
		uint16_t xOrigin;
		uint16_t yOrigin;
		uint16_t width;
		uint16_t height;
		unsigned char bpp;
		unsigned char descriptor;
	};
	//vector<uint8_t> _data(width * height);
	TGAHEADER header;

	memset(&header, 0, sizeof(TGAHEADER));
	header.imageType =2;
	header.width =width;
	header.height =height;
	header.bpp =24;

	datei.open(filename.c_str(), std::fstream::binary | std::fstream::trunc | std::fstream::out);

	datei.write((char*)&header, sizeof(TGAHEADER));
	for(int i =0;i <width * height;i++) {
		if(bpp ==1) {
			datei.put(data[i + 0]);
			datei.put(data[i + 0]);
			datei.put(data[i + 0]);
		}
		else if(bpp ==3) {
			datei.put(data[(i * bpp) + 2]);
			datei.put(data[(i * bpp) + 1]);
			datei.put(data[(i * bpp) + 0]);
		}
	}

	datei.close();
}

}
