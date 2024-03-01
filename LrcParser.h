/* *
 *
 * A simple LRC parser for C++
 *
 * Copyright (C) 2018 moonk5
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LRC_PARSER_H
#define LRC_PARSER_H

#include <chrono>
#include <fstream>
#include <regex>
#include <string>
#include <vector>

#define LRC_PARSER_MAJOR 1
#define LRC_PARSER_MINOR 1
#define LRC_PARSER_PATCH 0

namespace lrc
{
  const std::string LRC_PARSER_VERSION =
    std::to_string(LRC_PARSER_MAJOR) + "." +
    std::to_string(LRC_PARSER_MINOR) + "." +
    std::to_string(LRC_PARSER_PATCH);

  /*
   * Various ID Tags 
   * ar = Artist, al = Album, ti = Title
   * au = Creator of the Song text
   * length = length of the song
   * by = Creator of the LRC file 
   * offset = +/- Overall timestamp adjustment in milliseconds,
   *          + shifts time up, - shifts down
   * re = The player or editor that created the LRC file
   * ve = Version of program 
   */
  const std::string REGEX_ID_TAGS = 
    "\\[(ar|al|ti|au|length|by|offset|re|ve):(.*)\\]";
  const std::string REGEX_TIME_TAG =
    "\\[[0-5][0-9]:[0-5][0-9].[0-9][0-9]\\]";

  namespace time_conversion
  {
    std::string to_simple_string(unsigned int time_in_ms) {
      const char time_fmt[] = "%02d:%02d.%02d";
      std::vector<char> buff(sizeof(time_fmt));

      if (time_in_ms <= 0)
        return "";

      unsigned int minutes = 0;
      unsigned int seconds = 0;
      unsigned int milliseconds = time_in_ms % 1000;
      time_in_ms -= milliseconds;
      minutes = time_in_ms / (60 * 1000);
      time_in_ms -= minutes * (60 * 1000); 
      seconds = time_in_ms / 1000; 

      std::snprintf(&buff[0], buff.size(), time_fmt,
          minutes, seconds, milliseconds);

      return &buff[0];
    }

    unsigned int to_milliseconds(const std::string& time_in_str) {
      unsigned int milliseconds = 0;

      // check time format, mm:ss.SS
      std::regex r(lrc::REGEX_TIME_TAG);
      if (!std::regex_match(time_in_str, r))
        return milliseconds;  

      milliseconds += stoi(time_in_str.substr(1, 2)) * 60 * 1000;  // mins
      milliseconds += stoi(time_in_str.substr(4, 2)) * 1000;       // secs
      milliseconds += stoi(time_in_str.substr(7, 2));              // mils

      return milliseconds;
    }
  }

  struct time_tag 
  {
    unsigned int elapsed_time = 0; // unit in milliseconds
    std::string line_lyric = "";
    
    std::string to_json(bool pretty=false) {
      std::string str_json = "{\"time\":\""
        + time_conversion::to_simple_string(elapsed_time)
        + "\",\"lyric\":\"" + line_lyric + "\"}";

      return str_json;
    }
  };

  struct collection
  {
    void add_time(const time_tag& time_token) {
      time_tokens.push_back(time_token);
    }

    std::string id_tags_to_json_string() {
      std::string str_json = "";
      
      if (!ar.empty())
        str_json += "\"ar\":\"" + ar + "\",";
      if (!al.empty())
        str_json += "\"al\":\"" + al + "\",";
      if (!ti.empty())
        str_json += "\"ti\":\"" + ti + "\",";
      if (!au.empty())
        str_json += "\"au\":\"" + au + "\",";
      if (!by.empty())
        str_json += "\"by\":\"" + by + "\",";
      if (!re.empty())
        str_json += "\"re\":\"" + re + "\",";
      if (!ve.empty())
        str_json += "\"ve\":\"" + ve + "\",";
      if (!length.empty())
        str_json += "\"length\":\"" + length + "\",";
      if (!offset.empty())
        str_json += "\"offset\":\"" + offset + "\",";

      return str_json;
    }

    std::string time_tags_to_json_string() {
      std::string str_json = "\"time_tags\": [";
      
      for (time_tag t : time_tokens)
        str_json += t.to_json() + ",";

      return str_json.replace(str_json.size()-1, 1, 1, ']');
    }

    std::string to_json_string() {
      
      std::string str_json = "{";
      str_json += id_tags_to_json_string(); 
      str_json += time_tags_to_json_string();
      str_json += "}";

      return str_json;
    }

    void add_id_tag(const std::string& id, const std::string& value) {
      if (id == "ar")
        ar = value;
      else if (id == "al")
        al = value;
      else if (id == "ti")
        ti = value;
      else if (id == "au")
        au = value;
      else if (id == "by")
        by = value;
      else if (id == "re")
        re = value;
      else if (id == "ve")
        ve = value;
      else if (id == "length")
        length = value;
      else if (id == "offset")
        offset = value;
    }

    std::vector<time_tag> time_tokens;

    // id tags
    std::string ar = "";      // artist
    std::string al = "";      // album
    std::string ti = "";      // title
    std::string au = "";      // song writer
    std::string by = "";      // lrc writer
    std::string re = "";      // 
    std::string ve = "";      // version
    std::string offset = "";  // overall timestamp adjustment in milliseconds
                              // + shifts time up, - shifts time down
    std::string length = "";   // length of the song
  };

  class parser
  {
    public:
      parser(const std::string& filePath) {
        file_path = filePath;
      }
      
      // Actual parsing happens in this function
      bool load() {
        if (file_path.empty())
          return false;

        std::ifstream ifs(file_path);
        std::string lrc_content(
            (std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

        std::regex reg_ex(lrc::REGEX_TIME_TAG);
        std::smatch match;
        time_tag time_token; 

        // 1. Look for first time tag [mm:ss.SS]
        //    If there is no match, then return false
        if (!regex_search(lrc_content, match, reg_ex))
          return false;

        // 2. Already found first time tag
        //    If prefix exists before the first time tag,
        //    then assume it is a header / extra information
        if (match.prefix().str().length() > 0)
          parse_id_tags(match.prefix().str());

        // 3. Seaching through time tags and complete the 
        //    list of LRC time tokens
        time_token.elapsed_time =
          time_conversion::to_milliseconds(match[0]);
        lrc_content = match.suffix().str();

        while (std::regex_search(lrc_content, match, reg_ex)) {
          time_token.line_lyric =
            match.prefix().str().substr(0,
                match.prefix().str().find_last_not_of("\n") + 1);
          lrc_collection.add_time(time_token);

          time_token.elapsed_time = 
            time_conversion::to_milliseconds(match[0]);

          lrc_content = match.suffix().str();
        }

        // 4. Handle a corner case : matching the last time tag
        //    to the corresponding lyric
        time_token.line_lyric = lrc_content.substr(0,
            lrc_content.find_last_not_of("\n") + 1);
        lrc_collection.add_time(time_token);

        return true;
      }

      collection &get() {
        return lrc_collection;
      }

    private:
      // function to parsing id tags
      void parse_id_tags(std::string id_tags) {
        std::regex reg_ex(lrc::REGEX_ID_TAGS);
        //default constructor = end-of-sequence
        std::regex_token_iterator<std::string::iterator> rend;

        int submatches[] = {1, 2};
        std::regex_token_iterator<std::string::iterator> itr (id_tags.begin(),
            id_tags.end(), reg_ex, submatches);
        
        int i = 0;
        std::string id = "";
        while (itr != rend) {
          if (i % 2)
            lrc_collection.add_id_tag(id, *itr++);
          else
            id = *itr++;
          ++i;
        }
      }

      std::string file_path;
      collection lrc_collection;
  };
}

#endif // LRC_PARSER_H
