#ifndef CS40PARSER_H
#define CS40PARSER_H

#include <string>
#include <stdexcept>
#include <vector>
#include "common.h"

namespace cs40{

//A basic exception type we can throw when we have problems parsing
struct parser_error : public std::runtime_error {
    parser_error(std::string err) : std::runtime_error(err) {};
};

/* trim a string by removing any trailing comments
 * (starting with a #) and any leading/trailng whitespace.
 * Return a new string */
std::string trim(std::string line);

/* trim a line and then split it into a vector of words, using
 * whitespace as a delimiter. Return resulting vector*/
std::vector<std::string> split(std::string line);

/* convert given word to a float. Throws parser_error if problem.
 * returns valid float otherwise */
float parseFloat(const std::string& word);

/* convert given word to an int. Throws parser_error if problem.
 * returns valid int otherwise */
int parseInt(const std::string& word);

/* converts three consecutive floats in a vector of strings
 * to an vec3 object, starting from index pos.
 * Throws parser_error if problem. returns valid vec3 otherwise */
vec3 parseVec3(std::vector<std::string> words, int pos);

/* checks that a given vector has a specified minimum number
 * of arguments (ignoring the first element in the vector).
 * If the vector has more than the minimum number of arguments
 * and a nonzero maxargs is specified, this function also checks that
 * the number of arguments is either exactly minargs or
 * exactly maxargs. Throws parser_error if problems */
void checksize(const std::vector<std::string>& words,
               unsigned int minargs, unsigned int maxargs=0);

}
#endif
