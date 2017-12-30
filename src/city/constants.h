#ifndef CITY_CONSTANTS_H
#define CITY_CONSTANTS_H


typedef enum {
    ADVISOR_NONE = 0,
    ADVISOR_LABOR = 1,
    ADVISOR_MILITARY = 2,
    ADVISOR_IMPERIAL = 3,
    ADVISOR_RATINGS = 4,
    ADVISOR_TRADE = 5,
    ADVISOR_POPULATION = 6,
    ADVISOR_HEALTH = 7,
    ADVISOR_EDUCATION = 8,
    ADVISOR_ENTERTAINMENT = 9,
    ADVISOR_RELIGION = 10,
    ADVISOR_FINANCIAL = 11,
    ADVISOR_CHIEF = 12
} advisor_type;

typedef enum {
    EMIGRATION_CAUSE_NONE = 0,
    EMIGRATION_CAUSE_NO_FOOD = 1,
    EMIGRATION_CAUSE_NO_JOBS = 2,
    EMIGRATION_CAUSE_HIGH_TAXES = 3,
    EMIGRATION_CAUSE_LOW_WAGES = 4,
    EMIGRATION_CAUSE_MANY_TENTS = 5,
} emigration_cause;

typedef enum {
    FESTIVAL_NONE = 0,
    FESTIVAL_SMALL = 1,
    FESTIVAL_LARGE = 2,
    FESTIVAL_GRAND = 3
} festival_size;

typedef enum {
    TRADE_STATUS_NONE = 0,
    TRADE_STATUS_IMPORT = 1,
    TRADE_STATUS_EXPORT = 2
} resource_trade_status;

#endif // CITY_CONSTANTS_H
