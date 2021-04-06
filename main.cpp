#include <iostream>
#include <sstream>
#include <algorithm>

#include "XKBTools.hpp"
#include "json.hpp"
#include "boost/process/system.hpp"

using json = nlohmann::json;

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
  return std::find(begin, end, option) != end;
}

std::string getCmdOptionByName(char** begin, char** end, const std::string& option)
{
  auto ret = std::find(begin, end, option);
  if(ret == end || ret+1 == end)
  {
    return "";
  }
  return std::string(*(ret+1));
}

int main(int argc, char * argv[])
{
  if(cmdOptionExists(argv, argv+argc, "-l"))
  {
    std::string xkbRuleDir = XKBTools::FindX11Dir();
    std::string xkbRuleFile = XKBTools::FindXKBRulesFile(xkbRuleDir);
    json rules = XKBTools::LoadXKBRules(xkbRuleFile);

    std::cout << rules.dump() << std::endl;
    return 0;
  }
  else if(cmdOptionExists(argv, argv+argc, "-c"))
  {
    json current = XKBTools::GetCurrentKeyboardSettings();
    std::cout << current.dump() << std::endl;
    return 0;
  }
  else if(cmdOptionExists(argv, argv+argc, "-s") || cmdOptionExists(argv, argv+argc, "--set"))
  {
    std::string layout  = getCmdOptionByName(argv, argv+argc, "layout");
    std::string variant = getCmdOptionByName(argv, argv+argc, "variant");

    if(layout.empty())
      return 1;

    std::stringstream ss;
    ss << "setxkbmap -layout " << layout;
    if(!variant.empty())
    {
      ss << " -variant " << variant;
    }
    return boost::process::system(ss.str());
  }
  return 1;
}