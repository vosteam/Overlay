#pragma once
#include "addWatermark.h"

void screenRecoder(std::string file_video)
{
	std::string command = "ffmpeg -f gdigrab -framerate 30 -i desktop -c:v libx264 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p  " + ("C:\\Users\\" + (std::string)std::getenv("USERNAME") + "\\Documents\\vos.team\\" + file_video);
	system(command.c_str());
}