/**
 *  @file   AnalysisScripts/include/TMVAClassificationApplication.h
 *
 *  @brief  Header file for the TMVA classifier class.
 *
 *  $Log: $
 */

#ifndef TMVA_CLASSIFICATION_APPLICATION_H
#define TMVA_CLASSIFICATION_APPLICATION_H

#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TLeaf.h"

#include "TMVA/Reader.h"
#include "TMVA/Tools.h"

class BDT
{
    public:

        BDT(std::string fileName, std::string treeName, std::string bdtName, std::vector<std::string> &variableNames)
        {
            TFile* m_data = new TFile(fileName.c_str());
            m_datatree = (TTree*)(m_data->Get(treeName.c_str()));
            m_reader = new TMVA::Reader( "V:Color:!Silent" );

            m_variablenames = variableNames;
            m_bdtname = bdtName;

            std::vector<float> emptyVector(m_variablenames.size(), 0.f);
            m_eventvariables = emptyVector;
            m_uservariables = emptyVector;
    
            FillVariables();

            m_reader->BookMVA("BDT", bdtName.c_str());

            FillUserVariables();
        }

        //=====================================================================

        void Update(int &eventNumber)
        {
            m_datatree->GetEntry(eventNumber);
        }

        //=====================================================================

        float GetResponse()
        {
            return m_reader->EvaluateMVA("BDT");
        }

        //=====================================================================

        std::string GetName()
        {
            std::stringstream stream(m_bdtname);
            std::string segment;
            std::vector<std::string> seglist;

            while(std::getline(stream, segment, '/'))
            {
               seglist.push_back(segment);
            }

            //this remove the file extension .xml
            std::string bdtName = seglist.back().substr(0, seglist.back().size() - 4);        

            return bdtName;
        }

        //=====================================================================

        std::vector<float>          m_eventvariables;
        std::vector<float>          m_uservariables;
        std::vector<std::string>    m_variablenames;

    private:

        void FillVariables()
        {
            int variableCounter(0);

            for (const auto &variableName : m_variablenames)
            {   
                m_reader->AddVariable(variableName.c_str(), &m_eventvariables[variableCounter]);
                ++variableCounter;
            }   
        }

        //=====================================================================

        void FillUserVariables()
        {
            int variableCounter = 0;

            for (const auto &variableName : m_variablenames)
            {   
                m_datatree->SetBranchAddress(variableName.c_str(), &m_uservariables[variableCounter]);
                ++variableCounter;
            }   
        }

        //=====================================================================

        std::string                 m_bdtname;
        TFile*                      m_file;
        TMVA::Reader*               m_reader;
        TTree*                      m_datatree;
};

class TMVAClassificationApplication
{
    typedef std::vector<int> IntVector;
    typedef std::vector<float> FloatVector;
    typedef std::vector<double> DoubleVector;

    public:
        /**
         *  @brief Constructor
         */
        TMVAClassificationApplication();

        void FillVariables(TMVA::Reader* reader, TTree* dataTree, const std::vector<std::string> &variableNamesVector, std::vector<float> &variables);
        void FillUserVariables(TMVA::Reader* reader, TTree* dataTree, const std::vector<std::string> &variableNamesVector, std::vector<float> &userVariables);

        /**
         *  @brief Default destructor
         */
        ~TMVAClassificationApplication();

    private:
        /**
         *  @brief Convert a number to a string
         *
         *  @param T number to convert to string
         */
        template <class T>
        std::string NumberToString(T Number);
};

#endif
