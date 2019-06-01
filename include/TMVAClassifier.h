/**
 *  @file   AnalysisScripts/include/TMVAClassifier.h
 *
 *  @brief  Header file for the TMVA classifier class.
 *
 *  $Log: $
 */

#ifndef TMVA_CLASSIFIER_H
#define TMVA_CLASSIFIER_H

#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

class TMVAClassifier
{
    public:
        /**
         *  @brief Constructor
         */
        TMVAClassifier();

        /**
         *  @brief Default destructor
         */
        ~TMVAClassifier();

    private:
};

#endif
