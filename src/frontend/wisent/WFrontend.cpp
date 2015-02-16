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

#include "WFrontend.hpp"

#include "WBuilder.hpp"
#include "WParser.hpp"

#include "../../Properties.hpp"

#include <fstream>

using namespace wisent::frontend::wisent;
using namespace wisent;

//+ Construction / Destruction +++++++++++++++++++++++++++++++++++++++++++++++
WFrontend::WFrontend(Properties const& props)
  : name(props.getProperty("NAME")) {}
WFrontend::~WFrontend() {}

void WFrontend::build(Grammar::Builder& _bld) {
  // open file
  std::string const  file(name + ".ypp");
  std::ifstream      src (file.c_str());

  // check if file exists
  {
    int  c = src.get();
    if(c == EOF)  throw FileNotFoundException(__FILE__, __LINE__, file);
    src.putback(c);
  }

  // parse the file
  WBuilder  bld(_bld, file);
  WParser::parse(bld, src);
}
