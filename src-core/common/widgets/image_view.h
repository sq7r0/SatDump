#pragma once

#include "imgui/imgui.h"
#include "common/image/image.h"
#include <mutex>
#include <vector>

class ImageViewWidget
{
private:
    unsigned int texture_id = 0;
    std::vector<uint32_t> texture_buffer;

    int img_width;
    int img_height;

    bool has_to_update = false;
    std::mutex image_mtx;

    float img_scale = 1;
    bool first_run = true;

    void handleMouseDrag();

    std::string id_str;

public:
    ImageViewWidget();
    ~ImageViewWidget();

    std::function<void(int x, int y)> tooltipCallback = [](int x, int y) {}; // Function that can be used to show a tooltip when hovering the image

    void update(image::Image<uint16_t> image);
    void draw(ImVec2 win_size);
};