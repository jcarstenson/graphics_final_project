#include <sstream>
#include "parser.h"

using std::string;
using std::vector;
using std::stringstream;
using std::istringstream;

string cs40::trim(string line){
    size_t pos;

    //cut off comments
    pos = line.find_first_of("#");
    if(pos!=string::npos){
        line = line.substr(0,pos);
    }

    //cut leading whitespace
    pos = line.find_first_not_of(" \t");
    if(pos!=string::npos){
        line = line.substr(pos);
    }

    //cut trailing whitespace
    pos = line.find_last_not_of(" \t");
    if(pos!=string::npos){
        line = line.substr(0,pos+1);
    }

    return line;
}

vector<string> cs40::split(string line){
    vector<string> result;
    line = trim(line);
    stringstream ss;
    ss << line;
    string token;
    while(ss >> token){
        result.push_back(token);
    }
    return result;
}

float cs40::parseFloat(const string& word){
    istringstream s(word);
    float ans;
    if(s >> ans){
        return ans;
    }
    else{
        throw parser_error("Unable to parse float: " + word);
    }
}

int cs40::parseInt(const string& word){
    istringstream s(word);
    int ans;
    if(s >> ans){
        return ans;
    }
    else{
        throw parser_error("Unable to parse int: " + word);
    }
}

vec3 cs40::parseVec3(vector<string> words, int pos){
    vec3 ans;
    ans = vec3(parseFloat(words[pos++]),
            parseFloat(words[pos++]),
            parseFloat(words[pos]));
    return ans;
}

void cs40::checksize(const vector<string>& words, unsigned int minargs, unsigned int maxargs){
    stringstream msg;
    unsigned int len = words.size()-1; //only count arguments
    if(len < minargs){
        msg  << "Expect at least " << minargs << " arguments (got " << len << ")";
        throw parser_error(msg.str());
    }
    else if(maxargs != 0 && len > minargs && len != maxargs){
        msg << "Expect either " << minargs << " or " << maxargs << " arguments (got "
            << len << ")";
        throw parser_error(msg.str());
    }
}
