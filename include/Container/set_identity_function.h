#ifndef _SET_IDENTITY_FUNCTION_H_
#define _SET_IDENTITY_FUNCTION_H_

// System includes
#include <functional>

template<class TDataType> 
class SetIdentityFunction
{
public:
	TDataType& operator()(TDataType& data)
	{
		return data;
	}
	const TDataType& operator()(const TDataType& data) const
	{
		return data;
	}
};
#endif // KRATOS_SET_IDENTITY_FUNCTION_H_INCLUDED  defined 
