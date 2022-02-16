#include "layer.h"

#include "assets/group.h"
#include "assets/image.h"
#include "assets/xml.h"
#include "core/file.h"
#include "core/log.h"
#include "core/png_read.h"
#include "core/string.h"

#include <stdlib.h>
#include <string.h>

static color_t DUMMY_LAYER_DATA = COLOR_BLACK;

static void load_dummy_layer(layer *l)
{
    l->data = &DUMMY_LAYER_DATA;
    l->width = 1;
    l->height = 1;
    l->calculated_image_id = 0;
}

#ifndef BUILDING_ASSET_PACKER
static void copy_regular_image(layer *l, color_t *dst, const image *img, const color_t *atlas_pixels, int atlas_width)
{
    for (int y = 0; y < l->height; y++) {
        memcpy(&dst[y * l->width], &atlas_pixels[(y + img->atlas.y_offset) * atlas_width + img->atlas.x_offset],
            l->width * sizeof(color_t));
    }
}

static void copy_isometric_image(layer *l, color_t *dst, const image *img, const color_t *atlas_pixels, int atlas_width)
{
    // No difference in image data - keep using the original image
    if ((l->part == PART_BOTH && graphics_renderer()->isometric_images_are_joined()) ||
        ((l->part & PART_FOOTPRINT) && !img->top_height)) {
        copy_regular_image(l, dst, img, atlas_pixels, atlas_width);
    }
    int tiles = (img->width + 2) / (FOOTPRINT_WIDTH + 2);
    if ((l->part & PART_TOP) && img->top_height) {
        int height = l->part & PART_FOOTPRINT ? l->height - tiles * FOOTPRINT_HALF_HEIGHT : l->height;
        asset_image_copy_isometric_top(dst, atlas_pixels, l->width, height,
            0, 0, l->width, img->atlas.x_offset, img->atlas.y_offset, atlas_width);
    }
    if (l->part & PART_FOOTPRINT) {
        int y_offset;
        int footprint_height = tiles * FOOTPRINT_HEIGHT;
        if (graphics_renderer()->isometric_images_are_joined() && img->top_height) {
            y_offset = l->height - footprint_height;
        } else {
            y_offset = img->top_height;
        }
        asset_image_copy_isometric_footprint(dst, atlas_pixels, l->width, footprint_height,
            0, l->height - footprint_height, l->width,
            img->atlas.x_offset, img->atlas.y_offset + y_offset, atlas_width);
    }
}

static void convert_layer_to_grayscale(color_t *pixels, int width, int height)
{
    for (int y = 0; y < height; y++) {
        color_t *color = &pixels[y * width];
        for (int x = 0; x < width; x++) {
            color_t r = (*color & COLOR_CHANNEL_RED) >> COLOR_BITSHIFT_RED;
            color_t g = (*color & COLOR_CHANNEL_GREEN) >> COLOR_BITSHIFT_GREEN;
            color_t b = (*color & COLOR_CHANNEL_BLUE) >> COLOR_BITSHIFT_BLUE;
            color_t gray = (r + g + b) / 3;
            *color = (*color & COLOR_CHANNEL_ALPHA) | (gray << COLOR_BITSHIFT_RED) |
                (gray << COLOR_BITSHIFT_GREEN) | (gray << COLOR_BITSHIFT_BLUE);
            color++;
        }
    }
}

static void load_layer_from_another_image(layer *l, color_t **main_data, int *main_image_widths)
{
    const image *img = image_get(l->calculated_image_id);
    if (!img) {
        log_error("Problem loading layer from image id", 0, l->calculated_image_id);
        load_dummy_layer(l);
        return;
    }
    
    const asset_image *asset_img = 0;

    atlas_type type = img->atlas.id >> IMAGE_ATLAS_BIT_OFFSET;
    if (type == ATLAS_EXTRA_ASSET || type == ATLAS_UNPACKED_EXTRA_ASSET) {
        asset_img = asset_image_get_from_id(l->calculated_image_id - IMAGE_MAIN_ENTRIES);
        if (!asset_img) {
            log_error("Problem loading layer from image id", 0, l->calculated_image_id);
            load_dummy_layer(l);
            return;
        }
        if (!l->grayscale) {
            l->data = asset_img->data;
            return;
        }
    }

    int size = l->width * l->height * sizeof(color_t);
    color_t *data = malloc(size);
    if (!data) {
        log_error("Problem loading layer from image id - out of memory", 0, l->calculated_image_id);
        load_dummy_layer(l);
        return;
    }
    memset(data, 0, l->width * l->height * sizeof(color_t));

    if (asset_img) {
        int width = l->width < asset_img->img.width ? l->width : asset_img->img.width;
        int height = l->height < asset_img->img.height ? l->height : asset_img->img.height;
        for (int y = 0; y < height; y++) {
            memcpy(&data[y * l->width], &asset_img->data[y * asset_img->img.width], width * sizeof(color_t));
        }
    } else if (type == ATLAS_EXTERNAL) {
        if (!image_load_external_pixels(data, l->calculated_image_id, l->width)) {
            free(data);
            log_error("Problem loading layer from image id", 0, l->calculated_image_id);
            load_dummy_layer(l);
            return;
        }
    } else if (type == ATLAS_MAIN) {
        int atlas_width = main_image_widths[img->atlas.id & IMAGE_ATLAS_BIT_MASK];
        const color_t *atlas_pixels = main_data[img->atlas.id & IMAGE_ATLAS_BIT_MASK];
        if (!atlas_width || !atlas_pixels) {
            free(data);
            log_error("Problem loading layer from image id", 0, l->calculated_image_id);
            load_dummy_layer(l);
            return;
        }
        if (img->is_isometric) {
            copy_isometric_image(l, data, img, atlas_pixels, atlas_width);
        } else {
            copy_regular_image(l, data, img, atlas_pixels, atlas_width);
        }
    }
    l->calculated_image_id = 0;

    if (l->grayscale) {
        convert_layer_to_grayscale(data, l->width, l->height);
    }

    l->data = data;
}
#endif

void layer_load(layer *l, color_t **main_data, int *main_image_widths)
{
#ifndef BUILDING_ASSET_PACKER
    if (l->calculated_image_id) {
        load_layer_from_another_image(l, main_data, main_image_widths);
        return;
    }
#endif
    if (!l->asset_image_path) {
        log_error("No layer source", l->asset_image_path, 0);
        load_dummy_layer(l);
        return;
    }

    int size = l->width * l->height * sizeof(color_t);
    color_t *data = malloc(size);
    if (!data) {
        log_error("Problem loading layer - out of memory", l->asset_image_path, 0);
        load_dummy_layer(l);
        return;
    }
    memset(data, 0, size);
    if (!png_read(l->asset_image_path, data, l->src_x, l->src_y, l->width, l->height, 0, 0, l->width, 0)) {
        free(data);
        log_error("Problem loading layer from file", l->asset_image_path, 0);
        load_dummy_layer(l);
        return;
    }
#ifndef BUILDING_ASSET_PACKER
    if (l->grayscale) {
        convert_layer_to_grayscale(data, l->width, l->height);
    }
#endif

    l->data = data;
}

void layer_unload(layer *l)
{
    free(l->asset_image_path);
#ifdef BUILDING_ASSET_PACKER
    free(l->original_image_group);
    free(l->original_image_id);
#endif
    if (!l->calculated_image_id && l->data != &DUMMY_LAYER_DATA) {
        free((color_t *)l->data); // Freeing a const pointer. Ugly but necessary
    }
    if (l->prev) {
        free(l);
    } else {
        memset(l, 0, sizeof(layer));
    }
}

const color_t *layer_get_color_for_image_position(const layer *l, int x, int y)
{
    x -= l->x_offset;
    y -= l->y_offset;

    if (l->rotate == ROTATE_90_DEGREES || l->rotate == ROTATE_270_DEGREES) {
        int temp = x;
        x = y;
        y = temp;
    }
    layer_invert_type invert = l->invert;
    if (l->rotate == ROTATE_90_DEGREES) {
        invert ^= INVERT_VERTICAL;
    } else if (l->rotate == ROTATE_180_DEGREES) {
        invert ^= INVERT_BOTH;
    } else if (l->rotate == ROTATE_270_DEGREES) {
        invert ^= INVERT_HORIZONTAL;
    }
    if (invert & INVERT_HORIZONTAL) {
        x = l->width - x - 1;
    }
    if (invert & INVERT_VERTICAL) {
        y = l->height - y - 1;
    }
    if (x < 0 || x >= l->width || y < 0 || y >= l->height) {
        return ALPHA_TRANSPARENT;
    }
    return &l->data[y * l->width + x];
}

int layer_add_from_image_path(layer *l, const char *path,
    int src_x, int src_y, int offset_x, int offset_y, int width, int height)
{
    l->src_x = src_x;
    l->src_y = src_y;
    l->width = width;
    l->height = height;
    l->calculated_image_id = 0;
    l->asset_image_path = malloc(FILE_NAME_MAX * sizeof(char));
    if (path) {
        xml_get_full_image_path(l->asset_image_path, path);
    } else {
        snprintf(l->asset_image_path, FILE_NAME_MAX, "%s.png", group_get_current()->name);
    }
#ifndef BUILDING_ASSET_PACKER
    if ((!l->width || !l->height) && !png_get_image_size(l->asset_image_path, &l->width, &l->height)) {
        log_info("Unable to load image", path, 0);
        layer_unload(l);
        return 0;
    }
#endif
    l->x_offset = offset_x;
    l->y_offset = offset_y;
    return 1;
}

#ifdef BUILDING_ASSET_PACKER
static char *copy_attribute(const char *attribute)
{
    if (!attribute) {
        return 0;
    }
    char *dest = malloc((strlen(attribute) + 1) * sizeof(char));
    if (!dest) {
        log_error("There was no memory to copy the attribute", attribute, 0);
        return 0;
    }
    strcpy(dest, attribute);
    return dest;
}
#endif

#ifndef BUILDING_ASSET_PACKER
static int determine_layer_height(const image *img, layer_isometric_part part)
{
    if (!img->is_isometric || graphics_renderer()->isometric_images_are_joined()) {
        return img->height;
    }
    if (part == PART_BOTH || part == PART_FOOTPRINT) {
        if (img->top_height) {
            int tiles = (img->width + 2) / (FOOTPRINT_WIDTH + 2);
            return img->height - tiles * FOOTPRINT_HALF_HEIGHT;
        } else {
            return img->height;
        }
    } else if (part == PART_TOP) {
        return img->top_height;
    } else {
        return 0;
    }
}
#endif

int layer_add_from_image_id(layer *l, const char *group_id, const char *image_id, int offset_x, int offset_y)
{
    l->src_x = 0;
    l->src_y = 0;
    l->width = 0;
    l->height = 0;
    l->x_offset = offset_x;
    l->y_offset = offset_y;
#ifdef BUILDING_ASSET_PACKER
    l->original_image_group = copy_attribute(group_id);
    l->original_image_id = copy_attribute(image_id);
#else
    const image *original_image = 0;
    if (strcmp(group_id, "this") == 0) {
        const image_groups *group = group_get_current();
        const asset_image *image = asset_image_get_from_id(group->first_image_index);
        while (image && image->index <= group->last_image_index) {
            if (strcmp(image->id, image_id) == 0) {
                l->calculated_image_id = image->index + IMAGE_MAIN_ENTRIES;
                original_image = &image->img;
                break;
            }
            image = asset_image_get_from_id(image->index + 1);
        }
        if (!l->calculated_image_id) {
            log_error("Unable to find image on current group with id", image_id, 0);
            layer_unload(l);
            return 0;
        }
    } else {
        int group = string_to_int(string_from_ascii(group_id));
        int id = image_id ? string_to_int(string_from_ascii(image_id)) : 0;
        l->calculated_image_id = image_group(group) + id;
        original_image = image_get(l->calculated_image_id);
    }
    if (!original_image) {
        log_error("Unable to find image for group id", group_id, 0);
        layer_unload(l);
        return 0;
    }
    l->width = original_image->width;
    l->height = determine_layer_height(original_image, l->part);
#endif
    return 1;
}
