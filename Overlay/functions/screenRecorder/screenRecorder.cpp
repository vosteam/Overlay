#include "screenRecorder.h"

void record::screenRecorder(std::string ffilename)
{
    std::string filename = "video.avi"; //temporary fix

    int screen_width = GetSystemMetrics(SM_CXSCREEN);
    int screen_height = GetSystemMetrics(SM_CYSCREEN);
    cv::Size screen_size(screen_width, screen_height);

    int fourcc = cv::VideoWriter::fourcc('V', 'P', '9', '0');

    // Create video writer object
    cv::VideoWriter writer(filename, fourcc, 30, screen_size, true);

    // Check if writer is ready
    if (!writer.isOpened())
    {
        std::cout << "Error opening video file" << std::endl;
        return;
    }

    // Capture screen and write frames to video file
    while (true)
    {
        // Capture screen image
        HDC hScreen = GetDC(NULL);
        HDC hDC = CreateCompatibleDC(hScreen);
        HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screen_width, screen_height);
        HGDIOBJ hOld = SelectObject(hDC, hBitmap);
        SetStretchBltMode(hDC, HALFTONE);// Установка параметра HALFTONE
        BitBlt(hDC, 0, 0, screen_width, screen_height, hScreen, 0, 0, SRCCOPY);
        cv::Mat img = cv::Mat(screen_height, screen_width, CV_8UC4);
        GetBitmapBits(hBitmap, screen_width * screen_height * 4, img.data);
        cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);

        // Write frame to video file
        writer.write(img);

        // Release resources
        SelectObject(hDC, hOld);
        DeleteDC(hDC);
        DeleteObject(hBitmap);
        ReleaseDC(NULL, hScreen);

        //  Sleep(1000);

          // Check for escape key to exit loop (doesn't work)
        if (GetAsyncKeyState(VK_ESCAPE))
            break;
    }

    // Release video writer and close windows
    writer.release();
    cv::destroyAllWindows();
}