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

#include "CPPBackend.hpp"

using namespace wisent;
using namespace wisent::backend::cpp;

//+ Backend Registration +++++++++++++++++++++++++++++++++++++++++++++++++++++
ImplementationPool<Backend>::Entry<CPPBackend>
CPPBackend::registrar("cpp", "C++ Backend [lineinfo=no - disable #line directives]");
