#ifndef _FUMASU_TGA_H_
#define _FUMASU_TGA_H_

#include <string>
#include <fstream>
#include <cstring>

using namespace std;

namespace Fumasu {

void makeScreenshot(string filename, uint16_t width, uint16_t height, uint8_t bpp, uint8_t *data);

}

#endif
