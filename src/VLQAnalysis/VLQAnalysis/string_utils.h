// Dear emacs, this is -*- c++ -*-
#ifndef VLQ_STRING_UTILS_H
#define VLQ_STRING_UTILS_H

/*
  String manipulation utility functions

  Mostly useful to parse text files.

  davide.gerbaudo@gmail.com
  June 2014
*/

#include <string>
#include <vector>

namespace vlq{
namespace utils{

typedef std::vector< std::string > vstring_t;

/// return a copy of the str where the leading and trailing space+tab have been dropped
std::string rmLeadingTrailingWhitespaces(const std::string &str);
/// given str, return a copy where contiguous whitespaces have been replaced by single whitespaces
std::string multipleSpaces2singleSpace(std::string str);
/// return a string reproducting the command being executed 
std::string commandLineArguments(int argc, char **argv);
/// given str, return a copy where tabs have been replaced with whitespaces
std::string tab2space(std::string str);
/// whether str contains substr
bool contains(const std::string &str, const std::string &substr);
/// whether str ends with ending
bool endswith(const std::string &str, const std::string &ending);
/// split inputString in a vector of tokens
std::vector< std::string > tokenize(const std::string &inputString, char separator);
/// tokenize and return the first word
std::string firstToken(const std::string &inputString, char separator);
/// convert a string to a double
double string2double(const std::string &s);
/// convert a string to a double accounting for '*' multiplicative factors
double multiply(const std::string &str);
/// whether a string can be safely converted to an int
bool isInt(const std::string& s);
/// an empty line contains spaces or nothing at all
bool isEmptyLine(const std::string &s);
/// a comment line is a line that begins with a comment character (possibily preceded by whitespaces) 
bool isCommentLine(const std::string &s, char commentCharacter);
/// first non-empy, non-comment line from a txt file
std::string firstValidLine(const std::string &filename, char commentCharacter='#');
/// given a directory, return the list of its files
/**
   This function could be in another header.
 */
std::vector<std::string> filesFromDir(const std::string &dirname);

} // utils
} // vlq
#endif
