/***************************************************************************
 *
 * $Id$
 *
 * Author: Brian Lasiuk, Thomas Ullrich, April 1998
 ***************************************************************************
 *
 * Description:
 *
 * Remarks:   Since not all compilers support member templates
 *            we have to specialize the templated member on these
 *            platforms. If member templates are not supported the
 *            ST_NO_MEMBER_TEMPLATES flag has to be set. tu.
 *
 ***************************************************************************
 *
 * $Log$
 * Revision 1.1.1.1  2007/04/25 15:38:41  panos
 * Importing the HBT code dir
 *
 * Revision 1.1.1.1  2007/03/07 10:14:49  mchojnacki
 * First version on CVS
 *
 * Revision 1.15  2005/09/22 20:09:20  fisyak
 * Make AliFmLorentzVector persistent
 *
 * Revision 1.14  2005/07/19 22:27:11  perev
 * Cleanup
 *
 * Revision 1.13  2005/07/06 18:49:57  fisyak
 * Replace AliFmHelixD, AliFmLorentzVectorD,AliFmLorentzVectorF,AliFmMatrixD,AliFmMatrixF,AliFmPhysicalHelixD,AliFmThreeVectorD,AliFmThreeVectorF by templated version
 *
 * Revision 1.12  2005/03/28 06:03:41  perev
 * Defence FPE added
 *
 * Revision 1.11  2004/12/02 20:07:32  fine
 * define the valid method for both flavor of AliFmThreeVector
 *
 * Revision 1.10  2003/10/30 20:06:46  perev
 * Check of quality added
 *
 * Revision 1.9  2003/09/02 17:59:35  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 1.8  2002/06/21 17:47:37  genevb
 * Added pseudoProduct
 *
 * Revision 1.7  2000/01/04 19:56:05  ullrich
 * Added cpp macro for CINT.
 *
 * Revision 1.6  1999/12/21 15:14:31  ullrich
 * Modified to cope with new compiler version on Sun (CC5.0).
 *
 * Revision 1.5  1999/10/15 15:46:54  ullrich
 * Changed output format in operator<<
 *
 * Revision 1.4  1999/06/04 18:00:05  ullrich
 * Added new constructor which takes C-style array as argument.
 * New operators operator() and operator[] which can be used
 * as lvalues.
 *
 * Revision 1.3  1999/02/17 11:42:19  ullrich
 * Removed specialization for 'long double'.
 *
 * Revision 1.2  1999/02/14 23:11:48  fisyak
 * Fixes for Rootcint
 *
 * Revision 1.1  1999/01/30 03:59:05  fisyak
 * Root Version of AliFmarClassLibrary
 *
 * Revision 1.1  1999/01/23 00:28:04  ullrich
 * Initial Revision
 *
 **************************************************************************/
/*//
//// General class for a three-vector
///*/
#ifndef ST_THREE_VECTOR_HH
#define ST_THREE_VECTOR_HH
#ifdef __ROOT__
#include "Rtypes.h"
#endif
#ifndef __CINT__
#include <iostream>
#include <fstream>
#include <math.h>
#ifdef GNU_GCC
#    include <stddef.h>
#endif
#if defined (__SUNPRO_CC) && __SUNPRO_CC < 0x500
#    include <stdcomp.h>
#endif
#ifndef ST_NO_EXCEPTIONS
#    include <stdexcept>
#    if !defined(ST_NO_NAMESPACES)
using std::out_of_range;
#    endif
#endif
#endif // __CINT__

#ifdef WIN32
#include "gcc2vs.h"
#endif

class TRootIOCtor;//nic nie rozumiem
using namespace std;


template<class T> class AliFmThreeVector {
public:
    AliFmThreeVector(T = 0, T = 0, T = 0);
  //                     ROOT_VERSION(5,03,01)
#if ROOT_VERSION_CODE >= 328449
   AliFmThreeVector(TRootIOCtor*) : mX1(0), mX2(0), mX3(0) {}
#endif
    virtual ~AliFmThreeVector();

#ifndef ST_NO_MEMBER_TEMPLATES
    template<class X> AliFmThreeVector(const AliFmThreeVector<X>&);
    template<class X> AliFmThreeVector(const X*);
    template<class X> AliFmThreeVector<T>& operator=(const AliFmThreeVector<X>&);
    // AliFmThreeVector(const AliFmThreeVector<T>&);                use default
    // AliFmThreeVector<T>& operator=(const AliFmThreeVector<T>&);  use default
#else
    AliFmThreeVector(const AliFmThreeVector<float>&);
    AliFmThreeVector(const AliFmThreeVector<double>&);

    AliFmThreeVector(const float*);
    AliFmThreeVector(const double*);

    AliFmThreeVector<T>& operator=(const AliFmThreeVector<float>&);
    AliFmThreeVector<T>& operator=(const AliFmThreeVector<double>&);
#endif

    void SetX(T);
    void SetY(T);
    void SetZ(T);

    void SetPhi(T);
    void SetTheta(T);
    void SetMag(T);
    void SetMagnitude(T);

    T   x()                        const;
    T   y()                        const;
    T   z()                        const;
    T   Theta()                    const;
    T   CosTheta()                 const;
    T   Phi()                      const;
    T   Perp()                     const;
    T   Perp2()                    const;
    T   Magnitude()                const;
    T   Mag()                      const;
    T   Mag2()                     const;
    T   PseudoRapidity()           const;
    T   operator() (size_t)        const;
    T   operator[] (size_t)        const;

    T&  operator() (size_t);
    T&  operator[] (size_t);

    T   MassHypothesis(T mass)     const;

    AliFmThreeVector<T>  Unit()       const;
    AliFmThreeVector<T>  Orthogonal() const;

    void  RotateX(T);
    void  RotateY(T);
    void  RotateZ(T);

    AliFmThreeVector<T>  operator- ();
    AliFmThreeVector<T>  operator+ ();
    AliFmThreeVector<T>& operator*= (double);
    AliFmThreeVector<T>& operator/= (double);
    AliFmThreeVector<T>  PseudoProduct(double,double,double) const;

#ifndef ST_NO_MEMBER_TEMPLATES
    template<class X> T                Angle(const AliFmThreeVector<X>&) const;
    template<class X> AliFmThreeVector<T> Cross(const AliFmThreeVector<X>&) const;
    template<class X> T                Dot  (const AliFmThreeVector<X>&) const;
    template<class X> AliFmThreeVector<T> PseudoProduct(const AliFmThreeVector<X>&) const;

    template<class X> bool operator == (const AliFmThreeVector<X>& v) const;
    template<class X> bool operator != (const AliFmThreeVector<X>& v) const;

    template<class X> AliFmThreeVector<T>& operator+= (const AliFmThreeVector<X>&);
    template<class X> AliFmThreeVector<T>& operator-= (const AliFmThreeVector<X>&);
#else
    T                Angle(const AliFmThreeVector<float>&) const;
    AliFmThreeVector<T> Cross(const AliFmThreeVector<float>&) const;
    T                Dot  (const AliFmThreeVector<float>&) const;
    AliFmThreeVector<T> PseudoProduct(const AliFmThreeVector<float>&) const;

    T                Angle(const AliFmThreeVector<double>&) const;
    T                Dot  (const AliFmThreeVector<double>&) const;
    AliFmThreeVector<T> Cross(const AliFmThreeVector<double>&) const;
    AliFmThreeVector<T> PseudoProduct(const AliFmThreeVector<double>&) const;

    bool operator == (const AliFmThreeVector<float>& v) const;
    bool operator != (const AliFmThreeVector<float>& v) const;
    AliFmThreeVector<T>& operator+= (const AliFmThreeVector<float>&);
    AliFmThreeVector<T>& operator-= (const AliFmThreeVector<float>&);

    bool operator == (const AliFmThreeVector<double>& v) const;
    bool operator != (const AliFmThreeVector<double>& v) const;
    AliFmThreeVector<T>& operator+= (const AliFmThreeVector<double>&);
    AliFmThreeVector<T>& operator-= (const AliFmThreeVector<double>&);
#endif
  int             valid(double world = 1.e+5) const;
  int               bad(double world = 1.e+5) const;

protected:
    T    mX1, mX2, mX3;  // Three vector components

#ifdef __ROOT__
  /// \cond CLASSIMP
  ClassDef(AliFmThreeVector, 3);
  /// \endcond
#endif
};

#ifndef __CINT__
//
//        Implementation of member functions
//
template<class T>
inline AliFmThreeVector<T>::AliFmThreeVector(T x, T y, T z)
    : mX1(x), mX2(y), mX3(z) {/* nop */}
template<class T>
inline AliFmThreeVector<T>::~AliFmThreeVector() {/* nop */}

template<class T>
inline void AliFmThreeVector<T>::SetX(T x) {mX1 = x;}

template<class T>
inline void AliFmThreeVector<T>::SetY(T y) {mX2 = y;}

template<class T>
inline void AliFmThreeVector<T>::SetZ(T z) {mX3 = z;}

template<class T>
void AliFmThreeVector<T>::SetPhi(T angle)
{
    double  r = Magnitude();
    double th = Theta();

    mX1 = r*sin(th)*cos(angle);
    mX2 = r*sin(th)*sin(angle);
}

template <class T>
void AliFmThreeVector<T>::SetTheta(T angle)
{
    double r  = Magnitude();
    double ph = Phi();

    mX1 = r*sin(angle)*cos(ph);
    mX2 = r*sin(angle)*sin(ph);
    mX3 = r*cos(angle);
}

template <class T>
void AliFmThreeVector<T>::SetMagnitude(T r)
{
    double th = Theta();
    double ph = Phi();

    mX1 = r*sin(th)*cos(ph);
    mX2 = r*sin(th)*sin(ph);
    mX3 = r*cos(th);
}

template <class T>
void AliFmThreeVector<T>::SetMag(T mag)
{
    SetMagnitude(mag);
}

template<class T>
inline T AliFmThreeVector<T>::x() const {return mX1;}

template<class T>
inline T AliFmThreeVector<T>::y() const {return mX2;}

template<class T>
inline T AliFmThreeVector<T>::z() const {return mX3;}

template<class T>
inline T AliFmThreeVector<T>::Theta() const
{
  return acos(CosTheta());
}

template<class T>
inline T AliFmThreeVector<T>::CosTheta() const
{
  return mX3/(Mag()+1e-20);
}

template<class T>
inline T AliFmThreeVector<T>::Phi() const
{
    return atan2(mX2,mX1);
}

template<class T>
inline T AliFmThreeVector<T>::PseudoRapidity() const
{
    //
    // change code to more optimal:
    // double m = Mag();
    // return 0.5*::log( (m+z())/(m-z()) );
    double tmp = tan(Theta()/2.); if (tmp <=0.) return 1e20;
    return -::log(tmp);
}

template<class T>
inline AliFmThreeVector<T> AliFmThreeVector<T>::Unit() const
{
    double tmp = Mag(); if (tmp<=0.) tmp = 1e-20;
    return *this/tmp;
}

template <class T>
T AliFmThreeVector<T>::MassHypothesis(T mass) const
{
    return ::sqrt((*this)*(*this) + mass*mass);
}

template <class T>
AliFmThreeVector<T> AliFmThreeVector<T>::Orthogonal() const
{
    // Direct copy from CLHEP--it is probably better to
    // use your own dot/cross product code...
    double x = (mX1 < 0.0) ? -mX1 : mX1;
    double y = (mX2 < 0.0) ? -mX2 : mX2;
    double z = (mX3 < 0.0) ? -mX3 : mX3;

    if(x<y)
	return x < z ? AliFmThreeVector<T>(0,mX3,-mX2) :  AliFmThreeVector<T>(mX2,-mX1,0);
    else
	return  mX2 < mX3 ? AliFmThreeVector<T>(-mX3,0,mX1) :  AliFmThreeVector<T>(mX2,-mX1,0);
}

template <class T>
void AliFmThreeVector<T>::RotateX(T angle)
{
    // may in the future make use of the AliFmRotation class!
    double yPrime = cos(angle)*mX2 - sin(angle)*mX3;
    double zPrime = sin(angle)*mX2 + cos(angle)*mX3;

    mX2 = yPrime;
    mX3 = zPrime;
}

template <class T>
void AliFmThreeVector<T>::RotateY(T angle)
{
    // may in the future make use of the AliFmRotation class!
    double zPrime = cos(angle)*mX3 - sin(angle)*mX1;
    double xPrime = sin(angle)*mX3 + cos(angle)*mX1;

    mX1 = xPrime;
    mX3 = zPrime;
}

template <class T>
void AliFmThreeVector<T>::RotateZ(T angle)
{
    // may in the future make use of the AliFmRotation class!
    double xPrime = cos(angle)*mX1 - sin(angle)*mX2;
    double yPrime = sin(angle)*mX1 + cos(angle)*mX2;

    mX1 = xPrime;
    mX2 = yPrime;
}

template<class T>
inline T AliFmThreeVector<T>::Perp() const
{
    return ::sqrt(mX1*mX1+mX2*mX2);
}

template<class T>
inline T AliFmThreeVector<T>::Perp2() const
{
    return mX1*mX1+mX2*mX2;
}

template<class T>
inline T AliFmThreeVector<T>::Magnitude() const
{
    return Mag();
}

template<class T>
inline T AliFmThreeVector<T>::Mag() const
{
    return ::sqrt(mX1*mX1+mX2*mX2+mX3*mX3);
}

template<class T>
inline T AliFmThreeVector<T>::Mag2() const
{
    return mX1*mX1+mX2*mX2+mX3*mX3;
}

template<class T>
inline T AliFmThreeVector<T>::operator() (size_t i) const
{
    if (0 <=i && i <= 2)  return (&mX1)[i];
#ifndef ST_NO_EXCEPTIONS
    throw out_of_range("AliFmThreeVector<T>::operator(): bad index");
#else
    cerr << "AliFmThreeVector<T>::operator(): bad index" << endl;
#endif
    return 0;
}

template<class T>
inline T& AliFmThreeVector<T>::operator() (size_t i)
{
    if (0 <=i && i <= 2)  return (&mX1)[i];
#ifndef ST_NO_EXCEPTIONS
    throw out_of_range("AliFmThreeVector<T>::operator(): bad index");
#else
    cerr << "AliFmThreeVector<T>::operator(): bad index" << endl;
#endif
    return mX1;
}

template<class T>
inline T AliFmThreeVector<T>::operator[] (size_t i) const
{
    if (0 <=i && i <= 2)  return (&mX1)[i];
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("AliFmThreeVector<T>::operator[]: bad index");
#else
      cerr << "AliFmThreeVector<T>::operator[]: bad index" << endl;
#endif
      return 0;
}

template<class T>
inline T &AliFmThreeVector<T>::operator[] (size_t i)
{
    if (0 <=i && i <= 2)  return (&mX1)[i];
#ifndef ST_NO_EXCEPTIONS
      throw out_of_range("AliFmThreeVector<T>::operator[]: bad index");
#else
      cerr << "AliFmThreeVector<T>::operator[]: bad index" << endl;
#endif
      return mX1;
}

template<class T>
inline AliFmThreeVector<T>& AliFmThreeVector<T>::operator*= (double c)
{
    mX1 *= c; mX2 *= c; mX3 *= c;
    return *this;
}

template<class T>
inline AliFmThreeVector<T>& AliFmThreeVector<T>::operator/= (double c)
{
    mX1 /= c; mX2 /= c; mX3 /= c;
    return *this;
}

template<class T>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::PseudoProduct(double x,double y,double z) const
{
    return AliFmThreeVector<T>(mX1*x,mX2*y,mX3*z);
}

template<class T>
AliFmThreeVector<T> AliFmThreeVector<T>::operator- ()
{
    return AliFmThreeVector<T>(-mX1, -mX2, -mX3);
}

template<class T>
AliFmThreeVector<T> AliFmThreeVector<T>::operator+ ()
{
    return *this;
}

#ifndef ST_NO_MEMBER_TEMPLATES
#ifndef WIN32

template<class T>
template<class X>
inline AliFmThreeVector<T>::AliFmThreeVector(const AliFmThreeVector<X>& v)
    : mX1(v.x()), mX2(v.y()), mX3(v.z()) {/* nop */}

template<class T>
template<class X>
inline AliFmThreeVector<T>::AliFmThreeVector(const X *a)
{
    mX1 = a[0];
    mX2 = a[1];
    mX3 = a[2];
}

template<class T>
template<class X>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator=(const AliFmThreeVector<X>& v)
{
  if(this != &v) {
    mX1 = v.x();  mX2 = v.y();  mX3 = v.z();
  }
  return *this;
}

template<class T>
template<class X>
inline bool AliFmThreeVector<T>::operator== (const AliFmThreeVector<X>& v) const
{
    return mX1 == v.x() && mX2 == v.y() && mX3 == v.z();
}

template<class T>
template<class X>
inline bool AliFmThreeVector<T>::operator!= (const AliFmThreeVector<X>& v) const
{
    return !(*this == v);
}

template<class T>
template<class X>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator+= (const AliFmThreeVector<X>& v)
{
    mX1 += v.x(); mX2 += v.y(); mX3 += v.z();
    return *this;
}

template<class T>
template<class X>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator-= (const AliFmThreeVector<X>& v)
{
    mX1 -= v.x(); mX2 -= v.y(); mX3 -= v.z();
    return *this;
}

template<class T>
template<class X>
inline T AliFmThreeVector<T>::Dot(const AliFmThreeVector<X>& v) const
{
    return mX1*v.x() + mX2*v.y() + mX3*v.z();
}

template<class T>
template<class X>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::Cross(const AliFmThreeVector<X>& v) const
{
    return AliFmThreeVector<T>(mX2*v.z() - mX3*v.y(),
			    mX3*v.x() - mX1*v.z(),
			    mX1*v.y() - mX2*v.x());
}

template<class T>
template<class X>
inline T AliFmThreeVector<T>::Angle(const AliFmThreeVector<X>& vec) const
{
    double norm = this->Mag2()*vec.Mag2();

    return norm > 0 ? acos(this->Dot(vec)/(::sqrt(norm))) : 0;
}

template<class T>
template<class X>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::PseudoProduct(const AliFmThreeVector<X>& v) const
{
    return this->PseudoProduct(v.x(),v.y(),v.z());
}

#endif
#else

template<class T>
inline AliFmThreeVector<T>::AliFmThreeVector(const AliFmThreeVector<float>& v)
    : mX1(v.x()), mX2(v.y()), mX3(v.z()) {/* nop */}

template<class T>
inline AliFmThreeVector<T>::AliFmThreeVector(const AliFmThreeVector<double>& v)
    : mX1(v.x()), mX2(v.y()), mX3(v.z()) {/* nop */}

template<class T>
inline AliFmThreeVector<T>::AliFmThreeVector(const float *a)
{
    mX1 = a[0];
    mX2 = a[1];
    mX3 = a[2];
}

template<class T>
inline AliFmThreeVector<T>::AliFmThreeVector(const double *a)
{
    mX1 = a[0];
    mX2 = a[1];
    mX3 = a[2];
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator=(const AliFmThreeVector<float>& v)
{
    mX1 = v.x();  mX2 = v.y();  mX3 = v.z();
    return *this;
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator=(const AliFmThreeVector<double>& v)
{
    mX1 = v.x();  mX2 = v.y();  mX3 = v.z();
    return *this;
}

template<class T>
inline bool
AliFmThreeVector<T>::operator== (const AliFmThreeVector<float>& v) const
{
    return mX1 == v.x() && mX2 == v.y() && mX3 == v.z();
}

template<class T>
inline bool
AliFmThreeVector<T>::operator== (const AliFmThreeVector<double>& v) const
{
    return mX1 == v.x() && mX2 == v.y() && mX3 == v.z();
}

template<class T>
inline bool
AliFmThreeVector<T>::operator!= (const AliFmThreeVector<float>& v) const
{
    return !(*this == v);
}

template<class T>
inline bool
AliFmThreeVector<T>::operator!= (const AliFmThreeVector<double>& v) const
{
    return !(*this == v);
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator+= (const AliFmThreeVector<float>& v)
{
    mX1 += v.x(); mX2 += v.y(); mX3 += v.z();
    return *this;
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator+= (const AliFmThreeVector<double>& v)
{
    mX1 += v.x(); mX2 += v.y(); mX3 += v.z();
    return *this;
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator-= (const AliFmThreeVector<float>& v)
{
    mX1 -= v.x(); mX2 -= v.y(); mX3 -= v.z();
    return *this;
}

template<class T>
inline AliFmThreeVector<T>&
AliFmThreeVector<T>::operator-= (const AliFmThreeVector<double>& v)
{
    mX1 -= v.x(); mX2 -= v.y(); mX3 -= v.z();
    return *this;
}

template<class T>
inline T AliFmThreeVector<T>::Dot(const AliFmThreeVector<float>& v) const
{
    return mX1*v.x() + mX2*v.y() + mX3*v.z();
}

template<class T>
inline T AliFmThreeVector<T>::Dot(const AliFmThreeVector<double>& v) const
{
    return mX1*v.x() + mX2*v.y() + mX3*v.z();
}

template<class T>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::Cross(const AliFmThreeVector<float>& v) const
{
    return AliFmThreeVector<T>(mX2*v.z() - mX3*v.y(),
			    mX3*v.x() - mX1*v.z(),
			    mX1*v.y() - mX2*v.x());
}

template<class T>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::Cross(const AliFmThreeVector<double>& v) const
{
    return AliFmThreeVector<T>(mX2*v.z() - mX3*v.y(),
			    mX3*v.x() - mX1*v.z(),
			    mX1*v.y() - mX2*v.x());
}

template<class T>
inline T AliFmThreeVector<T>::Angle(const AliFmThreeVector<float>& v) const
{
    double tmp = Mag()*v.Mag(); if (tmp <=0) tmp = 1e-20;
    return acos(this->Dot(v)/tmp);
}

template<class T>
inline T AliFmThreeVector<T>::Angle(const AliFmThreeVector<double>& v) const
{
    double tmp = Mag()*v.Mag(); if (tmp <=0) tmp = 1e-20;
    return acos(this->Dot(v)/tmp);
}

template<class T>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::PseudoProduct(const AliFmThreeVector<float>& v) const
{
    return this->PseudoProduct(v.x(),v.y(),v.z());
}

template<class T>
inline AliFmThreeVector<T>
AliFmThreeVector<T>::PseudoProduct(const AliFmThreeVector<double>& v) const
{
    return this->PseudoProduct(v.x(),v.y(),v.z());
}
#endif  // ST_NO_MEMBER_TEMPLATES
template<class T>
inline int
AliFmThreeVector<T>::valid(double world) const  {return !bad(world);}

template<class T>
inline int
AliFmThreeVector<T>::bad(double world) const
{
  for (int i=0;i<3;i++) {
	  if (!finite((&mX1)[i])      ) return 10+i;
	  if ( fabs  ((&mX1)[i])>world) return 20+i;
  }
  return 0;
}
#endif /*! __CINT__ */
#ifdef __CINT__
template<> float abs(const AliFmThreeVector<float>& v);
template<> double abs(const AliFmThreeVector<double>& v);
template<> AliFmThreeVector<double> cross_product(const AliFmThreeVector<double>& v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  cross_product(const AliFmThreeVector<float>&  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> cross_product(const AliFmThreeVector<float>&  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> cross_product(const AliFmThreeVector<double>& v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator+ (const AliFmThreeVector<double>& v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  operator+ (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator+ (const AliFmThreeVector<double>& v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator+ (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> operator- (const AliFmThreeVector<double>& v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  operator- (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator- (const AliFmThreeVector<double>& v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator- (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> operator* (const AliFmThreeVector<double>& v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  operator* (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator* (const AliFmThreeVector<double>& v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator* (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> operator* (const double                 v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator* (const AliFmThreeVector<float>&  v1, const double v2);
template<> AliFmThreeVector<double> operator* (const double                 v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> operator* (const AliFmThreeVector<double>& v1, const double v2);
template<> AliFmThreeVector<double> operator/ (const AliFmThreeVector<double>& v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  operator/ (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator/ (const AliFmThreeVector<double>& v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator/ (const AliFmThreeVector<float>&  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<double> operator/ (const                double  v1, const AliFmThreeVector<double>& v2);
template<> AliFmThreeVector<float>  operator/ (const                double  v1, const AliFmThreeVector<float>& v2);
template<> AliFmThreeVector<double> operator/ (const AliFmThreeVector<double>& v1, const double v2);
template<> AliFmThreeVector<double> operator/ (const AliFmThreeVector<float>&  v1, const double v2);
template<> istream&  operator>>(istream& is,const AliFmThreeVector<double>& v);
template<> istream&  operator>>(istream& is,const AliFmThreeVector<float>& v);
template<> ostream&  operator<<(ostream& os,const AliFmThreeVector<double>& v);
template<> ostream&  operator<<(ostream& os,const AliFmThreeVector<float>& v);
#else
//
//        Non-member functions
//
template<class T>
inline T abs(const AliFmThreeVector<T>& v) {return v.Mag();}

template<class T, class X>
inline AliFmThreeVector<T>
cross_product(const AliFmThreeVector<T>& v1, const AliFmThreeVector<X>& v2)
{
    return v1.Cross(v2);
}


//
//        Non-member operators
//
template<class T, class X>
inline AliFmThreeVector<T>
operator+ (const AliFmThreeVector<T>& v1, const AliFmThreeVector<X>& v2)
{
    return AliFmThreeVector<T>(v1) += v2;
}

template<class T, class X>
inline AliFmThreeVector<T>
operator- (const AliFmThreeVector<T>& v1, const AliFmThreeVector<X>& v2)
{
    return AliFmThreeVector<T>(v1) -= v2;
}

template<class T, class X>
inline T operator* (const AliFmThreeVector<T>& v1, const AliFmThreeVector<X>& v2)
{
    return AliFmThreeVector<T>(v1).Dot(v2);
}

template<class T>
inline AliFmThreeVector<T> operator* (const AliFmThreeVector<T>& v, double c)
{
    return AliFmThreeVector<T>(v) *= c;
}

template<class T>
inline AliFmThreeVector<T> operator* (double c, const AliFmThreeVector<T>& v)
{
    return AliFmThreeVector<T>(v) *= c;
}

template<class T, class X>
inline AliFmThreeVector<T> operator/ (const AliFmThreeVector<T>& v, X c)
{
    return AliFmThreeVector<T>(v) /= c;
}

template<class T>
ostream&  operator<<(ostream& os, const AliFmThreeVector<T>& v)
{
    return os << v.x() << '\t' << v.y() << '\t' << v.z();
}

template<class T>
istream&  operator>>(istream& is, AliFmThreeVector<T>& v)
{
    T  x, y, z;
    is >> x >> y >> z;
    v.SetX(x);
    v.SetY(y);
    v.SetZ(z);
    return is;
}
#endif /* ! __CINT__ */
#endif
