// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _Vrml_TextureCoordinate2_HeaderFile
#define _Vrml_TextureCoordinate2_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_Vrml_TextureCoordinate2.hxx>

#include <Handle_TColgp_HArray1OfVec2d.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_OStream.hxx>
class TColgp_HArray1OfVec2d;


//! defines a TextureCoordinate2 node of VRML specifying properties of geometry
//! and its appearance.
//! This  node  defines  a  set  of  2D  coordinates  to  be  used  to  map  textures
//! to  the  vertices  of  subsequent  PointSet,  IndexedLineSet,  or  IndexedFaceSet
//! objects.  It  replaces  the  current  texture  coordinates  in  the  rendering
//! state  for  the  shapes  to  use.
//! Texture  coordinates  range  from  0  to  1  across  the  texture.
//! The  horizontal  coordinate,  called  S,  is  specified  first,  followed
//! by  vertical  coordinate,  T.
//! By  default  :
//! myPoint (0 0)
class Vrml_TextureCoordinate2 : public MMgt_TShared
{

public:

  
  Standard_EXPORT Vrml_TextureCoordinate2();
  
  Standard_EXPORT Vrml_TextureCoordinate2(const Handle(TColgp_HArray1OfVec2d)& aPoint);
  
  Standard_EXPORT   void SetPoint (const Handle(TColgp_HArray1OfVec2d)& aPoint) ;
  
  Standard_EXPORT   Handle(TColgp_HArray1OfVec2d) Point()  const;
  
  Standard_EXPORT   Standard_OStream& Print (Standard_OStream& anOStream)  const;




  DEFINE_STANDARD_RTTI(Vrml_TextureCoordinate2)

protected:




private: 


  Handle(TColgp_HArray1OfVec2d) myPoint;


};







#endif // _Vrml_TextureCoordinate2_HeaderFile
