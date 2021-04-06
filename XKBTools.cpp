#include "XKBTools.hpp"

namespace XKBTools
{
  static const std::vector<std::string> X11DirList = {
    "/etc/X11/",
    "/usr/share/X11/",
    "/usr/local/share/X11/",
    "/usr/X11R6/lib/X11/",
    "/usr/X11R6/lib64/X11/",
    "/usr/local/X11R6/lib/X11/",
    "/usr/local/X11R6/lib64/X11/",
    "/usr/lib/X11/",
    "/usr/lib64/X11/",
    "/usr/local/lib/X11/",
    "/usr/local/lib64/X11/",
    "/usr/pkg/share/X11/",
    "/usr/pkg/xorg/lib/X11/"
  };

  static const std::vector<std::string> XKBRulesFileList = {
    "xkb/rules/base",
    "xkb/rules/xorg",
    "xkb/rules/xfree86"
  };


  static Display* GetDefaultDisplay()
  {
    return XOpenDisplay(nullptr);
  }

  std::string FindX11Dir()
  {
    boost::system::error_code ec;
    for(auto it = X11DirList.begin(); it != X11DirList.end(); it++)
    {
      const std::string currDir = *it;
      if(boost::filesystem::is_directory(std::string(currDir + "xkb/rules"), ec))
      {
        return currDir;
      }
    }
    return "";
  }

  std::string FindXKBRulesFile(const std::string& X11Dir)
  {
    std::string rulesFilePath = "";
    XkbRF_VarDefsRec vd;
    char* tmp = nullptr;

    Display* dpy = GetDefaultDisplay();
    if(!dpy)
    {
      return "";
    }

    if(XkbRF_GetNamesProp(dpy, &tmp, &vd) && tmp != nullptr)
    {
      rulesFilePath = std::string(X11Dir + "xkb/rules/" + tmp);
    }
    else
    {
      // Fallback method
      boost::system::error_code ec;
      for(auto it = XKBRulesFileList.begin(); it != XKBRulesFileList.end(); it++)
      {
        const std::string currRuleFile = *it;
        std::string currRuleFilePath = X11Dir + currRuleFile;
        if(boost::filesystem::exists(currRuleFilePath, ec))
        {
          rulesFilePath = currRuleFilePath;
          break;
        }
      }
    }
    if(tmp)
      delete(tmp);
    return rulesFilePath;
  }

  
  json LoadXKBRules(std::string& XKBRulesFile)
  {
    json rules;
    // bullet proof const casts?
    XkbRF_RulesPtr XKBRulesPtr = XkbRF_Load(const_cast<char*>(XKBRulesFile.c_str()), const_cast<char*>(""), true, true);
    if(!XKBRulesPtr)
    {
      return rules;
    }


    for(int i = 0; i < XKBRulesPtr->layouts.num_desc; i++)
    {
      rules["layouts"][XKBRulesPtr->layouts.desc[i].name] = XKBRulesPtr->layouts.desc[i].desc;
    }

    for (int i = 0; i < XKBRulesPtr->models.num_desc; i++)
    {
      rules["models"][XKBRulesPtr->models.desc[i].name] = XKBRulesPtr->models.desc[i].desc;
    }

    for (int i = 0; i < XKBRulesPtr->variants.num_desc; i++)
    {
      std::string descstr = XKBRulesPtr->variants.desc[i].desc;
      std::size_t delim = descstr.find(":");
      std::string layout = descstr.substr(0, delim);
      std::string desc = descstr.substr(delim + 2, descstr.length() - delim - 2);

      rules["variants"][layout][XKBRulesPtr->variants.desc[i].name] = desc;
    }

    for (int i = 0; i < XKBRulesPtr->options.num_desc; i++)
    {
      // omit adding options to the list ? I think this is not needed.
      //rules.options[XKBRulesPtr->options.desc[i].name] = XKBRulesPtr->options.desc[i].desc;
    }

    return rules;
  }

  json GetCurrentKeyboardSettings()
  {
    json ret;

    Display* dpy = GetDefaultDisplay();
    if(!dpy)
      return ret;

    XkbStateRec state;
    XkbGetState(dpy, XkbUseCoreKbd, &state);

    XkbDescPtr desc = XkbGetKeyboard(dpy, XkbAllComponentsMask, XkbUseCoreKbd);
    char *group = XGetAtomName(dpy, desc->names->groups[state.group]);
    if(!group)
      return ret;

    ret["desc"] = group;

    XkbRF_VarDefsRec vd;
    XkbRF_GetNamesProp(dpy, nullptr, &vd);

    std::string layout = vd.layout;
    layout = layout.substr(0, layout.find(","));
    ret["layout"] = layout;

    if(vd.variant)
    {
      std::string variant = vd.variant;
      variant = variant.substr(0, variant.find(","));
      ret["variant"] = variant;
    }

    XFree(group);
    XCloseDisplay(dpy);

    return ret;
  }

  
}