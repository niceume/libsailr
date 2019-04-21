#include "simple_date.h"
#include <stdio.h>

int
main(int argc, char** argv)
{
  printf("%d\n", simple_date_ymd(2019,4,7));
  printf("%d\n", simple_date_ym_weekday_nth(2019,7,"Mon",3));
  printf("%s\n", simple_date_format( simple_date_add_n_years( simple_date_ymd(2019,7,15),2), "%Y-%m-%d"));
  printf("%s\n", simple_date_format( simple_date_add_n_months( simple_date_ymd(2019,7,15),3), "%Y-%m-%d"));
  printf("%s\n", simple_date_format( simple_date_add_n_days( simple_date_ymd(2019,7,15), 21), "%Y-%m-%d"));
}
