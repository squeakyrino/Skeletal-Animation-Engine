#include <Exceptions/NotInversibleException.h>

namespace Engine {

	const char* NotInversibleException::what() const throw()
	{
		return "Matrix not inversable";
	}
}