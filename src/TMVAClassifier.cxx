/**
 *  @file   TMVA/src/TMVAClassifier.cxx
 *
 *  @brief  Implementation of the TMVA classifier class.
 *
 *  $Log: $
 */

#include "TMVAClassifier.h"

//=====================================================================

int main(int argc, char **argv)
{
    TMVAClassifier *pTMVAClassifier = new TMVAClassifier();
}

//=====================================================================

TMVAClassifier::TMVAClassifier()
{
    TString outfileName( "TMVA_BDT_N1_Cleaned_Variables.root" );
    TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

    TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile, "!V:!Silent:Color:DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification");
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");

    dataloader->AddVariable( "NeutrinoMomentumX", "p_{#nu,x}", "", 'F');
    dataloader->AddVariable( "NeutrinoMomentumY", "p_{#nu,y}", "", 'F');
    dataloader->AddVariable( "NeutrinoMomentumZ", "p_{#nu,z}", "", 'F');
    dataloader->AddVariable( "OpeningAngle", "#zeta", "", 'F');

    //Topological Variables
    dataloader->AddVariable( "LongestPfoCharge", "Q_{PFO, l}", "", 'F');
    dataloader->AddVariable( "LongestPfoLength", "L_{PFO, l}", "", 'F');
    dataloader->AddVariable( "LongestPfoTrackProbability", "P_{t, l}", "", 'F');
    dataloader->AddVariable( "LongestPfoPolarAngle", "#theta_{l}", "", 'F');
    dataloader->AddVariable( "LongestPfoCorrectedMinX", "x_{min, l}", "", 'F');
    dataloader->AddVariable( "LongestPfoCorrectedMaxX", "x_{max, l}", "", 'F');
    dataloader->AddVariable( "LongestPfoCorrectedMaxY", "y_{max, l}", "", 'F');

    //dataloader->AddVariable( "ShortestPfoCharge", "Q_{PFO, s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoLength", "L_{PFO, s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoTrackProbability", "P_{t, r}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoPolarAngle", "#theta_{s}", "", 'F');

    dataloader->AddVariable( "LongestPfoUpDownDeltaChiSquaredPerHit", "#Delta#chi^{2}_{DU,l}", "", 'F');
    dataloader->AddVariable( "LongestPfoDeltaChiSquaredPerHit", "#Delta#chi^{2}_{FB,l}", "", 'F');
    dataloader->AddVariable( "LongestPfoMinChiSquaredPerHit", "#chi^{2}_{min,l}", "", 'F');
    dataloader->AddVariable( "LongestPfoFitChargeRangeOverLength", "Q_{r,l}/L_{l}", "", 'F');
    dataloader->AddVariable( "LongestPfoMinSumQW", "Q_{min,l}", "", 'F');

    //dataloader->AddVariable( "ShortestPfoUpDownDeltaChiSquaredPerHit", "#Delta#chi^{2}_{DU,s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoDeltaChiSquaredPerHit", "#Delta#chi^{2}_{FB,s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoMinChiSquaredPerHit", "#chi^{2}_{min,s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoFitChargeRangeOverLength", "Q_{r,s}/L_{s}", "", 'F');
    //dataloader->AddVariable( "ShortestPfoMinSumQW", "Q_{min,s}", "", 'F');

    dataloader->AddVariable( "LongestPfoRecoDeltaY", "#Deltay_{l}", "", 'F');
    dataloader->AddVariable( "LongestPfoRecoLowestTenCmTotalCharge", "Q_{10cm,l}", "", 'F');

    dataloader->AddVariable( "LongestPfoMinChiSquaredPerHit3D", "MinChiSquaredPerHit3D", "", 'F');
    //dataloader->AddVariable( "ShortestPfoMinChiSquaredPerHit3D", "Shortest PFO MinChiSquaredPerHit3D", "", 'F');

    TFile *pFile = new TFile("/usera/jjd49/pandora_direction/CondorUtilities/saved_results/event_selection/0.root");
    TTree* pTree = (TTree*)(pFile->Get("EventSelection"));

    dataloader->AddSignalTree(pTree, 1.f);
    dataloader->AddBackgroundTree(pTree, 1.f);

    //EVENT SELECTION
    TCut signalCut = "(RecoNeutrinoNumberAssociatedParticles == 1 && Signal == 1 && NuRecoContained == 1)";
    TCut backgroundCut = "(RecoNeutrinoNumberAssociatedParticles == 1 && Signal == 0 && NuRecoContained == 1)";

    //COSMIC
    //TCut signalCut = "(RecoNeutrinoNumberAssociatedParticles == 2 && RecoNeutrinoNumberCosmicRays == 0 && NuRecoContained == 1)";
    //TCut backgroundCut = "(RecoNeutrinoNumberAssociatedParticles == 2 && RecoNeutrinoNumberCosmicRays != 0 && NuRecoContained == 1)";

    //PARTICLE ID
    //TCut signalCut = "(RecoNeutrinoNumberAssociatedParticles == 2 && NumberTrueProtons == 1 && NuRecoContained == 1)";
    //TCut backgroundCut = "(RecoNeutrinoNumberAssociatedParticles == 2 && NumberTrueProtons == 0 && NuRecoContained == 1)";

    dataloader->PrepareTrainingAndTestTree(signalCut, backgroundCut, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V");

    factory->BookMethod(dataloader, TMVA::Types::kBDT, "BDT", "!H:!V:NTrees=250:MinNodeSize=2.5%:MaxDepth=5:BoostType=AdaBoost:AdaBoostBeta=0.5:UseBaggedBoost:BaggedSampleFraction=0.5:SeparationType=GiniIndex:nCuts=20"); 

    // ---- Now you can tell the factory to train, test, and evaluate the MVAs

    // Train MVAs using the set of training events
    factory->TrainAllMethods();

    // ---- Evaluate all MVAs using the set of test events
    factory->TestAllMethods();

    // ----- Evaluate and compare performance of all configured MVAs
    factory->EvaluateAllMethods();

    // --------------------------------------------------------------

    // Save the output
    outputFile->Close();

    std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl
              << "==> TMVAClassification is done!" << std::endl
              << std::endl
              << "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << std::endl
              << std::endl;

    // Clean up
    delete factory;
    delete dataloader;
}

//=====================================================================
