// File:	AdvApp2Var_EvaluatorFunc2Var.hxx
// Created:	Thu Apr 18 10:31:26 1996
// Author:	Joelle CHAUVET
//		<jct@sgi38>


#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Standard_PrimitiveTypes_HeaderFile
#include <Standard_PrimitiveTypes.hxx>
#endif


#ifndef AdvApp2Var_EvaluatorFunc2Var_HeaderFile
#define AdvApp2Var_EvaluatorFunc2Var_HeaderFile

// Roman Lygin Oct 30, 2009: the C function pointer converted to a virtual class
// in order to get rid of usage of static functions and static data
// (modifications fully similar to AdvApprox_EvaluatorFunction introduced by
// Andrey Betenev)
// see also BSplSLib_EvaluatorFunction, BSplCLib_EvaluatorFunction, etc

class AdvApp2Var_EvaluatorFunc2Var 
{
 public:
  
  //! Empty constructor
  AdvApp2Var_EvaluatorFunc2Var () {}
  
  //! Destructor should be declared as virtual
  virtual ~AdvApp2Var_EvaluatorFunc2Var () {}
  
  //! Function evaluation method to be defined by descendant
  virtual void Evaluate (Standard_Integer * Dimension,
					     Standard_Real    * UStartEnd,
					     Standard_Real    * VStartEnd,
					     Standard_Integer * FavorIso,// Choice of constante, 1 for U, 2 for V
					     Standard_Real    * ConstParam,// Value of constant parameter
				         Standard_Integer * NbParams, // Number of parameters N
				         Standard_Real    * Parameters, // Values of parameters,
				         Standard_Integer * UOrder, // Derivative Request in U
				         Standard_Integer * VOrder, // Derivative Request in V
				         Standard_Real    * Result, // Result[Dimension,N]
				         Standard_Integer * ErrorCode) = 0;

  //! Shortcut for function-call style usage
  void operator () (Standard_Integer * Dimension,
				    Standard_Real    * UStartEnd,
				    Standard_Real    * VStartEnd,
				    Standard_Integer * FavorIso,
				    Standard_Real    * ConstParam,
			        Standard_Integer * NbParams,
			        Standard_Real    * Parameters,
			        Standard_Integer * UOrder,
			        Standard_Integer * VOrder,
			        Standard_Real    * Result,
			        Standard_Integer * ErrorCode)
  { Evaluate (Dimension, UStartEnd, VStartEnd, FavorIso, ConstParam, NbParams,
      Parameters, UOrder, VOrder, Result, ErrorCode);
  }
  
 private:

  //! Copy constructor is declared private to forbid copying
  AdvApp2Var_EvaluatorFunc2Var (const AdvApp2Var_EvaluatorFunc2Var&) {}

  //! Assignment operator is declared private to forbid copying
  void operator = (const AdvApp2Var_EvaluatorFunc2Var&) {}
};

#endif
