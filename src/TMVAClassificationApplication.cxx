/**
 *  @file   AnalysisScripts/src/TMVAClassificationApplication.cxx
 *
 *  @brief  Implementation of the TMVA classifier class.
 *
 *  $Log: $
 */

#include "TMVAClassificationApplication.h"

//=====================================================================

int main(int argc, char **argv)
{
    TMVAClassificationApplication *pTMVAClassificationApplication = new TMVAClassificationApplication();

}

//=====================================================================

TMVAClassificationApplication::TMVAClassificationApplication()
{
    std::string inputFileName("/usera/jjd49/pandora_direction/TMVA/roots/input_roots/data_tiny.root");
    TFile* data = new TFile(inputFileName.c_str());
    TTree* dataTree = (TTree*)(data->Get("EventSelection"));

    TFile *target = new TFile("data_tiny.root_bdtresponse.root", "RECREATE" );
    TTree *tree = new TTree("EventSelection", "Eventselection Tree");

    //copy all variables in input file to output file
    Int_t intValues[200], intCounter(0), floatCounter(0);
    Float_t floatValues[200];

    auto pBranchList(dataTree->GetListOfBranches());

    for (int i = 0; i < pBranchList->GetEntries(); ++i)
    {
        TBranch* pBranch(static_cast<TBranch*>(pBranchList->At(i)));
        std::string branchName(pBranch->GetName());
        //std::cout << "Branch name: " << pBranch->GetName() << std::endl; 

        TLeaf *pFirstLeaf= (TLeaf*) pBranch->GetListOfLeaves()->At(0);
        std::string leafName(pFirstLeaf->GetTypeName());

        if (leafName == "Int_t")
        {
            dataTree->SetBranchAddress(branchName.c_str(), &intValues[intCounter]); 
            tree->Branch(branchName.c_str(), &intValues[intCounter]); 
            ++intCounter;
        }
        else if (leafName == "Float_t")
        {
            dataTree->SetBranchAddress(branchName.c_str(), &floatValues[floatCounter]); 
            tree->Branch(branchName.c_str(), &floatValues[floatCounter]); 
            ++floatCounter;
        }
        else
            std::cout << "Seriously worrying error: unknown typename" << std::endl;
    }

    TMVA::Tools::Instance();

    //---------------------------------------------------

    std::vector<BDT> bdtVector;

    std::vector<std::string> allVariableNames = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", "LongestPfoAzimuthalAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMinY", "LongestPfoCorrectedMaxY", "LongestPfoCorrectedMinZ", "LongestPfoCorrectedMaxZ", 
    "LongestPfoUpDownDeltaChiSquaredPerHit", "LongestPfoDeltaChiSquaredPerHit", "LongestPfoMinChiSquaredPerHit", "LongestPfoFitChargeRangeOverLength", 
    "LongestPfoCosmicProbability", "LongestPfoMinSumQW", "LongestPfoMaxSumQW", "LongestPfoSumQWRatio", 
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge", 
    "LongestPfoMeanHitCharge"}; 

    std::vector<std::string> noDirectionNames = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", "LongestPfoAzimuthalAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMinY", "LongestPfoCorrectedMaxY", "LongestPfoCorrectedMinZ", "LongestPfoCorrectedMaxZ", 
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge", 
    "LongestPfoMeanHitCharge"}; 

    std::vector<std::string> eventSelectionVariableNamesVector = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", "LongestPfoAzimuthalAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMinY", "LongestPfoCorrectedMaxY", "LongestPfoCorrectedMinZ", "LongestPfoCorrectedMaxZ", 
    "ShortestPfoCharge", "ShortestPfoLength", "ShortestPfoTrackProbability", "ShortestPfoPolarAngle", "ShortestPfoAzimuthalAngle", 
    "ShortestPfoCorrectedMinX", "ShortestPfoCorrectedMaxX", "ShortestPfoCorrectedMinY", "ShortestPfoCorrectedMaxY", "ShortestPfoCorrectedMinZ", "ShortestPfoCorrectedMaxZ", 
    "LongestPfoUpDownDeltaChiSquaredPerHit", "LongestPfoDeltaChiSquaredPerHit", "LongestPfoMinChiSquaredPerHit", "LongestPfoFitChargeRangeOverLength", 
    "LongestPfoCosmicProbability", "LongestPfoMinSumQW", "LongestPfoMaxSumQW", "LongestPfoSumQWRatio", 
    "ShortestPfoUpDownDeltaChiSquaredPerHit", "ShortestPfoDeltaChiSquaredPerHit", "ShortestPfoMinChiSquaredPerHit", "ShortestPfoFitChargeRangeOverLength", 
    "ShortestPfoCosmicProbability", "ShortestPfoMinSumQW", "ShortestPfoMaxSumQW", "ShortestPfoSumQWRatio", 
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge", 
    "ShortestPfoRecoDeltaY", "ShortestPfoRecoLowestTenCmTotalCharge", 
    "LongestPfoMeanHitCharge", 
    "ShortestPfoMeanHitCharge"}; 

    std::vector<std::string> N2_PID_precut_variable_names = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMaxY", 
    "ShortestPfoCharge", "ShortestPfoLength", "ShortestPfoTrackProbability", "ShortestPfoPolarAngle",
    "LongestPfoUpDownDeltaChiSquaredPerHit", "LongestPfoDeltaChiSquaredPerHit", "LongestPfoMinChiSquaredPerHit", "LongestPfoFitChargeRangeOverLength", "LongestPfoMinSumQW", 
    "ShortestPfoUpDownDeltaChiSquaredPerHit", "ShortestPfoDeltaChiSquaredPerHit", "ShortestPfoMinChiSquaredPerHit", "ShortestPfoFitChargeRangeOverLength", "ShortestPfoMinSumQW",
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge", 
    "ShortestPfoMinChiSquaredPerHit3D"}; 

    std::vector<std::string> N2_Contained_CleanedVariables_variable_names = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMaxY", 
    "ShortestPfoCharge", "ShortestPfoLength", "ShortestPfoTrackProbability", "ShortestPfoPolarAngle",
    "LongestPfoUpDownDeltaChiSquaredPerHit", "LongestPfoDeltaChiSquaredPerHit", "LongestPfoMinChiSquaredPerHit", "LongestPfoFitChargeRangeOverLength", "LongestPfoMinSumQW", 
    "ShortestPfoUpDownDeltaChiSquaredPerHit", "ShortestPfoDeltaChiSquaredPerHit", "ShortestPfoMinChiSquaredPerHit", "ShortestPfoFitChargeRangeOverLength", "ShortestPfoMinSumQW",
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge"}; 

    std::vector<std::string> N1_Contained_CleanedVariables_variable_names = {"NeutrinoMomentumX", "NeutrinoMomentumY", "NeutrinoMomentumZ", "OpeningAngle", 
    "LongestPfoCharge", "LongestPfoLength", "LongestPfoTrackProbability", "LongestPfoPolarAngle", 
    "LongestPfoCorrectedMinX", "LongestPfoCorrectedMaxX", "LongestPfoCorrectedMaxY", 
    "LongestPfoUpDownDeltaChiSquaredPerHit", "LongestPfoDeltaChiSquaredPerHit", "LongestPfoMinChiSquaredPerHit", "LongestPfoFitChargeRangeOverLength", "LongestPfoMinSumQW", 
    "LongestPfoRecoDeltaY", "LongestPfoRecoLowestTenCmTotalCharge"}; 

    //BDT N1_AllVariablesBDT(inputFileName, "EventSelection", "directional_impact/bdts/TMVA_BDT_AllVariables_N1_Contained.xml", allVariableNames);
    //BDT N1_NoDirectionBDT(inputFileName, "EventSelection", "directional_impact/bdts/TMVA_BDT_NoDirection_N1_Contained.xml", noDirectionNames);
    //BDT N2_AllVariables_BDT(inputFileName, "EventSelection", "directional_impact/bdts/TMVA_BDT_AllVariables_N2_Contained.xml", eventSelectionVariableNamesVector);
    //BDT N2_PID_precut_BDT(inputFileName, "EventSelection", "BDTs/BDT_PID_precut_N2.xml", N2_PID_precut_variable_names);
    BDT N1_Contained_CleanedVariables_BDT(inputFileName, "EventSelection", "BDTs/BDT_N1_Contained_CleanedVariables.xml", N1_Contained_CleanedVariables_variable_names);
    BDT N2_Contained_CleanedVariables_BDT(inputFileName, "EventSelection", "BDTs/BDT_N2_Contained_CleanedVariables.xml", N2_Contained_CleanedVariables_variable_names);

    //bdtVector.push_back(std::move(N1_AllVariablesBDT));
    //bdtVector.push_back(std::move(N1_NoDirectionBDT));
    //bdtVector.push_back(std::move(N2_AllVariables_BDT));
    //bdtVector.push_back(std::move(N2_PID_precut_BDT));
    bdtVector.push_back(std::move(N1_Contained_CleanedVariables_BDT));
    bdtVector.push_back(std::move(N2_Contained_CleanedVariables_BDT));

    std::vector<float> bdtResponses(bdtVector.size(), 0.f);

    int bdtCounter(0);

    for (BDT &bdt : bdtVector)
    {
        std::cout << bdt.GetName() << std::endl;
        tree->Branch(bdt.GetName().c_str(), &bdtResponses.at(bdtCounter));
        ++bdtCounter;
    }

    for (int ievt = 0; ievt < dataTree->GetEntries(); ievt++) 
    {
        dataTree->GetEntry(ievt);

        if (ievt % 1000 == 0)
            std::cout << "--- ... Processing event: " << ievt << std::flush << '\xd'; 

        int bdtCounter(0);

        for (BDT &bdt : bdtVector)
        {
            bdt.Update(ievt);

            for (unsigned int i = 0; i < bdt.m_variablenames.size(); ++i)
                bdt.m_eventvariables.at(i) = bdt.m_uservariables.at(i);

            bdtResponses.at(bdtCounter) = bdt.GetResponse();

            ++bdtCounter;
        }

        tree->Fill();
    }

    std::cout << std::endl;
      
    target->Write();
    target->Close();
}

//=====================================================================
