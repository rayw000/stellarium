#ifndef __CHINESE_LUNISOLAR__
#define __CHINESE_LUNISOLAR__

typedef struct Lunisolar
{
  bool isleap;
  int day;
  int month;
  int year;
} Lunisolar;

typedef struct Gregorian
{
  int day;
  int month;
  int year;
} Gregorian;

Lunisolar getLunisolarFromGregorian(Gregorian g);


#endif /* defined(__CHINESE_LUNISOLAR__) */
