// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _TColgp_HArray2OfXY_HeaderFile
#define _TColgp_HArray2OfXY_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineHandle.hxx>
#include <Handle_TColgp_HArray2OfXY.hxx>

#include <TColgp_Array2OfXY.hxx>
#include <MMgt_TShared.hxx>
#include <Standard_Integer.hxx>
class Standard_RangeError;
class Standard_OutOfRange;
class Standard_OutOfMemory;
class Standard_DimensionMismatch;
class gp_XY;
class TColgp_Array2OfXY;



class TColgp_HArray2OfXY : public MMgt_TShared
{

public:

  
    TColgp_HArray2OfXY(const Standard_Integer R1, const Standard_Integer R2, const Standard_Integer C1, const Standard_Integer C2);
  
    TColgp_HArray2OfXY(const Standard_Integer R1, const Standard_Integer R2, const Standard_Integer C1, const Standard_Integer C2, const gp_XY& V);
  
      void Init (const gp_XY& V) ;
  
      Standard_Integer ColLength()  const;
  
      Standard_Integer RowLength()  const;
  
      Standard_Integer LowerCol()  const;
  
      Standard_Integer LowerRow()  const;
  
      Standard_Integer UpperCol()  const;
  
      Standard_Integer UpperRow()  const;
  
      void SetValue (const Standard_Integer Row, const Standard_Integer Col, const gp_XY& Value) ;
  
     const  gp_XY& Value (const Standard_Integer Row, const Standard_Integer Col)  const;
  
      gp_XY& ChangeValue (const Standard_Integer Row, const Standard_Integer Col) ;
  
     const  TColgp_Array2OfXY& Array2()  const;
  
      TColgp_Array2OfXY& ChangeArray2() ;




  DEFINE_STANDARD_RTTI(TColgp_HArray2OfXY)

protected:




private: 


  TColgp_Array2OfXY myArray;


};

#define ItemHArray2 gp_XY
#define ItemHArray2_hxx <gp_XY.hxx>
#define TheArray2 TColgp_Array2OfXY
#define TheArray2_hxx <TColgp_Array2OfXY.hxx>
#define TCollection_HArray2 TColgp_HArray2OfXY
#define TCollection_HArray2_hxx <TColgp_HArray2OfXY.hxx>
#define Handle_TCollection_HArray2 Handle_TColgp_HArray2OfXY
#define TCollection_HArray2_Type_() TColgp_HArray2OfXY_Type_()

#include <TCollection_HArray2.lxx>

#undef ItemHArray2
#undef ItemHArray2_hxx
#undef TheArray2
#undef TheArray2_hxx
#undef TCollection_HArray2
#undef TCollection_HArray2_hxx
#undef Handle_TCollection_HArray2
#undef TCollection_HArray2_Type_




#endif // _TColgp_HArray2OfXY_HeaderFile
