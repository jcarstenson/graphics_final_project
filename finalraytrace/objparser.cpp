#include "objparser.h"
#include "parser.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::ifstream;
using std::vector;

using namespace cs40;

ObjParser::ObjParser() {
    index = 1;
}

ObjParser::~ObjParser() {
}

void ObjParser::parseObj(const string &fname) {
    ifstream infile;
    infile.open(fname.c_str());

    if(!infile.good()){
        cout << "error opening file " << fname << endl;
        return;
    }

    int lno = 1; //line number
    string line; //current line
    vector<string> words;


    while(!infile.eof()){
        getline(infile, line);
        //cout << lno << ": " << line.length() << " " << line <<  endl;
        words = split(line);
        if(words.size()>0){
            try{
                parseObjLine(words);
            }
            catch(parser_error e){
                cout << "Error on line " << lno << ": " << line << endl;
                cout << e.what() << endl;
            }
        }
        lno++;
    }

    infile.close();
    index = 1;
}

void ObjParser::parseObjLine(const vector<string> &words) {
    string cmd = words[0];

    if(cmd == "v") {
        float v1 = parseFloat(words[1]);
        float v2 = parseFloat(words[2]);
        float v3 = parseFloat(words[3]);


        //v1 *= 50;
        //v2 *= 50;
        //v3 *= 50;

        //Temporary scales, translations, and rotations to alter obj file vertices
        v1 *= 5;
        v2 *= 5;
        v3 *= 5;

        v1 = v1;
        float temp = v2;
        v2 = v3;
        v3 = temp * -1;

        v2 -= 4.5;

        v1 -= 4.5;


        //v1 *= 10;
        //v2 *= 10;
        //v3 *= 10;

        vec3 v = vec3(v1, v2, v3);
        vList.insert(index, v);
        index++;
    }

    if(cmd == "f") {
        int size = vList.size();
        size++;

        int f1 = parseInt(words[1]);
        int f2 = parseInt(words[2]);
        int f3 = parseInt(words[3]);

        if (f1 < 0)
            f1 += size;

        if (f2 < 0)
            f2 += size;

        if (f3 < 0)
            f3 += size;

        Triangle *t = new Triangle(vList.value(f1), vList.value(f2), vList.value(f3));

        tList.push_back(t);
    }


}
