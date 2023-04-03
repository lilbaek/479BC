#include "locale.h"

#include "core/lang.h"
#include "core/log.h"
#include "core/string.h"

#include <stdint.h>

static struct {
    language_type last_determined_language;
} data;

static language_type determine_language(void)
{
    return LANGUAGE_ENGLISH;
}

static void log_language(void)
{
    const char *desc;
    switch (data.last_determined_language) {
        case LANGUAGE_ENGLISH: desc = "English"; break;
        default: desc = "Unknown"; break;
    }
    log_info("Detected language:", desc, 0);
}

language_type locale_determine_language(void)
{
    data.last_determined_language = determine_language();
    log_language();
    return data.last_determined_language;
}

int locale_year_before_ad(void)
{
    // In all languages it's "200 AD" except for English
    return data.last_determined_language != LANGUAGE_ENGLISH;
}

int locale_translate_money_dn(void)
{
    return 1;
}

int locale_paragraph_indent(void)
{
    return 50;
}

