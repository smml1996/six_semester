#include <iostream>

#include <string>


namespace Util{

  std::string normalizeNumberString(const std::string &number){

    if(number.size() == 3)
      return number;

    if(number.size() > 3)
      return "-1";

    std::string answer = "";

    while(answer.size() + number.size() < 3){
      answer+="0";
    }

    answer+= number;

    return number;
  }

  std::string normalizeNumber(const int &number){
      return normalizeNumberString(std::to_string(number));
  }

  void cleanBuffer(char buffer[]){
    bzero(buffer,10000);
  }

}
