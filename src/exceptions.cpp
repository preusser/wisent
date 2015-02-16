/***
 * wisent - LALR(1) Parser Generator
 * Copyright (C) 2004,2005 Thomas B. Preuﬂer <preusser@ite.inf.tu-dresden.de>
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

#include "exceptions.hpp"

using namespace wisent;

//+ Exception ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Exception::dump(std::ostream& o) const {
  o << getName();
  if(file != 0)  o << " @ " << file << ':' << line;
  o << std::endl;
  if(!msg.empty()) o << "\t\"" << msg << '"' << std::endl;
}

//+ OutOfMemoryException +++++++++++++++++++++++++++++++++++++++++++++++++++++
OutOfMemoryException::OutOfMemoryException(char const *_file, unsigned _line)
  : Exception(_file, _line, "") {}
OutOfMemoryException::~OutOfMemoryException() {}

char const *OutOfMemoryException::getName() const {
  return  "OutOfMemoryException";
}

//+ BuildException +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
BuildException::BuildException(char const *_file, unsigned _line, 
			       std::string const& _msg)
  : Exception(_file, _line, _msg) {}
BuildException::~BuildException() {}

char const *BuildException::getName() const {
  return  "BuildException";
}

//+ NoEntryException +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
NoEntryException::NoEntryException(char const *_file, unsigned _line, 
				   std::string const& _msg)
  : Exception(_file, _line, _msg) {}
NoEntryException::~NoEntryException() {}

char const *NoEntryException::getName() const {
  return  "NoEntryException";
}

//+ FileNotFoundException ++++++++++++++++++++++++++++++++++++++++++++++++++++
FileNotFoundException::FileNotFoundException
(char const *_file, unsigned _line, std::string const& _msg)
  : Exception(_file, _line, _msg) {}
FileNotFoundException::~FileNotFoundException() {}

char const *FileNotFoundException::getName() const {
  return  "FileNotFoundException";
}
