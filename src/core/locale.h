#ifndef CORE_LOCALE_H
#define CORE_LOCALE_H

#include <stdint.h>

/**
 * Language type
 */
typedef enum {
    LANGUAGE_UNKNOWN,
    LANGUAGE_ENGLISH,
    LANGUAGE_MAX_ITEMS
} language_type;

/**
 * Determines language based on the language file
 * @return Guessed language
 */
language_type locale_determine_language(void);

/**
 * Check whether to write the year before the 'AD' part or the other way around
 * @return Boolean true if years should be written as "200 AD", false for "AD 200"
 */
int locale_year_before_ad(void);

/**
 * Check whether to use the string 'Dn' from the 'funds' top menu when used for money amounts
 * @return Boolean true for the translated version, false to use fixed string 'Dn'
 */
int locale_translate_money_dn(void);

/**
 * Returns the indent size to use for paragraphs in rich text
 * @return Indent in pixels
 */
int locale_paragraph_indent(void);


#endif // CORE_LOCALE_H
