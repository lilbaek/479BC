#include <assert.h>
#include "gui_assets.h"
#include "graphics/renderer.h"
#include "core/dir.h"
#include "core/array.h"
#include "image.h"
#include "core/log.h"
#include "core/xml_parser.h"
#include "core/file.h"
#include "core/image_packer.h"
#include "core/png_read.h"
#include "platform/file_manager.h"
#include "assets.h"

#define GUI_ASSET_ARRAY_SIZE 500
#define XML_BUFFER_SIZE 1024
#define XML_TOTAL_ELEMENTS 3

static struct {
    char file_name[FILE_NAME_MAX];

    array(asset_image) asset_images;

    size_t file_name_position;
    asset_image *current_image;
    int atlas_width;
    int atlas_height;
} gui_data;

static int xml_start_assetlist_element(void);

static int xml_start_image_element(void);

static int xml_start_layer_element(void);

static void xml_end_assetlist_element(void);

static void xml_end_image_element(void);

static const xml_parser_element xml_elements[XML_TOTAL_ELEMENTS] = {
        {"assetlist", xml_start_assetlist_element, xml_end_assetlist_element},
        {"image",     xml_start_image_element,     xml_end_image_element, "assetlist"},
        {"layer",     xml_start_layer_element,     0,                     "image"},
};

static const char *INVERT_VALUES[3] = {"horizontal", "vertical", "both"};
static const char *ROTATE_VALUES[3] = {"90", "180", "270"};

asset_image *image_get_from_id(int image_id) {
    asset_image *last = array_last(gui_data.asset_images);
    if (image_id < 0 || !last || image_id > last->index) {
        return 0;
    }
    return array_item(gui_data.asset_images, image_id);
}


const image *gui_assets_get_image_id(const char *image_name) {
    if (!image_name || !*image_name) {
        assert("Gui Asset group not found");
        return 0;
    }
    const asset_image *image = image_get_from_id(0);
    while (image && image->index <= gui_data.asset_images.size) {
        if (image->id && strcmp(image->id, image_name) == 0) {
            return &image->img;
        }
        image = image_get_from_id(image->index + 1);
    }
    log_info("Asset image not found: ", image_name, 0);
    assert("Gui Asset group not found");
    return 0;
}

static void unload_image_layers(asset_image *img) {
    layer *l = img->last_layer;
    while (l) {
        layer *current = l;
        l = l->prev;
        layer_unload(current);
    };
    img->last_layer = &img->first_layer;
}

void gui_asset_image_unload(asset_image *img) {
    unload_image_layers(img);
    free((char *) img->id);
    free((color_t *) img->data); // Freeing a const pointer - ugly but necessary
    if (!img->is_reference) {
        free(img->img.top);
    }
    free(img->img.animation);
    img->id = 0;
    img->data = 0;
    img->active = 0;
    memset(&img->img, 0, sizeof(image));
}

static inline int layer_is_empty(const layer *l) {
#ifndef BUILDING_ASSET_PACKER
    return !l->width && !l->height;
#else
    return !l->original_image_group && !l->original_image_id && !l->asset_image_path;
#endif
}

static layer *create_layer_for_image(asset_image *img) {
    if (layer_is_empty(img->last_layer)) {
        return img->last_layer;
    }
    layer *l = malloc(sizeof(layer));
    if (!l) {
        log_error("Out of memory to create layer", 0, 0);
        return 0;
    }
    memset(l, 0, sizeof(layer));
    l->prev = img->last_layer;
    return l;
}

static void new_image(asset_image *img, int index) {
    img->index = index;
    img->active = 1;
}

static int is_image_active(const asset_image *img) {
    return img->active;
}

int gui_asset_image_init_array(void) {
    asset_image *image;
    array_foreach(gui_data.asset_images, image) {
        gui_asset_image_unload(image);
    }
    return array_init(gui_data.asset_images, GUI_ASSET_ARRAY_SIZE, new_image, is_image_active);
}

asset_image *gui_asset_image_create(void) {
    asset_image *result;
    array_new_item(gui_data.asset_images, 1, result);
    return result;
}

void gui_xml_init(void) {
    xml_parser_init(xml_elements, XML_TOTAL_ELEMENTS);
}

int gui_xml_process_assetlist_file(const char *xml_file_name) {
    log_info("Loading assetlist file", xml_file_name, 0);

    FILE *xml_file = file_open_asset(xml_file_name, "r");

    if (!xml_file) {
        log_error("Error opening assetlist file", xml_file_name, 0);
        return 0;
    }

    char buffer[XML_BUFFER_SIZE];
    int done = 0;
    int error = 0;

    xml_parser_reset();

    do {
        size_t bytes_read = fread(buffer, 1, XML_BUFFER_SIZE, xml_file);
        done = bytes_read < sizeof(buffer);
        if (!xml_parser_parse(buffer, (unsigned int) bytes_read, done)) {
            log_error("Error parsing file", xml_file_name, 0);
            error = 1;
            break;
        }
    } while (!done);
    file_close(xml_file);
    return !error;
}

void gui_xml_finish(void) {
    xml_parser_free();
}

static int load_image_layers(asset_image *img) {
    int has_alpha_mask = 0;
    for (layer *l = img->last_layer; l; l = l->prev) {
        if (l->mask == LAYER_MASK_ALPHA) {
            has_alpha_mask = 1;
        }
        layer_load_asset(l);
    }
    return has_alpha_mask;
}

static int load_image(asset_image *img) {
    img->img.original.width = img->img.width;
    img->img.original.height = img->img.height;

    int has_alpha_mask = load_image_layers(img);

    color_t *data = malloc(sizeof(color_t) * img->img.width * img->img.height);
    if (!data) {
        log_error("Error creating image - out of memory", 0, 0);
        unload_image_layers(img);
        return 0;
    }
    memset(data, 0, sizeof(color_t) * img->img.width * img->img.height);

    // Images with an alpha mask layer need to be loaded from first to last, which is slower
    const layer *l = has_alpha_mask ? &img->first_layer : img->last_layer;

    while (l) {
        int image_start_x, image_start_y, image_valid_width, image_valid_height, layer_step_x;

        if (l->rotate == ROTATE_NONE || l->rotate == ROTATE_180_DEGREES) {
            image_start_x = l->x_offset < 0 ? 0 : l->x_offset;
            image_start_y = l->y_offset < 0 ? 0 : l->y_offset;
            image_valid_width = image_start_x + l->width - (l->x_offset < 0 ? -l->x_offset : 0);
            image_valid_height = image_start_y + l->height - (l->y_offset < 0 ? -l->y_offset : 0);
            layer_step_x = 1;
        } else {
            image_start_x = l->x_offset < 0 ? 0 : l->x_offset;
            image_start_y = l->y_offset < 0 ? 0 : l->y_offset;
            image_valid_width = image_start_x + l->height - (l->x_offset < 0 ? -l->x_offset : 0);
            image_valid_height = image_start_y + l->width - (l->y_offset < 0 ? -l->y_offset : 0);
            layer_step_x = l->width;
        }
        if (image_valid_width > img->img.width) {
            image_valid_width = img->img.width;
        }
        if (image_valid_height > img->img.height) {
            image_valid_height = img->img.height;
        }

        // The above code is innacurate when a layer is rotated either by 90 or 270 degrees and inverted.
        // In those cases, we use layer_get_color_for_image_position for every pixel, which is much slower but accurate.
        int inverts_and_rotates = l->invert != INVERT_NONE &&
                                  (l->rotate == ROTATE_90_DEGREES || l->rotate == ROTATE_270_DEGREES);

        if (!inverts_and_rotates) {
            layer_invert_type invert = l->invert;
            if (l->rotate == ROTATE_90_DEGREES || l->rotate == ROTATE_180_DEGREES) {
                invert ^= INVERT_HORIZONTAL;
            }
            if (invert & INVERT_HORIZONTAL) {
                layer_step_x = -layer_step_x;
            }
        }

        for (int y = image_start_y; y < image_valid_height; y++) {
            color_t *pixel = &data[y * img->img.width + image_start_x];
            const color_t *layer_pixel = 0;
            if (!inverts_and_rotates) {
                layer_pixel = layer_get_color_for_image_position(l, image_start_x, y);
            }
            for (int x = image_start_x; x < image_valid_width; x++) {
                if (inverts_and_rotates) {
                    layer_pixel = layer_get_color_for_image_position(l, x, y);
                }
                color_t image_pixel_alpha = *pixel & COLOR_CHANNEL_ALPHA;
                if (l->mask == LAYER_MASK_ALPHA) {
                    // Since the alpha mask should be grayscale, we use the blue color as the mask value
                    color_t alpha_mask = (*layer_pixel & COLOR_CHANNEL_BLUE) << COLOR_BITSHIFT_ALPHA;
                    if (alpha_mask == ALPHA_TRANSPARENT || image_pixel_alpha == ALPHA_TRANSPARENT) {
                        *pixel = ALPHA_TRANSPARENT;
                    } else if (alpha_mask != ALPHA_OPAQUE) {
                        if (image_pixel_alpha != ALPHA_OPAQUE) {
                            color_t alpha_src = image_pixel_alpha >> COLOR_BITSHIFT_ALPHA;
                            color_t alpha_dst = alpha_mask >> COLOR_BITSHIFT_ALPHA;
                            alpha_mask = COLOR_MIX_ALPHA(alpha_src, alpha_dst) << COLOR_BITSHIFT_ALPHA;
                        }
                        color_t result = *pixel | ALPHA_OPAQUE;
                        result &= alpha_mask & COLOR_WHITE;
                        *pixel = result;
                    }
                    pixel++;
                    layer_pixel += layer_step_x;
                    continue;
                }
                color_t layer_pixel_alpha = *layer_pixel & COLOR_CHANNEL_ALPHA;
                if ((image_pixel_alpha == ALPHA_OPAQUE && !has_alpha_mask) || layer_pixel_alpha == ALPHA_TRANSPARENT) {
                    pixel++;
                    layer_pixel += layer_step_x;
                    continue;
                }
                if (image_pixel_alpha == ALPHA_TRANSPARENT || (layer_pixel_alpha == ALPHA_OPAQUE && has_alpha_mask)) {
                    *pixel = *layer_pixel;
                } else if (layer_pixel_alpha == ALPHA_OPAQUE) {
                    color_t alpha = image_pixel_alpha >> COLOR_BITSHIFT_ALPHA;
                    *pixel = COLOR_BLEND_ALPHA_TO_OPAQUE(*pixel, *layer_pixel, alpha);
                } else {
                    color_t alpha_src = image_pixel_alpha >> COLOR_BITSHIFT_ALPHA;
                    color_t alpha_dst = layer_pixel_alpha >> COLOR_BITSHIFT_ALPHA;
                    color_t alpha_mix = COLOR_MIX_ALPHA(alpha_src, alpha_dst);
                    *pixel = COLOR_BLEND_ALPHAS(*pixel, *layer_pixel, alpha_src, alpha_dst, alpha_mix);
                }
                pixel++;
                layer_pixel += layer_step_x;
            }
        }
        l = has_alpha_mask ? l->next : l->prev;
    }

    unload_image_layers(img);

    img->data = data;

    return 1;
}

int gui_asset_image_load_all() {
#ifndef BUILDING_ASSET_PACKER
    image_packer packer;
    int max_width, max_height;
    graphics_renderer()->get_max_image_size(&max_width, &max_height);
    int rectangles = gui_data.asset_images.size;
    if (image_packer_init(&packer, rectangles, max_width, max_height) != IMAGE_PACKER_OK) {
        log_error("Failed to init image packer", 0, 0);
        return 0;
    }
    packer.options.fail_policy = IMAGE_PACKER_NEW_IMAGE;
    packer.options.reduce_image_size = 1;
    packer.options.sort_by = IMAGE_PACKER_SORT_BY_AREA;

    asset_image *current_image;
    int rect = 0;
    array_foreach(gui_data.asset_images, current_image) {
        if (current_image->is_reference) {
            continue;
        }
        load_image(current_image);
        int top_height = current_image->img.top ? current_image->img.top->height : 0;

        if (graphics_renderer()->should_pack_image(current_image->img.width, current_image->img.height + top_height)) {
            image *img_to_crop = 0;
            if (current_image->img.is_isometric) {
                if (current_image->img.top) {
                    img_to_crop = current_image->img.top;
                }
            } else {
                img_to_crop = &current_image->img;
            }
            if (img_to_crop) {
                image_crop(img_to_crop, current_image->data);
            }
            current_image->img.atlas.id = ATLAS_GUI << IMAGE_ATLAS_BIT_OFFSET;
            packer.rects[rect].input.width = current_image->img.width;
            packer.rects[rect].input.height = current_image->img.height;

            if (current_image->img.is_isometric && img_to_crop) {
                rect++;
                img_to_crop->atlas.id = ATLAS_GUI << IMAGE_ATLAS_BIT_OFFSET;
                packer.rects[rect].input.width = img_to_crop->width;
                packer.rects[rect].input.height = img_to_crop->height;
            }

            // Uncrop image for now, crop it later again
            if (img_to_crop) {
                img_to_crop->x_offset = 0;
                img_to_crop->y_offset = 0;
                img_to_crop->width = img_to_crop->original.width;
                img_to_crop->height = img_to_crop->original.height;
            }
            rect++;
        } else {
            current_image->img.atlas.id = ATLAS_UNPACKED_EXTRA_ASSET << IMAGE_ATLAS_BIT_OFFSET;
            if (current_image->img.top) {
                current_image->img.top->atlas.id = ATLAS_UNPACKED_EXTRA_ASSET << IMAGE_ATLAS_BIT_OFFSET;
            }
        }
    }

    png_unload();
    image_packer_pack(&packer);

    const image_atlas_data *atlas_data = graphics_renderer()->prepare_image_atlas(ATLAS_GUI,
                                                                                  packer.result.images_needed,
                                                                                  packer.result.last_image_width,
                                                                                  packer.result.last_image_height);

    if (!atlas_data) {
        log_error("Failed to create packed images atlas - out of memory", 0, 0);
        return 0;
    }

    rect = 0;

    array_foreach(gui_data.asset_images, current_image) {
        int top_height = current_image->img.top ? current_image->img.top->height : 0;

        if (graphics_renderer()->should_pack_image(current_image->img.width, current_image->img.height + top_height)) {
            int original_width = current_image->img.width;
            int original_height = current_image->img.height;
            if (current_image->img.top) {
                original_height += current_image->img.top->original.height;
            }
            int original_y_offset = current_image->img.atlas.y_offset;
            if (!current_image->img.is_isometric) {
                image_crop(&current_image->img, current_image->data);
            }
            current_image->img.atlas.x_offset = packer.rects[rect].output.x;
            current_image->img.atlas.y_offset = packer.rects[rect].output.y;
            current_image->img.atlas.id += packer.rects[rect].output.image_index;
            int dst_side = atlas_data->image_widths[packer.rects[rect].output.image_index];
            image_copy_info copy = {
                    .src = {current_image->img.x_offset, current_image->img.y_offset + original_y_offset,
                            original_width, original_height, current_image->data},
                    .dst = {current_image->img.atlas.x_offset, current_image->img.atlas.y_offset,
                            dst_side, dst_side, atlas_data->buffers[packer.rects[rect].output.image_index]},
                    .rect = {0, 0, current_image->img.width, current_image->img.height}
            };
            image_copy(&copy);
            if (current_image->img.top) {
                rect++;
                image *top = current_image->img.top;
                int top_width = top->width;
                int top_height = top->original.height;
                top->atlas.x_offset = packer.rects[rect].output.x;
                top->atlas.y_offset = packer.rects[rect].output.y;
                top->atlas.id += packer.rects[rect].output.image_index;
                dst_side = atlas_data->image_widths[packer.rects[rect].output.image_index];
                image_crop(top, current_image->data);
                image_copy_info copy = {
                        .src = {top->x_offset, top->y_offset, top_width, top_height, current_image->data},
                        .dst = {top->atlas.x_offset, top->atlas.y_offset,
                                dst_side, dst_side, atlas_data->buffers[packer.rects[rect].output.image_index]},
                        .rect = {0, 0, top->width, top->height}
                };
                image_copy(&copy);
            }

            free((color_t *) current_image->data); // Freeing a const pointer - ugly but necessary

            current_image->data = 0;
            rect++;
        }
    }
    image_packer_free(&packer);
    gui_data.atlas_width = atlas_data->image_widths[0];
    gui_data.atlas_height = atlas_data->image_heights[0];
    graphics_renderer()->create_image_atlas(atlas_data, 1);
#endif
    return 1;
}

void gui_assets_init() {
    if (graphics_renderer()->has_image_atlas(ATLAS_GUI)) {
        return;
    }

    graphics_renderer()->free_image_atlas(ATLAS_GUI);

    const dir_listing *xml_files = dir_find_files_with_extension(ASSETS_DIRECTORY, "xml");

    if (!gui_asset_image_init_array()) {
        log_error("Not enough memory to initialize gui assets. The game will probably crash.", 0, 0);
    }
    gui_xml_init();

    for (int i = 0; i < xml_files->num_files; ++i) {
        if (strcmp(xml_files->files[i], "gui.xml") == 0) {
            gui_xml_process_assetlist_file(xml_files->files[i]);
        }
    }

    gui_xml_finish();

    gui_asset_image_load_all();
}

static void set_asset_image_base_path(const char *name) {
    size_t position = 0;
    char *dst = gui_data.file_name;
    memset(dst, 0, FILE_NAME_MAX);
    strncpy(dst, name, FILE_NAME_MAX - position - 1);
    position += strlen(name);
    dst[position++] = '/';
    gui_data.file_name_position = position;
}

static int xml_start_assetlist_element(void) {
    char *name = xml_parser_copy_attribute_string("name");
    set_asset_image_base_path(name);
    return 1;
}


static int xml_start_image_element(void) {
    if (xml_parser_get_total_attributes() > 12) {
        return 0;
    }
    asset_image *img = gui_asset_image_create();
    if (!img) {
        return 0;
    }
    gui_data.current_image = img;
    img->id = xml_parser_copy_attribute_string("id");
    img->last_layer = &img->first_layer;
    return 1;
}

int gui_layer_add_from_image_path(layer *l, int src_x, int src_y, int offset_x, int offset_y, int width, int height) {
    l->src_x = src_x;
    l->src_y = src_y;
    l->width = width;
    l->height = height;
    l->calculated_image_id = 0;
    l->asset_image_path = malloc(FILE_NAME_MAX * sizeof(char));
    snprintf(l->asset_image_path, FILE_NAME_MAX, "%s.png", "gui");
#ifndef BUILDING_ASSET_PACKER
    if (!l->width || !l->height) {
        if (!png_get_image_size(l->asset_image_path, &width, &height)) {
            log_info("Unable to load image", l->asset_image_path, 0);
            layer_unload(l);
            return 0;
        }
        if (!l->width) {
            l->width = width;
        }
        if (!l->height) {
            l->height = height;
        }
    }
#endif
    l->x_offset = offset_x;
    l->y_offset = offset_y;
    return 1;
}

int gui_asset_image_add_layer(asset_image *img,
                              const char *path, const char *group_id, const char *image_id,
                              int src_x, int src_y, int offset_x, int offset_y, int width, int height,
                              layer_invert_type invert, layer_rotate_type rotate, layer_isometric_part part,
                              layer_mask mask) {
    layer *current_layer = create_layer_for_image(img);

    if (!current_layer) {
        return 0;
    }
    current_layer->invert = invert;
    current_layer->rotate = rotate;
    current_layer->part = part;
    current_layer->mask = mask;

    int result = gui_layer_add_from_image_path(current_layer, src_x, src_y, offset_x, offset_y, width, height);
    if (!result) {
        return 0;
    }
    if (rotate == ROTATE_NONE || rotate == ROTATE_180_DEGREES) {
        if (!img->img.width) {
            img->img.width = current_layer->width;
        }
        if (!img->img.height) {
            img->img.height = current_layer->height;
        }
    } else {
        if (!img->img.width) {
            img->img.width = current_layer->height;
        }
        if (!img->img.height) {
            img->img.height = current_layer->width;
        }
    }
    if (img->last_layer != current_layer) {
        img->last_layer->next = current_layer;
    }
    img->last_layer = current_layer;
    return 1;
}

static int xml_start_layer_element(void) {
    int total_attributes = xml_parser_get_total_attributes();
    if (total_attributes < 2 || total_attributes > 24) {
        return 0;
    }
    asset_image *img = gui_data.current_image;
    static const char *part_values[2] = {"footprint", "top"};
    static const char *mask_values[2] = {"grayscale", "alpha"};

    const char *path = xml_parser_get_attribute_string("src");
    const char *group = xml_parser_get_attribute_string("group");
    const char *image = xml_parser_get_attribute_string("image");
    int src_x = xml_parser_get_attribute_int("src_x");
    int src_y = xml_parser_get_attribute_int("src_y");
    int offset_x = xml_parser_get_attribute_int("x");
    int offset_y = xml_parser_get_attribute_int("y");
    int width = xml_parser_get_attribute_int("width");
    int height = xml_parser_get_attribute_int("height");
    layer_invert_type invert = xml_parser_get_attribute_enum("invert", INVERT_VALUES, 3, INVERT_HORIZONTAL);
    layer_rotate_type rotate = xml_parser_get_attribute_enum("rotate", ROTATE_VALUES, 3, ROTATE_90_DEGREES);
    layer_isometric_part part = xml_parser_get_attribute_enum("part", part_values, 2, PART_FOOTPRINT);
    layer_mask mask = xml_parser_get_attribute_enum("mask", mask_values, 2, LAYER_MASK_GRAYSCALE);

    if (!gui_asset_image_add_layer(img, path, group, image, src_x, src_y,
                                   offset_x, offset_y, width, height, invert, rotate,
                                   part == PART_NONE ? PART_BOTH : part, mask)) {
        log_info("Invalid layer for image", img->id, 0);
    }
    return 1;
}

static void xml_end_image_element(void) {
    image *img = &gui_data.current_image->img;
    if (!img->width || !img->height) {
        gui_asset_image_unload(gui_data.current_image);
        return;
    }
}

static void xml_end_assetlist_element(void) {

}

int gui_assets_get_atlas_width() {
    return gui_data.atlas_width;
}

int gui_assets_get_atlas_height() {
    return gui_data.atlas_height;
}
