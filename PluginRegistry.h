/*
 * Copyright (C) 2018-present, Facebook, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

#include "oomd/OomDetector.h"
#include "oomd/OomKiller.h"

namespace Oomd {

template <typename T>
class PluginRegistry {
 public:
  using FactoryFunction = std::function<T*(const PluginArgs&)>;
  using FactoryMap = std::unordered_map<std::string, FactoryFunction>;

  bool add(const std::string& name, FactoryFunction fac) {
    if (map_.find(name) != map_.end()) {
      return false;
    }

    map_[name] = fac;
    return true;
  }

  T* create(const std::string& name, const PluginArgs& args) {
    if (map_.find(name) == map_.end()) {
      return nullptr;
    }

    return map_[name](args);
  }

 private:
  FactoryMap map_;
};

PluginRegistry<OomDetector>& getDetectorRegistry();
PluginRegistry<OomKiller>& getKillerRegistry();

#define REGISTER_DETECTOR_PLUGIN(plugin_name, create_func) \
  bool plugin_name##_detector_entry =                      \
      getDetectorRegistry().add(#plugin_name, (create_func))
#define REGISTER_KILLER_PLUGIN(plugin_name, create_func) \
  bool plugin_name##_killer_entry =                      \
      getKillerRegistry().add(#plugin_name, (create_func))

} // namespace Oomd
