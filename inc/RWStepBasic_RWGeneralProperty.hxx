// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _RWStepBasic_RWGeneralProperty_HeaderFile
#define _RWStepBasic_RWGeneralProperty_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Handle_StepData_StepReaderData.hxx>
#include <Standard_Integer.hxx>
#include <Handle_Interface_Check.hxx>
#include <Handle_StepBasic_GeneralProperty.hxx>
class StepData_StepReaderData;
class Interface_Check;
class StepBasic_GeneralProperty;
class StepData_StepWriter;
class Interface_EntityIterator;


//! Read & Write tool for GeneralProperty
class RWStepBasic_RWGeneralProperty 
{
public:

  DEFINE_STANDARD_ALLOC

  
  //! Empty constructor
  Standard_EXPORT RWStepBasic_RWGeneralProperty();
  
  //! Reads GeneralProperty
  Standard_EXPORT   void ReadStep (const Handle(StepData_StepReaderData)& data, const Standard_Integer num, Handle(Interface_Check)& ach, const Handle(StepBasic_GeneralProperty)& ent)  const;
  
  //! Writes GeneralProperty
  Standard_EXPORT   void WriteStep (StepData_StepWriter& SW, const Handle(StepBasic_GeneralProperty)& ent)  const;
  
  //! Fills data for graph (shared items)
  Standard_EXPORT   void Share (const Handle(StepBasic_GeneralProperty)& ent, Interface_EntityIterator& iter)  const;




protected:





private:





};







#endif // _RWStepBasic_RWGeneralProperty_HeaderFile
