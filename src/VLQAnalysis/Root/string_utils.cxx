#include "VLQAnalysis/string_utils.h"

#include <algorithm> // transform, replace
#include <cstdlib> // strtod
#include <dirent.h> // DIR
#include <fstream>
#include <functional> // multiplies
#include <iostream>
#include <numeric> // accumulate
#include <iterator> // ostream_iterator, back_inserter
#include <sstream> // istringstream
#include <utility> // make_pair

using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;

using namespace vlq::utils;

//----------------------------------------------------------
std::string vlq::utils::rmLeadingTrailingWhitespaces(const std::string &str)
{
  string result;
  size_t startpos = str.find_first_not_of(" \t");
  size_t endpos = str.find_last_not_of(" \t");
  bool allSpaces = ( string::npos == startpos ) || ( string::npos == endpos);
  if(allSpaces)
      result = "";
  else
      result = str.substr(startpos, endpos-startpos+1);
  return result;
}
//----------------------------------------------------------
bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }
std::string vlq::utils::multipleSpaces2singleSpace(std::string str)
{ // from http://stackoverflow.com/questions/8362094/replace-multiple-spaces-with-one-space-in-a-string
  std::string::iterator new_end = std::unique(str.begin(), str.end(), BothAreSpaces);
 str.erase(new_end, str.end());
 return str;
}
//----------------------------------------------------------
std::string vlq::utils::commandLineArguments(int argc, char **argv)
{
    int iarg=0;
    std::ostringstream oss;
    while(iarg<argc){
        oss<<" "<<argv[iarg];
        iarg++;
    }
    return oss.str();
}
//----------------------------------------------------------
std::string vlq::utils::tab2space(std::string str)
{
    std::replace(str.begin(), str.end(), '\t', ' ');
    return str;
}
//----------------------------------------------------------
bool vlq::utils::contains(const std::string &str, const std::string &substr)
{
  return (str.find(substr)!=std::string::npos);
}
//----------------------------------------------------------
// http://stackoverflow.com/questions/874134/find-if-string-endswith-another-string-in-c
bool vlq::utils::endswith(const std::string &str, const std::string &ending) {
    if(str.length()<ending.length()) return false;
    else return (0==str.compare(str.length() - ending.length(), ending.length(), ending));
}
//----------------------------------------------------------
std::vector< std::string > vlq::utils::tokenize(const std::string &inputString, char separator)
{
  vector<string> tokens;
  std::istringstream buffer(string(multipleSpaces2singleSpace(tab2space(inputString))));
  for(string token; getline(buffer, token, separator); /*nothing*/ ) tokens.push_back(token);
  return tokens;
}
//----------------------------------------------------------
std::string vlq::utils::firstToken(const std::string &inputString, char separator)
{
    vector<string> tokens = tokenize(inputString, separator);
    string first;
    if(tokens.size()<1) {
        cerr<<"firstToken: zero tokens from '"<<inputString<<"' with separator '"<<separator<<"'"<<endl;
    } else {
        first = tokens[0];
    }
    return first;
}
//----------------------------------------------------------
double vlq::utils::string2double(const std::string &s) { return strtod(s.c_str(), NULL); }
//----------------------------------------------------------
double vlq::utils::multiply(const std::string &str)
{
  if(!contains(str, "*")) return string2double(str);
  vector<string> tks(tokenize(str, '*'));
  vector<double> factors;
  std::transform(tks.begin(), tks.end(), std::back_inserter(factors), string2double);
  return std::accumulate(factors.begin(), factors.end(), 1.0, std::multiplies<double>());
}
//----------------------------------------------------------
bool vlq::utils::isInt(const std::string& s)
{
  std::string rs(rmLeadingTrailingWhitespaces(s));
  if(rs.empty() || ((!isdigit(rs[0])) && (rs[0] != '-') && (rs[0] != '+'))) return false ;
  char * p ;
  strtol(rs.c_str(), &p, 10);
  return (*p == 0) ;
}
//----------------------------------------------------------
bool vlq::utils::isEmptyLine(const std::string &s)
{
    return vlq::utils::rmLeadingTrailingWhitespaces(s).size()==0;
}
//----------------------------------------------------------
bool vlq::utils::isCommentLine(const std::string &s, char commentCharacter)
{
    string strippedLine(vlq::utils::rmLeadingTrailingWhitespaces(s));
    return strippedLine.size() > 0 && strippedLine[0] == commentCharacter;
}
//----------------------------------------------------------
std::string vlq::utils::firstValidLine(const std::string &filename, char commentCharacter)
{
    string line;
    ifstream input;
    input.open(filename.c_str(), ifstream::in);
    if(input.is_open()) {
        while (std::getline(input, line)) {
            if(vlq::utils::isEmptyLine(line) or
               vlq::utils::isCommentLine(line, commentCharacter))
                continue;
            else
                break;
        } // while(getline)
    }
    return line;
}
//----------------------------------------------------------
std::vector<std::string> vlq::utils::filesFromDir(const std::string &dirname)
{
// from http://stackoverflow.com/questions/306533/how-do-i-get-a-list-of-files-in-a-directory-in-c
    vector<string> filenames;
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(dirname.c_str());
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf))){
            filenames.push_back(dirname+"/"+epdf->d_name);
        }
    }
    closedir(dpdf);
    return filenames;
}
//----------------------------------------------------------
