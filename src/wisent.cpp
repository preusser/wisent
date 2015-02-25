/***
 * wisent - LALR(1) Parser Generator
 * Copyright (C) 2004-2015 Thomas B. Preußer <thomas.preusser@utexas.edu>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "Properties.hpp"
#include "Frontend.hpp"
#include "Backend.hpp"
#include "ImplementationPool.hpp"
#include "Grammar.hpp"

#include "exceptions.hpp"

#include <iostream>
#include <fstream>
#include <memory>
#include <string.h>

using namespace wisent;

int analyze_spec(char const*& name, Properties& props, char *spec) {
  char const *apos = strchr(spec, ':');

  // isolate properties
  name = spec;
  if(apos == 0)  return  0;
  *const_cast<char*>(apos++) = '\0';

  while(*apos != '\0') {
    char const *end, *val;
    
    end = strchr(apos, ',');
    if(end != 0) *const_cast<char*>(end++) = '\0';
    else  end = "";
    val = strchr(apos, '=');
    if(val != 0) *const_cast<char*>(val++) = '\0';
    else  val = "";
    props.setProperty(apos, val);
    apos = end;
  }
  return  0;
}

int main(int const  argc, char *const  argv[]) {
  std::ofstream   log; // log stream to be opened
  std::streambuf *clg; // original clog streambuf

  char const *fname = "wisent";
  char const *bname = "cpp";
  std::string  gname;
  Properties  _props;
  Properties  fprops(&_props);
  Properties  bprops(&_props);

  { // Evaluate command-line arguments
    int  idx = 1;
    while(true) {
      char *const  arg = argv[idx];
      
      if(arg == 0) {
	std::cerr << "Missing grammar specification.\n" << std::endl;
	return  1;
      }
      
      if(arg[0] == '-') { // option processing
	switch(arg[1]) {
	case 'f':
	case 'b': {
	  char *const  spec = (arg[2] == '\0')? argv[++idx] : arg+2;
	  bool  const  f    = arg[1] == 'f';
	  
	  if(analyze_spec(f? fname:bname, f? fprops:bprops, spec) != 0) {
	    std::cerr << "Syntax error in "
		      << (f? "front":"back") << "end specification: \""
		      << spec << "\"\n" << std::endl;
	    return  1;
	  }
	  idx++;
	  break;
	}
	case '?':
	  std::cerr << "Usage: " << argv[0]
		    << " [-f <frontend>[:<props>]] "
	               "[-b <backend>[:<props>]] <grammar>\n\n"
	               "\t<props> ::= <prop>[,<props>]\n"
	               "\t<prop>  ::= <name>[=<value>]\n\n"
	               "Frontends:\n"
 	               "----------\n";
	  ImplementationPool<Frontend>::dump(std::cerr);
	  std::cerr << "\n"
                       "Backends:\n"
                       "---------\n";
	  ImplementationPool<Backend> ::dump(std::cerr);
	  std::cerr << std::endl;
	  return  0;

	default:
	  std::cerr << "Unknown option '" << arg[1] << "'.\n" << std::endl;
	  return  1;
	}
      }
      else {
	if(idx+1 == argc) {
	  gname = arg; {
	    // Normalize grammar name
	    size_t const  l = gname.length();
	    if((l >= 4) && (gname.compare(l-4, 4, ".ypp") == 0)) {
	      gname = gname.substr(0, l-4);
	    }
	  }
	  _props.setProperty("NAME", gname.c_str());
	  log.open((gname + ".log").c_str());
	  break;
	}
	std::cerr << "Extra argument beyond grammar specification \""
		  << arg << "\".\n" << std::endl;
	return  1;
      }
    }
  }
  
  try {
    // Create Frontend and Backend
    std::auto_ptr<Frontend> const
      frnt(ImplementationPool<Frontend>::create(fname, fprops));
    std::auto_ptr<Backend>  const
      back(ImplementationPool<Backend> ::create(bname, bprops));

    if(frnt.get() == 0) {
      std::cerr << "Frontend \"" << fname << "\" not found.\n" << std::endl;
      return  1;
    }
    if(back.get() == 0) {
      std::cerr << "Backend \""  << bname << "\" not found.\n" << std::endl;
      return  1;
    }

    clg = std::clog.rdbuf(log.rdbuf());

    Grammar  grm(*frnt);
    grm.echo   (std::clog);
    grm.warn   (std::cerr);
    grm.analyze(std::clog);
    back->accept(grm);      

    std::clog.rdbuf(clg);
  }
  catch(Exception& e) {
    e.dump(std::cerr);
    return  2;
  }
  return  0;
}
