/*
 * Fountain Screenplay Processor
 * Copyright 2021 xiota
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <map>
#include <string>
#include <vector>

namespace Fountain {

enum ScriptNodeType : size_t {
  ftnNone = 0,
  ftnUnknown = 1ull,
  ftnKeyValue = 1ull << 1,
  ftnContinuation = 1ull << 2,
  ftnPageBreak = 1ull << 3,
  ftnBlankLine = 1ull << 4,
  ftnSceneHeader = 1ull << 5,
  ftnAction = 1ull << 6,
  ftnActionCenter = 1ull << 7,
  ftnTransition = 1ull << 8,
  ftnDialog = 1ull << 9,
  ftnDialogLeft = 1ull << 10,
  ftnDialogRight = 1ull << 11,
  ftnCharacter = 1ull << 12,
  ftnParenthetical = 1ull << 13,
  ftnSpeech = 1ull << 14,
  ftnNotation = 1ull << 15,
  ftnLyric = 1ull << 16,
  ftnSection = 1ull << 17,
  ftnSynopsis = 1ull << 18,
};

class ScriptNode {
 public:
  std::string to_string(size_t const &flags = ScriptNodeType::ftnNone) const;

  void clear() {
    type = ScriptNodeType::ftnUnknown;
    key.clear();
    value.clear();
  }

 public:
  ScriptNodeType type = ScriptNodeType::ftnUnknown;
  std::string key;
  std::string value;
};

class Script {
 public:
  Script() = default;
  Script(std::string const &text) { parseFountain(text); }

  void clear() {
    nodes.clear();
    curr_node.clear();
  }

  void parseFountain(std::string const &text);
  std::string to_string(size_t const &flags = ScriptNodeType::ftnNone) const;

 public:
  std::vector<ScriptNode> nodes;
  std::map<std::string, std::string> metadata;

 private:
  ScriptNode curr_node;

  std::string parseNodeText(std::string const &input);

  void new_node(ScriptNodeType const &type, std::string const &key = "",
                std::string const &value = "") {
    end_node();
    curr_node = {type, key, value};
  }
  void end_node() {
    if (curr_node.type != ScriptNodeType::ftnUnknown) {
      curr_node.value = parseNodeText(curr_node.value);
      nodes.push_back(curr_node);
      curr_node.clear();
    }
  }
  void append(std::string const &s) {
    if (curr_node.value.size()) {
      curr_node.value += '\n';
    }
    curr_node.value += s;
  }
};

// html output compatible with screenplain css files
std::string ftn2screenplain(std::string const &input,
                            std::string const &css_fn = "screenplain.css");

// html output compatible with textplay css files
std::string ftn2textplay(std::string const &input,
                         std::string const &css_fn = "textplay.css");

// possibly compatible with finaldraft fdx files
std::string ftn2fdx(std::string const &input);

// native output; modern browsers can display with css
std::string ftn2xml(std::string const &input, std::string const &css_fn = "fountain-xml.css",
                    bool const &embed_css = true);
std::string ftn2html(std::string const &input, std::string const &css_fn = "fountain-html.css",
                     bool const &embed_css = true);

bool ftn2pdf(std::string const &fn, std::string const &input,
             std::string const &css_fn = "");

}  // namespace Fountain
