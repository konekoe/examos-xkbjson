#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>

#include <boost/filesystem.hpp>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

#include "json.hpp"

using json = nlohmann::json;

namespace XKBTools
{
  /* Find directory whith xkb config files */
  std::string FindX11Dir();
  /* Use two methods to find the rules file to list the keyboard layouts */
  std::string FindXKBRulesFile(const std::string& X11Dir);
  /* Load the XKB data from internal X11 structs to our maps */
  json LoadXKBRules(std::string& XKBRulesFile);

  json GetCurrentKeyboardSettings();
}