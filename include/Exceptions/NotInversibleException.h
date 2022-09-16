#pragma once
#include <exception>

namespace Engine {
	class NotInversibleException :
		public std::exception
	{
		virtual const char* what() const throw();
		
	};
}
