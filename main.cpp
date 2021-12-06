//
// Created by makoto on 2021/09/13.
//

#include <iostream>
#include <optional>
#include "fstream"

#include "GeographicLib/MGRS.hpp"
#include "GeographicLib/UTMUPS.hpp"
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"


namespace rx = rapidxml;

using Coordinate = std::pair<double, double>;

std::optional<double> stof(const char* str) {
  assert(str != nullptr);

  errno = 0;
  char* end;
  double v = strtof(str, &end);
  if (errno == ERANGE) {
    return std::nullopt;
  } else if (str == end) {
    return std::nullopt;
  }
  return std::make_optional(v);
}

Coordinate read_lanelet_point(const std::string& filename){
  rx::file<> input(filename.c_str());
  rx::xml_document<> doc;

  doc.parse<0>(input.data());
  rx::xml_node<>* osm = doc.first_node("osm");
  Coordinate osm_point;
  for (rx::xml_node<>* node = osm->first_node(); node != nullptr; node = node->next_sibling()) {
    if (std::string(node->name()) == "node"){
      // get lat point
      {
        auto v = stof(node->first_attribute("lat")->value());
        if (!v.has_value()) {
          continue;
        }
        osm_point.first = v.value();
      }

      // get lon point
      {
        auto v = stof(node->first_attribute("lon")->value());
        if (!v.has_value()) {
          continue;
        }
        osm_point.second = v.value();
      }
      break;
    }
  }

  // std::cout << osm_point.first << ", " << osm_point.second << std::endl;
  return osm_point;
}

Coordinate get_center_latlon (Coordinate latlon) {
  Coordinate output_latlon{};
  try {
    int zone {};
    bool northp {};
    std::string mgrs_code {};
    Coordinate utm_point{};

    GeographicLib::UTMUPS::Forward(latlon.first, latlon.second, zone, northp, utm_point.first, utm_point.second);
    GeographicLib::MGRS::Forward(zone, northp, utm_point.first, utm_point.second, latlon.first, 0, mgrs_code);

    // get mgrs center point from mgrs code
    Coordinate output_mgrs{};
    int precision;
    GeographicLib::MGRS::Reverse(mgrs_code, zone, northp, output_mgrs.first, output_mgrs.second, precision, false);

    double gamma, scale;
    GeographicLib::UTMUPS::Reverse(zone, northp, output_mgrs.first, output_mgrs.second, output_latlon.first, output_latlon.second, gamma, scale);
  } catch (const GeographicLib::GeographicErr& err){
    std::cerr << err.what() << std::endl;
    return Coordinate{};
  }

  return output_latlon;
}

int main(int argc, char *args[]) {
  if(argc != 2){
    std::cout << "invalid number of arguments: " << argc << ", Expected 2" << std::endl;
  }

  std::string filename(args[1]);

  Coordinate latlon = read_lanelet_point(filename);

  auto center_latlon = get_center_latlon(latlon);
  std::cout << std::fixed << "/**:" << std::endl
            << "  ros__parameters:" << std::endl
            << "    map_origin:" << std::endl
            << "      latitude: " << center_latlon.first << std::endl
            << "      longitude: " << center_latlon.second << std::endl
            << "      elevation: 0.0" << std::endl
            << "      roll: 0.0" << std::endl
            << "      pitch: 0.0" << std::endl
            << "      yaw: 0.0" << std::endl;
  return 0;
}


