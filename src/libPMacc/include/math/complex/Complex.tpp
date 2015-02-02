/**
 * Copyright 2015 Alexander Debus
 *
 * This file is part of libPMacc.
 *
 * libPMacc is free software: you can redistribute it and/or modify
 * it under the terms of of either the GNU General Public License or
 * the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libPMacc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License and the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and the GNU Lesser General Public License along with libPMacc.
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <cmath>
#include "math/Complex.hpp"
#include "algorithms/math.hpp"
#include "algorithms/TypeCast.hpp"
 
namespace PMacc
{
namespace algorithms
{
namespace math
{

/* specialize setting complex number by using Euler's formula */

template<typename Type>
struct Euler< ::PMacc::math::Complex<Type>, Type >
{
    typedef typename ::PMacc::math::Complex<Type> result;
        
    HDINLINE result operator( )(const Type &magnitude, const Type &phase)
    {
        return result(magnitude * picongpu::math::cos(phase),magnitude * picongpu::math::sin(phase));
    }
    
    HDINLINE result operator( )(const Type &magnitude, const Type &sinValue, const Type &cosValue)
    {
        return result(magnitude * cosValue, magnitude * sinValue);
    }
};

template<typename Type>
struct Sqrt< ::PMacc::math::Complex<Type>, Type >
{
    typedef typename ::PMacc::math::Complex<Type> result;
    
    HDINLINE result operator( )(const Complex<Type>& other)
    {
        Complex<Type> helper = other;
        if (other.real<=0.0 && other.imaginary==0.0) {
            return Complex<Type>(0.0, PMacc::algorithms::math::sqrt( -helper.real ) );
        }
        else {
            return PMacc::algorithms::math::sqrt( PMacc::algorithms::math::abs(other) )*(other+PMacc::algorithms::math::abs(other))
                /PMacc::algorithms::math::abs(other+PMacc::algorithms::math::abs(other));
        }
    }
};

template<typename Type>
struct exp< ::PMacc::math::Complex<Type>, Type >
{
    typedef typename ::PMacc::math::Complex<Type> result;
    
    HDINLINE result operator( )(const Complex<Type>& other)
    {
        return PMacc::algorithms::math::Euler(1.0,other.imaginary)*PMacc::algorithms::math::exp(other.real);
    }
};

template<typename Type>
struct Pow< ::PMacc::math::Complex<Type>, Type >
{
    typedef typename ::PMacc::math::Complex<Type> result;
    
    HDINLINE result operator( )(const Complex<Type>& other, const Type& exponent)
    {
        return PMacc::algorithms::math::pow( PMacc::algorithms::math::abs(other),exponent )
                *PMacc::algorithms::math::exp( Complex<Type>(0.,1.)*PMacc::algorithms::math::Arg(other)*exponent );
    }
};

template<typename Type>
struct Abs< ::PMacc::math::Complex<Type>, Type >
{
    HDINLINE typename Type operator( )(const Complex<Type>& other)
    {
        return PMacc::algorithms::math::sqrt( PMacc::algorithms::math::abs2(other.real) + PMacc::algorithms::math::abs2(other.imaginary) );
    }
};

template<typename Type>
struct Abs2< ::PMacc::math::Complex<Type>, Type >
{
    HDINLINE typename Type operator( )(const Complex<Type>& other)
    {
        return PMacc::algorithms::math::abs2(other.real) + PMacc::algorithms::math::abs2(other.imaginary);
    }
};

// Phase of complex number (Note: Branchcut running from -infinity to 0)
template<typename Type>
struct Arg< ::PMacc::math::Complex<Type>, Type >
{
    HDINLINE typename Type operator( )(const Complex<Type>& other)
    {
        if (other.real==0.0 && other.imaginary==0.0) return 0.0;
        else if (other.real==0.0 && other.imaginary>0.0) return M_PI/2;
        else if (other.real==0.0 && other.imaginary<0.0) return -M_PI/2;
        else if (other.real<0.0 && other.imaginary==0.0) return M_PI;
        else return PMacc::algorithms::math::atan2(imaginary,real);
    }
};

template<typename Type>
struct Zero< ::PMacc::math::Complex<Type>, Type >
{
    typedef typename ::PMacc::math::Complex<Type> result;
    
    HDINLINE result operator( )()
    {
        return result(0.0, 0.0);
    }
};

} //namespace math
} //namespace algorithms
} // namespace PMacc

namespace PMacc
{
namespace algorithms
{
namespace precisionCast
{

template<typename CastToType>
struct TypeCast<CastToType, ::PMacc::math::Complex<CastToType> >
{
    typedef const ::PMacc::math::Complex<CastToType>& result;

    HDINLINE result operator( )(const ::PMacc::math::Complex<CastToType>& complexNumber ) const
    {
        return complexNumber;
    }
};

template<typename CastToType, typename OldType>
struct TypeCast<CastToType, ::PMacc::math::Complex<OldType> >
{
    typedef ::PMacc::math::Complex<CastToType> result;

    HDINLINE result operator( )(const ::PMacc::math::Complex<OldType>& complexNumber ) const
    {
        return result( complexNumber );
    }
};

} //namespace typecast
} //namespace algorithms
} //PMacc
