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

#ifndef WISENT_EXCEPTIONS_HPP_INCLUDED
#define WISENT_EXCEPTIONS_HPP_INCLUDED

#include <string>
#include <iostream>

namespace wisent {
  class Exception {
    char const *const  file;
    unsigned    const  line;
    std::string const  msg;

  protected:
    Exception(char const *_file, unsigned _line, std::string const& _msg)
      : file(_file), line(_line), msg(_msg) {}
    virtual ~Exception() {}

  public:
    virtual char const *getName() const = 0;
    void dump(std::ostream& o = std::cerr) const;
  };

  class OutOfMemoryException : public Exception {
  public:
    OutOfMemoryException(char const *_file, unsigned _line);
    ~OutOfMemoryException();

  public:
    char const *getName() const;
  };

  class BuildException : public Exception {
  public:
    BuildException(char const *_file, unsigned _line, std::string const& _msg);
    ~BuildException();

  public:
    char const *getName() const;
  };

  class NoEntryException : public Exception {
  public:
    NoEntryException(char const *_file, unsigned _line, std::string const& _msg);
    ~NoEntryException();

  public:
    char const *getName() const;
  };

  class FileNotFoundException : public Exception {
  public:
    FileNotFoundException(char const *_file, unsigned _line, std::string const& _msg);
    ~FileNotFoundException();

  public:
    char const *getName() const;
  };
}
#endif
