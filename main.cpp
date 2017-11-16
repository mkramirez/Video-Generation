#include "Rectangle.h"

const double frames_per_second = 30;
const int duration_in_seconds = 3;

Rectangle drawRect;

int main(int argc, char * argv[]) {
    int num_frames = duration_in_seconds * frames_per_second;
    for (int i = 0; i < num_frames; ++i) {
        double time_in_seconds = i / frames_per_second;
        drawRect.draw_frame(time_in_seconds);
    }

    std::cout << "num_frames: " << num_frames << std::endl;
    std::cout << "Done." << std::endl;

    return 0;
}