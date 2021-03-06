// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Adaptor3d_IsoCurve_HeaderFile
#define _Adaptor3d_IsoCurve_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Handle_Adaptor3d_HSurface.hxx>
#include <GeomAbs_IsoType.hxx>
#include <Standard_Real.hxx>
#include <Adaptor3d_Curve.hxx>
#include <GeomAbs_Shape.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Adaptor3d_HCurve.hxx>
#include <Standard_Boolean.hxx>
#include <GeomAbs_CurveType.hxx>
#include <Handle_Geom_BezierCurve.hxx>
#include <Handle_Geom_BSplineCurve.hxx>
class Adaptor3d_HSurface;
class Standard_NoSuchObject;
class Standard_OutOfRange;
class Standard_DomainError;
class TColStd_Array1OfReal;
class Adaptor3d_HCurve;
class gp_Pnt;
class gp_Vec;
class gp_Lin;
class gp_Circ;
class gp_Elips;
class gp_Hypr;
class gp_Parab;
class Geom_BezierCurve;
class Geom_BSplineCurve;


//! Defines an isoparametric curve on  a surface.  The
//! type  of isoparametric curve  (U  or V) is defined
//! with the   enumeration  IsoType from   GeomAbs  if
//! NoneIso is given an error is raised.
class Adaptor3d_IsoCurve  : public Adaptor3d_Curve
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! The iso is set to NoneIso.
  Standard_EXPORT Adaptor3d_IsoCurve();
  
  //! The surface is loaded. The iso is set to NoneIso.
  Standard_EXPORT Adaptor3d_IsoCurve(const Handle(Adaptor3d_HSurface)& S);
  
  //! Creates  an  IsoCurve curve.   Iso  defines the
  //! type (isoU or  isoU) Param defines the value of
  //! the iso. The bounds  of  the iso are the bounds
  //! of the surface.
  Standard_EXPORT Adaptor3d_IsoCurve(const Handle(Adaptor3d_HSurface)& S, const GeomAbs_IsoType Iso, const Standard_Real Param);
  
  //! Create an IsoCurve curve.  Iso defines the type
  //! (isoU or isov).  Param defines the value of the
  //! iso. WFirst,WLast define the bounds of the iso.
  Standard_EXPORT Adaptor3d_IsoCurve(const Handle(Adaptor3d_HSurface)& S, const GeomAbs_IsoType Iso, const Standard_Real Param, const Standard_Real WFirst, const Standard_Real WLast);
  
  //! Changes  the surface.  The  iso  is  reset  to
  //! NoneIso.
  Standard_EXPORT   void Load (const Handle(Adaptor3d_HSurface)& S) ;
  
  //! Changes the iso on the current surface.
  Standard_EXPORT   void Load (const GeomAbs_IsoType Iso, const Standard_Real Param) ;
  
  //! Changes the iso on the current surface.
  Standard_EXPORT   void Load (const GeomAbs_IsoType Iso, const Standard_Real Param, const Standard_Real WFirst, const Standard_Real WLast) ;
  
     const  Handle(Adaptor3d_HSurface)& Surface()  const;
  
      GeomAbs_IsoType Iso()  const;
  
      Standard_Real Parameter()  const;
  
      Standard_Real FirstParameter()  const;
  
      Standard_Real LastParameter()  const;
  
  Standard_EXPORT   GeomAbs_Shape Continuity()  const;
  
  //! Returns  the number  of  intervals for  continuity
  //! <S>. May be one if Continuity(me) >= <S>
  Standard_EXPORT   Standard_Integer NbIntervals (const GeomAbs_Shape S) ;
  
  //! Stores in <T> the  parameters bounding the intervals
  //! of continuity <S>.
  //!
  //! The array must provide  enough room to  accomodate
  //! for the parameters. i.e. T.Length() > NbIntervals()
  Standard_EXPORT   void Intervals (TColStd_Array1OfReal& T, const GeomAbs_Shape S) ;
  
  //! Returns    a  curve equivalent   of  <me>  between
  //! parameters <First>  and <Last>. <Tol>  is used  to
  //! test for 3d points confusion.
  //! If <First> >= <Last>
  Standard_EXPORT   Handle(Adaptor3d_HCurve) Trim (const Standard_Real First, const Standard_Real Last, const Standard_Real Tol)  const;
  
  Standard_EXPORT   Standard_Boolean IsClosed()  const;
  
  Standard_EXPORT   Standard_Boolean IsPeriodic()  const;
  
  Standard_EXPORT   Standard_Real Period()  const;
  
  //! Computes the point of parameter U on the curve.
  Standard_EXPORT   gp_Pnt Value (const Standard_Real U)  const;
  
  //! Computes the point of parameter U on the curve.
  Standard_EXPORT   void D0 (const Standard_Real U, gp_Pnt& P)  const;
  
  //! Computes the point of parameter U on the curve with its
  //! first derivative.
  //! Raised if the continuity of the current interval
  //! is not C1.
  Standard_EXPORT   void D1 (const Standard_Real U, gp_Pnt& P, gp_Vec& V)  const;
  

  //! Returns the point P of parameter U, the first and second
  //! derivatives V1 and V2.
  //! Raised if the continuity of the current interval
  //! is not C2.
  Standard_EXPORT   void D2 (const Standard_Real U, gp_Pnt& P, gp_Vec& V1, gp_Vec& V2)  const;
  

  //! Returns the point P of parameter U, the first, the second
  //! and the third derivative.
  //! Raised if the continuity of the current interval
  //! is not C3.
  Standard_EXPORT   void D3 (const Standard_Real U, gp_Pnt& P, gp_Vec& V1, gp_Vec& V2, gp_Vec& V3)  const;
  

  //! The returned vector gives the value of the derivative for the
  //! order of derivation N.
  //! Raised if the continuity of the current interval
  //! is not CN.
  //! Raised if N < 1.
  Standard_EXPORT   gp_Vec DN (const Standard_Real U, const Standard_Integer N)  const;
  
  //! Returns the parametric  resolution corresponding
  //! to the real space resolution <R3d>.
  Standard_EXPORT   Standard_Real Resolution (const Standard_Real R3d)  const;
  
  //! Returns  the  type of the   curve  in the  current
  //! interval :   Line,   Circle,   Ellipse, Hyperbola,
  //! Parabola, BezierCurve, BSplineCurve, OtherCurve.
  Standard_EXPORT   GeomAbs_CurveType GetType()  const;
  
  Standard_EXPORT   gp_Lin Line()  const;
  
  Standard_EXPORT   gp_Circ Circle()  const;
  
  Standard_EXPORT   gp_Elips Ellipse()  const;
  
  Standard_EXPORT   gp_Hypr Hyperbola()  const;
  
  Standard_EXPORT   gp_Parab Parabola()  const;
  
  Standard_EXPORT   Standard_Integer Degree()  const;
  
  Standard_EXPORT   Standard_Boolean IsRational()  const;
  
  Standard_EXPORT   Standard_Integer NbPoles()  const;
  
  Standard_EXPORT   Standard_Integer NbKnots()  const;
  
  Standard_EXPORT   Handle(Geom_BezierCurve) Bezier()  const;
  
  Standard_EXPORT   Handle(Geom_BSplineCurve) BSpline()  const;




protected:





private:



  Handle(Adaptor3d_HSurface) mySurface;
  GeomAbs_IsoType myIso;
  Standard_Real myFirst;
  Standard_Real myLast;
  Standard_Real myParameter;


};


#include <Adaptor3d_IsoCurve.lxx>





#endif // _Adaptor3d_IsoCurve_HeaderFile
