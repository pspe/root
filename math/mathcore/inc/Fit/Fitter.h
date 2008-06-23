// @(#)root/mathcore:$Id$
// Author: L. Moneta Wed Aug 30 11:05:19 2006

/**********************************************************************
 *                                                                    *
 * Copyright (c) 2006  LCG ROOT Math Team, CERN/PH-SFT                *
 *                                                                    *
 *                                                                    *
 **********************************************************************/

// Header file for class Fitter

#ifndef ROOT_Fit_Fitter
#define ROOT_Fit_Fitter

/**
@defgroup Fit Fitting and Parameter Estimation

Classes used for fitting (regression analysis) and estimation of parameter values given a data sample. 
*/

#ifndef ROOT_Fit_DataVectorfwd
#include "Fit/DataVectorfwd.h"
#endif

#ifndef ROOT_Fit_FitConfig
#include "Fit/FitConfig.h"
#endif

#ifndef ROOT_Fit_FitResult
#include "Fit/FitResult.h"
#endif

#ifndef ROOT_Math_IParamFunctionfwd
#include "Math/IParamFunctionfwd.h"
#endif



namespace ROOT { 


   namespace Math { 
      class Minimizer;
   } 

   /**
      Namespace for the fitting classes
      @ingroup Fit
    */

   namespace Fit { 

/**
   @defgroup FitMain Main fitting classes
   @ingroup Fit
*/

//___________________________________________________________________________________
/** 
   Fitter class, entry point for performing all type of fits. 
   Fits are performed using the generic ROOT::Fit::Fitter::Fit method. 
   The inputs are the data points and a model function (using a ROOT::Math::IParamFunction)
   The result of the fit is returned and kept internally in the  ROOT::Fit::FitResult class. 
   The configuration of the fit (parameters, options, etc...) are specified in the 
   ROOT::Math::FitConfig class. 

   @ingroup FitMain
*/ 
class Fitter {

public: 

   typedef ROOT::Math::IParamMultiFunction       IModelFunction; 
   typedef ROOT::Math::IParamMultiGradFunction   IGradModelFunction;
   typedef ROOT::Math::IParamFunction            IModel1DFunction; 
   typedef ROOT::Math::IParamGradFunction        IGradModel1DFunction; 

   typedef ROOT::Math::IMultiGenFunction BaseFunc; 
   typedef ROOT::Math::IMultiGradFunction BaseGradFunc; 


   /** 
      Default constructor
   */ 
   Fitter (); 

   /** 
      Destructor
   */ 
   ~Fitter (); 


   

private:
   // usually copying is non trivial, so we make this unaccessible

   /** 
      Copy constructor
   */ 
   Fitter(const Fitter &);

   /** 
      Assignment operator
   */ 
   Fitter & operator = (const Fitter & rhs);  


public: 

   /** 
       fit a data set using any  generic model  function
       Pre-requisite on the function: 
   */ 
   template < class Data , class Function> 
   bool Fit( const Data & data, const Function & func) { 
      SetFunction(func);
      return Fit(data);
   }

   /** 
       fit a binned data set (default method: use chi2)
       To be implemented option to do likelihood bin fit
   */ 
   bool Fit(const BinData & data) { 
      return DoLeastSquareFit(data); 
   } 
   /** 
       fit an binned data set using loglikelihood method
   */ 
   bool Fit(const UnBinData & data) { 
      return DoLikelihoodFit(data); 
   } 

   /**
      Likelihood fit 
    */
   template <class Data> 
   bool LikelihoodFit(const Data & data) { 
      return DoLikelihoodFit(data);
   }

   /** 
       fit a data set using any  generic model  function
       Pre-requisite on the function: 
   */ 
   template < class Data , class Function> 
   bool LikelihoodFit( const Data & data, const Function & func) { 
      SetFunction(func);
      return DoLikelihoodFit(data);
   }

   /**
      fit using the given FCN function. Give also initial parameter values and data size to get  Ndf
    */
   bool FitFCN(const ROOT::Math::IMultiGenFunction & fcn, const double * params, unsigned int dataSize ); 
   /**
      fit using the given gradient FCN function. Give also initial parameter values and data size to get  Ndf
    */
   bool FitFCN(const ROOT::Math::IMultiGradFunction & fcn, const double * params, unsigned int dataSize ); 

   /**
      do a linear fit on a set of bin-data
    */
   bool LinearFit(const BinData & data) { return DoLinearFit(data); }

   /** 
       Set the fitted function (model function) from a parametric function interface
   */ 
   void  SetFunction(const IModelFunction & func); 
   /**
      Set the fitted function from a parametric 1D function interface
    */
   void  SetFunction(const IModel1DFunction & func); 

   /** 
       Set the fitted function (model function) from a parametric gradient function interface
   */ 
   void  SetFunction(const IGradModelFunction & func); 
   /**
      Set the fitted function from 1D gradient parametric function interface
    */
   void  SetFunction(const IGradModel1DFunction & func); 


   /**
      get fit result
   */
   const FitResult & Result() const { return fResult; } 

   /**
      access to the fit configuration (const method)
   */
   const FitConfig & Config() const { return fConfig; } 

   /**
      access to the configuration (non const method)
   */
   FitConfig & Config() { return fConfig; } 



protected: 

   /// least square fit 
   bool DoLeastSquareFit(const BinData & data); 
   /// binned likelihood fit
   bool DoLikelihoodFit(const BinData & data); 
   /// un-binned likelihood fit
   bool DoLikelihoodFit(const UnBinData & data); 
   /// linear least square fit 
   bool DoLinearFit(const BinData & data);

   /// do minimization
   template<class ObjFunc> 
   bool DoMinimization(ROOT::Math::Minimizer & min, const ObjFunc & f, unsigned int dataSize, const ROOT::Math::IMultiGenFunction * chifunc = 0); 

private: 

   bool fUseGradient;       // flag to indicate if using gradient or not

   IModelFunction * fFunc;  // copy of the fitted  function containing on output the fit result

   FitResult fResult;       // object containing the result of the fit

   FitConfig fConfig;       // fitter configuration (options and parameter settings)

}; 

   } // end namespace Fit

} // end namespace ROOT


#endif /* ROOT_Fit_Fitter */
