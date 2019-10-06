#include "cpp_date.hpp"
#include <iostream>

int
main()
{
  std::cout << cpp_date_ymd( 2009, 7 , 14 ) << std::endl;
  std::cout << cpp_date_ym_weekday_nth( 2019, 7 , 1 , 3 ) << std::endl;
  int marine_day = cpp_date_ym_weekday_nth( 2019, 7 , 1 , 3 );
  std::cout << cpp_date_format( marine_day, "%Y-%m-%d" ) << std::endl;
  std::cout << "1 months from marine day" << cpp_date_format( cpp_date_add_n_months( marine_day, 1) ,  "%Y-%m-%d" ) << std::endl;
  std::cout << "2 years from marine day" << cpp_date_format( cpp_date_add_n_years( marine_day, 2) , "%Y-%m-%d" ) << std::endl;
}
