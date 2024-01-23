#pragma once

#include <borealis.hpp>

namespace activity {
class MainActivity : public brls::Activity {
public:
  CONTENT_FROM_XML_RES("activity/main.xml");
};
}
