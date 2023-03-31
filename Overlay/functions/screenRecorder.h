#pragma once
#include "addWatermark.h"

void screenRecorder(const std::string& file_video) {
	const std::string command = "ffmpeg -f gdigrab -framerate 60 -i desktop -c:v libx264 -preset ultrafast -tune zerolatency -crf 25 -pix_fmt yuv420p C:\\Users\\%USERNAME%\\Documents\\vos.team\\" + file_video;
	std::system(command.c_str());
}