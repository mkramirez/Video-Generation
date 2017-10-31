#include <iostream>
#include <sstream>

typedef unsigned char byte;
#define W 720
#define H 480

const double frames_per_second = 30;
const int duration_in_seconds = 3;

unsigned char frame[H][W][3];

void clear_frame() {
    memset(frame, 0, sizeof(frame));
}

void clamp(int * x, int * y) {
    if(*x < 0) *x = 0;
    else if(*x >= W) *x = W - 1;

    if(*y < 0) *y = 0;
    else if(*y >= H) *y = H - 1;
}

bool outside_frame(int * x, int * y) {
    return *x < 0 or *x >= W or *y < 0 or *y >= H;
}

void draw_rect(int x, int y, int w, int h, byte r, byte g, byte b) {
    if (outside_frame(&x, &y)) return;
    clamp(&x, &y);
    int x0 = x;
    int x1 = x + w;
    int y0 = y;
    int y1 = y + h;
    clamp(&x0, &y0);
    clamp(&x1, &y1);
    for (int y = y0; y < y1; ++y) {
        for (int x = x0; x < x1; ++x) {
            frame[y][x][0] = r;
            frame[y][x][1] = g;
            frame[y][x][2] = b;
        }
    }
}

//Main Drawing Code.
//Expand this function to add content to the video.

void draw_frame(double t) {
    clear_frame();
    const double pps = 80;
    draw_rect(0 + t * pps, 0 + t * pps, 20, 10, 0xff, 0xff, 0xff);//origin to south east
    draw_rect(400 , 200 - t * pps, 20, 10, 0xff, 0x00, 0xff); //origin to north
    draw_rect(500 , 200 + t * pps, 20, 10, 0xff, 0xff, 0x00); //origin to south
    draw_rect(250 + t * pps, 150, 20, 10, 0x00, 0xff, 0xff); //origin to east
    draw_rect(300 - t * pps, 150, 20, 10, 0x00, 0x00, 0xff); //origin to west
}

int main(int argc, char * argv[]) {
    const char * cmd =
            "ffmpeg              "
                    "-y                  "
                    "-hide_banner        "
                    "-f rawvideo         " // input to be raw video data
                    "-pixel_format rgb24 "
                    "-video_size 720x480 "
                    "-r 60               " // frames per second
                    "-i -                " // read data from the standard input stream
                    "-pix_fmt yuv420p    " // to render with Quicktime
                    "-vcodec mpeg4       "
                    "-an                 " // no audio
                    "-q:v 5              " // quality level; 1 <= q <= 32
                    "output.mp4          ";

    FILE * pipe = popen(cmd, "w");
    if (pipe == 0) {
        std::cout << "error: " << strerror(errno) << std::endl;
        return 1;
    }

    int num_frames = duration_in_seconds * frames_per_second;
    for (int i = 0; i < num_frames; ++i) {
        double time_in_seconds = i / frames_per_second;
        draw_frame(time_in_seconds);
        fwrite(frame, 3, W * H, pipe);
    }

    fflush(pipe);
    pclose(pipe);

    std::cout << "num_frames: " << num_frames << std::endl;
    std::cout << "Done." << std::endl;

    return 0;
}