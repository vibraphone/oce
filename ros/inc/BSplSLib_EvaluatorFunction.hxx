// File:	BSplSLib_EvaluatorFunction.hxx
// Created:	Mon May 12 10:55:26 1997
// Author:	Xavier BENVENISTE
//		<xab@zozox.paris1.matra-dtv.fr>


#ifndef _BSplSLib_EvaluatorFunction_HeaderFile
#define _BSplSLib_EvaluatorFunction_HeaderFile


#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_PrimitiveTypes_HeaderFile
#include <Standard_PrimitiveTypes.hxx>
#endif

// Roman Lygin Oct 30, 2009: the C function pointer converted to a virtual class
// in order to get rid of usage of static functions and static data
// (modifications fully similar to AdvApprox_EvaluatorFunction introduced by
// Andrey Betenev)
// see also BSplCLib_EvaluatorFunction, AdvApp2Var_EvaluatorFunc2Var, etc

class BSplSLib_EvaluatorFunction 
{
 public:
  
  //! Empty constructor
  BSplSLib_EvaluatorFunction () {}
  
  //! Destructor should be declared as virtual
  virtual ~BSplSLib_EvaluatorFunction () {}
  
  //! Function evaluation method to be defined by descendant
  virtual void Evaluate (const Standard_Integer DerivativeRequest,
                         const Standard_Real    UParameter,
                         const Standard_Real    VParameter,
                         Standard_Real&         Result,
                         Standard_Integer&      ErrorCode) = 0;

  //! Shortcut for function-call style usage
  void operator () (const Standard_Integer DerivativeRequest,
                    const Standard_Real    UParameter,
                    const Standard_Real    VParameter,
                    Standard_Real&         Result,
                    Standard_Integer&      ErrorCode)
  { Evaluate (DerivativeRequest, UParameter, VParameter, Result, ErrorCode); }
  
 private:

  //! Copy constructor is declared private to forbid copying
  BSplSLib_EvaluatorFunction (const BSplSLib_EvaluatorFunction&) {}

  //! Assignment operator is declared private to forbid copying
  void operator = (const BSplSLib_EvaluatorFunction&) {}
};

#endif
