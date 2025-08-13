#include <opencv2/opencv.hpp>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>

struct FB {
    int fd{-1};
    fb_fix_screeninfo finfo{};
    fb_var_screeninfo vinfo{};
    size_t screensize{0};
    uint8_t* fbptr{nullptr};
    bool open(const char* dev="/dev/fb0") {
        fd = ::open(dev, O_RDWR);
        if (fd < 0) { perror("open /dev/fb0"); return false; }
        if (ioctl(fd, FBIOGET_FSCREENINFO, &finfo) == -1) { perror("FBIOGET_FSCREENINFO"); return false; }
        if (ioctl(fd, FBIOGET_VSCREENINFO, &vinfo) == -1) { perror("FBIOGET_VSCREENINFO"); return false; }
        screensize = vinfo.yres_virtual * finfo.line_length;
        fbptr = (uint8_t*)mmap(nullptr, screensize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
        if (fbptr == MAP_FAILED) { perror("mmap"); fbptr=nullptr; return false; }
        return true;
    }
    ~FB() {
        if (fbptr && fbptr!=MAP_FAILED) munmap(fbptr, screensize);
        if (fd>=0) close(fd);
    }
};

// Pack BGR888 pixel into framebuffer format (RGB565 or 32-bit XRGB)
static inline void put_pixel(uint8_t* dst, const fb_var_screeninfo& vinfo, uint8_t b, uint8_t g, uint8_t r) {
    if (vinfo.bits_per_pixel == 16) {
        // RGB565 default
        uint16_t R = (r >> 3);
        uint16_t G = (g >> 2);
        uint16_t B = (b >> 3);
        uint16_t pix = (R << 11) | (G << 5) | (B);
        *(uint16_t*)dst = pix;
    } else { // assume 32bpp XRGB
        // Use offsets from vinfo to be robust
        uint32_t pix = 0;
        pix |= ((uint32_t)r & ((1u<<vinfo.red.length)-1))   << vinfo.red.offset;
        pix |= ((uint32_t)g & ((1u<<vinfo.green.length)-1)) << vinfo.green.offset;
        pix |= ((uint32_t)b & ((1u<<vinfo.blue.length)-1))  << vinfo.blue.offset;
        *(uint32_t*)dst = pix;
    }
}

void blitMatToFB(const cv::Mat& bgr, FB& fb) {
    const int bpp = fb.vinfo.bits_per_pixel / 8;
    const int width  = std::min<int>(bgr.cols, fb.vinfo.xres);
    const int height = std::min<int>(bgr.rows, fb.vinfo.yres);
    for (int y = 0; y < height; ++y) {
        const uint8_t* src = bgr.ptr<uint8_t>(y);
        uint8_t* dst = fb.fbptr + (y + fb.vinfo.yoffset) * fb.finfo.line_length + fb.vinfo.xoffset * bpp;
        for (int x = 0; x < width; ++x) {
            const uint8_t b = src[3*x + 0];
            const uint8_t g = src[3*x + 1];
            const uint8_t r = src[3*x + 2];
            put_pixel(dst + x*bpp, fb.vinfo, b, g, r);
        }
    }
}

struct Vec3 { float x,y,z; };
static inline Vec3 rotateY(const Vec3& v, float a){ float c=cosf(a), s=sinf(a); return { c*v.x + s*v.z, v.y, -s*v.x + c*v.z }; }
static inline Vec3 rotateX(const Vec3& v, float a){ float c=cosf(a), s=sinf(a); return { v.x, c*v.y - s*v.z, s*v.y + c*v.z }; }

int main() {
    FB fb;
    if (!fb.open("/dev/fb0")) return 1;

    const int W = fb.vinfo.xres;
    const int H = fb.vinfo.yres;
    cv::Mat frame(H, W, CV_8UC3);

    // Cube vertices (unit cube centered at origin)
    std::vector<Vec3> verts = {
        {-1,-1,-1},{ 1,-1,-1},{ 1, 1,-1},{-1, 1,-1},
        {-1,-1, 1},{ 1,-1, 1},{ 1, 1, 1},{-1, 1, 1}
    };
    // Edges (indices into verts)
    int edges[][2] = {
        {0,1},{1,2},{2,3},{3,0},
        {4,5},{5,6},{6,7},{7,4},
        {0,4},{1,5},{2,6},{3,7}
    };

    float angle = 0.f;
    const float fov = 400.0f;   // projection scale
    const float zcam = 4.0f;    // camera distance

    while (true) {
        frame.setTo(cv::Scalar(0,0,0));

        // Rotate
        std::vector<cv::Point> pts;
        pts.reserve(verts.size());
        for (const auto& v : verts) {
            Vec3 r = rotateX(rotateY(v, angle*0.9f), angle*0.7f);
            float z = r.z + zcam;
            float px = (r.x * fov) / z + W*0.5f;
            float py = (r.y * fov) / z + H*0.5f;
            pts.emplace_back((int)std::lround(px), (int)std::lround(py));
        }

        // Draw edges
        for (auto& e : edges) {
            cv::line(frame, pts[e[0]], pts[e[1]], cv::Scalar(200, 255, 255), 2, cv::LINE_AA);
        }

        // Optional: small FPS/format overlay
        char info[128];
        std::snprintf(info, sizeof(info), "%dx%d %dbpp", W, H, fb.vinfo.bits_per_pixel);
        cv::putText(frame, info, {10, 30}, cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255,255,255), 2, cv::LINE_AA);

        // Blit to framebuffer
        blitMatToFB(frame, fb);

        angle += 0.03f;
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS cap
    }
    return 0;
}

