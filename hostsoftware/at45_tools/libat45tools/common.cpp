/**
 * This file is part of at45_tools
 *
 * (c) Fraunhofer ITWM - Mathias Dalheimer <dalheimer@itwm.fhg.de>, 2013
 *
 * at45_tools is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * at45_tools is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with at45_tools. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include "common.hpp"
#include <sstream>

namespace at45tools {

#define STR2(x) #x
#define STR(x) STR2(x)

	const char* version()
	{
		return STR(LIBAT45TOOLS_VERSION_MAJOR) "." STR(LIBAT45TOOLS_VERSION_MINOR) "." STR(LIBAT45TOOLS_VERSION_PATCH);
	}

}

